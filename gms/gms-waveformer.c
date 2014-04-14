/* GStreamer Made Simple
 * Copyright (C) 2014 Mathieu Duponchelle <mathieu.duponchelle@opencreed.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <math.h>               /* pow For getting on a 0 <-> 100 scale */
#include "gms-waveformer.h"
#include <gst/pbutils/pbutils.h>

enum
{
  PEAK_ADDED,
  LAST_SIGNAL
};

static guint gms_waveformer_signals[LAST_SIGNAL] = { 0 };

struct _GMSWaveformerPrivate
{
  GList *peaks;
  gchar *uri;
  GstPipeline *pipeline;
  gboolean is_pipeline_owner;
  guint bus_watch_id;
  GstElement *level;
  GstClockTime duration;
};

G_DEFINE_TYPE (GMSWaveformer, gms_waveformer, G_TYPE_OBJECT);

static void
gms_waveformer_init (GMSWaveformer * wf)
{
  wf->priv =
      G_TYPE_INSTANCE_GET_PRIVATE (wf, GMS_TYPE_WAVEFORMER,
      GMSWaveformerPrivate);
  wf->priv->peaks = NULL;
}

static void
gms_waveformer_class_init (GMSWaveformerClass * klass)
{
  g_type_class_add_private (klass, sizeof (GMSWaveformerPrivate));

  /**
   * GMSWaveformer:peak-added:
   * @object: the #GESWaveformer
   * @peak: the #GstTimedValue peak that was added.
   *
   * Will be emitted when a peak is added to the waveformer.
   */
  gms_waveformer_signals[PEAK_ADDED] =
      g_signal_new ("peak-added", G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_FIRST, 0, NULL, NULL, g_cclosure_marshal_generic,
      G_TYPE_NONE, 1, G_TYPE_POINTER);
}

static gboolean
wave_bus_callback (GstBus * bus, GstMessage * message, GMSWaveformer * wf)
{
  GMSWaveformerPrivate *priv = wf->priv;

  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ELEMENT:
      if (GST_MESSAGE_SRC (message) == GST_OBJECT (wf->priv->level)) {
        const GstStructure *s = gst_message_get_structure (message);
        const GValue *array_val;
        GValueArray *arr = NULL;
        const GValue *v;
        GstTimedValue *value = g_slice_new (GstTimedValue);
        gdouble rms = 0;
        gint i;
        guint64 stream_time;

        /* FIXME : make level not use GValueArrays to avoid deprecation warnings. */
        v = gst_structure_get_value (s, "stream-time");
        stream_time = g_value_get_uint64 (v);
        array_val = gst_structure_get_value (s, "rms");
        arr = (GValueArray *) g_value_get_boxed (array_val);
        for (i = 0; i < arr->n_values; i++) {
          v = g_value_array_get_nth (arr, i);
          rms += g_value_get_double (v);
        }
        rms /= arr->n_values;
        value->timestamp = stream_time;
        value->value = pow (10, rms / 20.0) * 100;
        priv->peaks = g_list_append (priv->peaks, value);
        g_signal_emit (wf, gms_waveformer_signals[PEAK_ADDED], 0,
            (gpointer) value);
      }
      break;
    case GST_MESSAGE_ERROR:{
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);

      g_assert_no_error (err);
      g_error_free (err);
      g_free (debug);
      if (priv->is_pipeline_owner)
        gst_element_set_state (GST_ELEMENT (priv->pipeline), GST_STATE_NULL);
      break;
    }
    case GST_MESSAGE_EOS:{
      GST_ERROR ("EOS allright");
      if (priv->is_pipeline_owner)
        gst_element_set_state (GST_ELEMENT (priv->pipeline), GST_STATE_NULL);
      break;
    }
    default:
      /* unhandled message */
      break;
  }

  return TRUE;
}

static guint
gms_pipeline_add_watch (GstPipeline * pipeline, GstBusFunc bus_callback,
    void *data)
{
  GstBus *bus;

  bus = gst_pipeline_get_bus (pipeline);
  return gst_bus_add_watch (bus, bus_callback, data);
}

static gint
compare_type_name (const GValue * item, const gchar * type_name)
{
  GstElement *element = g_value_get_object (item);

  return g_strcmp0 (type_name, G_OBJECT_TYPE_NAME (element));
}

static GstElement *
find_by_type_name (GstBin * bin, const gchar * type_name)
{
  GstIterator *it;
  GstElement *element = NULL;
  gboolean found;
  GValue item = { 0, };

  it = gst_bin_iterate_recurse (bin);
  found = gst_iterator_find_custom (it,
      (GCompareFunc) compare_type_name, &item, (void *) type_name);
  gst_iterator_free (it);
  if (found) {
    element = g_value_get_object (&item);
    g_value_unset (&item);
  }

  return element;
}

static gboolean
monitor_pipeline (GMSWaveformer * wf)
{
  gboolean res = TRUE;
  GMSWaveformerPrivate *priv = wf->priv;

  priv->level = find_by_type_name (GST_BIN (wf->priv->pipeline), "GstLevel");
  if (!priv->level) {
    res = FALSE;
    goto beach;
  }

  priv->bus_watch_id =
      gms_pipeline_add_watch (wf->priv->pipeline,
      (GstBusFunc) wave_bus_callback, wf);

beach:
  return res;
}

static void
uri_discovered_cb (GstDiscoverer * disco, GstDiscovererInfo * info,
    GError * disco_error, GMSWaveformer * wf)
{
  GMSWaveformerPrivate *priv;
  gchar *launch_string;
  GError *error;

  priv = wf->priv;

  priv->duration = gst_discoverer_info_get_duration (info);

  launch_string =
      g_strdup_printf ("uridecodebin uri=%s expose-all-streams=false "
      "caps=audio/x-raw ! level interval=10000000 ! pulsesink sync=true",
      priv->uri);

  error = NULL;
  priv->pipeline = GST_PIPELINE (gst_parse_launch (launch_string, &error));

  if (!priv->pipeline) {
    GST_ERROR ("Cannot construct waveforming pipeline, the error is : %s",
        error->message);
    return;
  }

  priv->is_pipeline_owner = TRUE;
  monitor_pipeline (wf);

  gst_element_set_state (GST_ELEMENT (priv->pipeline), GST_STATE_PLAYING);
}

static gboolean
start_pipeline_for_uri (GMSWaveformer * wf)
{
  GstDiscoverer *disco;

  disco = gst_discoverer_new (3 * GST_SECOND, NULL);
  g_signal_connect (disco, "discovered", G_CALLBACK (uri_discovered_cb), wf);
  gst_discoverer_start (disco);
  gst_discoverer_discover_uri_async (disco, wf->priv->uri);

  return TRUE;
}

/**
 * gms_waveformer_set_uri:
 * @waveformer: A #GMSWaveformer
 * @uri: the uri to monitor for waveform levels
 * @start_waveforming: Whether to start monitoring immediately
 *
 */
gboolean
gms_waveformer_set_uri (GMSWaveformer * waveformer, const gchar * uri,
    gboolean start_waveforming)
{
  gboolean res = TRUE;
  GMSWaveformerPrivate *priv = waveformer->priv;

  priv->uri = g_strdup (uri);

  if (start_waveforming)
    res = start_pipeline_for_uri (waveformer);

  return res;
}

/**
 * gms_waveformer_set_pipeline:
 * @waveformer: A #GMSWaveformer
 * @pipeline: the pipeline to monitor for waveform levels
 * @start_waveforming: Whether to start monitoring immediately
 *
 */
gboolean
gms_waveformer_set_pipeline (GMSWaveformer * waveformer, GstPipeline * pipeline,
    gboolean start_waveforming)
{
  GMSWaveformerPrivate *priv = waveformer->priv;

  priv->is_pipeline_owner = FALSE;
  priv->pipeline = pipeline;
  return monitor_pipeline (waveformer);
}

/**
 * gms_waveformer_get_duration:
 * @waveformer: A #GMSWaveformer
 *
 * Returns: The duration of the current uri or the amount monitored
 * on the set pipeline.
 */
GstClockTime
gms_waveformer_get_current_duration (GMSWaveformer * wf)
{
  return wf->priv->duration;
}

/**
 * gms_waveformer_new:
 *
 * Creates a new #GMSWaveformer
 *
 * Returns: A new #GMSWaveformer.
 */
GMSWaveformer *
gms_waveformer_new (void)
{
  GMSWaveformer *wf;

  wf = g_object_new (GMS_TYPE_WAVEFORMER, NULL);

  return wf;
}
