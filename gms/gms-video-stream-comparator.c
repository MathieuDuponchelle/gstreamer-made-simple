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

#include <string.h>
#include "gms-video-stream-comparator.h"

GMSVideoStreamComparatorResult *gms_video_stream_comparator_result_new (void);

struct _GMSVideoStreamComparatorPrivate
{
  gchar *reference_uri;
  GMainLoop *loop;
  GstElement *ssim;
  GList *streams;
  gfloat tolerance;
};

struct _GMSVideoStreamComparatorResultPrivate
{
  gchar *uri;
  gfloat matching_percentage;
  guint total_frames_number;
  guint failed_frames_number;
  GList *frame_results;
};

G_DEFINE_TYPE (GMSVideoStreamComparator, gms_video_stream_comparator,
    G_TYPE_OBJECT);

G_DEFINE_TYPE (GMSVideoStreamComparatorResult,
    gms_video_stream_comparator_result, G_TYPE_OBJECT);


static void
gms_video_stream_comparator_result_init (GMSVideoStreamComparatorResult * res)
{
  res->priv =
      G_TYPE_INSTANCE_GET_PRIVATE (res, GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT,
      GMSVideoStreamComparatorResultPrivate);

  res->priv->matching_percentage = 1.0;
  res->priv->frame_results = NULL;
}

static void
gms_video_stream_comparator_init (GMSVideoStreamComparator * comp)
{
  comp->priv =
      G_TYPE_INSTANCE_GET_PRIVATE (comp, GMS_TYPE_VIDEO_STREAM_COMPARATOR,
      GMSVideoStreamComparatorPrivate);
  comp->priv->reference_uri = NULL;
  comp->priv->streams = NULL;
  comp->priv->tolerance = 0.95;
}

static void
gms_video_stream_comparator_dispose (GObject * object)
{
  GMSVideoStreamComparator *comp = GMS_VIDEO_STREAM_COMPARATOR (object);

  g_free (comp->priv->reference_uri);
  g_list_free (comp->priv->streams);
}

static void
gms_video_stream_comparator_finalize (GObject * object)
{
  G_OBJECT_CLASS (gms_video_stream_comparator_parent_class)->finalize (object);
}

/**
 * gms_video_stream_comparator_set_reference_uri:
 * @comparator: a #GMSVideoStreamComparator
 * @uri: the uri of the reference file.
 * @inpoint: the inpoint in the reference file, or -1 if from 0
 * @duration: the duration starting from the inpoint, or -1 if rest of the file.
 *
 * Sets up the reference file against which other files will be compared.
 */
void
gms_video_stream_comparator_set_reference_uri (GMSVideoStreamComparator *
    comparator, const gchar * uri, guint64 inpoint, guint64 duration)
{
  /* FIXME */
  if (inpoint != -1 || duration != -1)
    GST_WARNING_OBJECT (comparator,
        "Comparing subsections of files is not implemented yet !");

  g_free (comparator->priv->reference_uri);
  comparator->priv->reference_uri = g_strdup (uri);
}


/**
 * gms_video_stream_comparator_add_compared_uri:
 * @comparator: a #GMSVideoStreamComparator
 * @uri: the uri of the reference file.
 * @inpoint: the inpoint in the reference file, or -1 if from 0
 * @duration: the duration starting from the inpoint, or -1 if rest of the file.
 *
 * Sets up the reference file against which other files will be compared.
 * Returns: %TRUE if the comparator accepts handling that uri, %FALSE otherwise.
 */
gboolean
gms_video_stream_comparator_add_compared_uri (GMSVideoStreamComparator *
    comparator, const gchar * uri, guint64 inpoint, guint64 duration)
{
  gboolean ret = TRUE;

  /* FIXME */
  if (inpoint != -1 || duration != -1)
    GST_WARNING_OBJECT (comparator,
        "Comparing subsections of files is not implemented yet !");

  if (g_list_length (comparator->priv->streams) > 0) {
    ret = FALSE;
    /* FIXME */
    GST_ERROR_OBJECT (comparator,
        "comparison of multiple uris at the same time is not implemented yet !");
  } else {
    GMSVideoStreamComparatorResult *res =
        gms_video_stream_comparator_result_new ();
    res->priv->uri = g_strdup (uri);
    comparator->priv->streams =
        g_list_append (comparator->priv->streams, (gpointer) res);
  }

  return ret;
}

static void
pad_added_cb (GstElement * element G_GNUC_UNUSED, GstPad * pad,
    GstPad * ssim_pad)
{
  gst_pad_link (pad, ssim_pad);
}

static gboolean
my_bus_callback (GstBus * bus, GstMessage * message,
    GMSVideoStreamComparator * comp)
{
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ELEMENT:
      if (GST_MESSAGE_SRC (message) == GST_OBJECT (comp->priv->ssim)) {
        const GstStructure *s = gst_message_get_structure (message);
        const GValue *v;
        gfloat mean;
        GList *tmp;
        guint64 timestamp;

        v = gst_structure_get_value (s, "mean");
        mean = g_value_get_float (v);
        v = gst_structure_get_value (s, "timestamp");
        timestamp = g_value_get_uint64 (v);
        for (tmp = comp->priv->streams; tmp; tmp = tmp->next) {
          GstTimedValue *frame_result;
          GMSVideoStreamComparatorResult *res =
              GMS_VIDEO_STREAM_COMPARATOR_RESULT (tmp->data);

          frame_result = g_slice_new (GstTimedValue);
          frame_result->timestamp = timestamp;
          frame_result->value = mean;
          res->priv->frame_results =
              g_list_append (res->priv->frame_results, frame_result);

          res->priv->total_frames_number += 1;
          if (mean < comp->priv->tolerance)
            res->priv->failed_frames_number += 1;
        }
      }
      break;
    case GST_MESSAGE_ERROR:{
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);

      g_assert_no_error (err);
      g_error_free (err);
      g_free (debug);
      g_main_loop_quit (comp->priv->loop);
      break;
    }
    case GST_MESSAGE_EOS:
      g_main_loop_quit (comp->priv->loop);
      break;
    default:
      /* unhandled message */
      break;
  }

  return TRUE;
}

/**
 * gms_video_stream_comparator_compare_sync:
 * @comparator: a #GMSVideoStreamComparator
 * @tolerance: the ratio of difference tolerated between two video frames.
 * @dump_directory: if specified, dump differing frames in that directory.
 *
 * Returns: (transfer full) (element-type utf8): %NULL if all the streams match,
 * a #GList of non matching uris otherwise.
 */
GList *
gms_video_stream_comparator_compare_sync (GMSVideoStreamComparator * comparator,
    gfloat tolerance, const gchar * dump_directory)
{
  GList *uris, *tmp;
  GstBus *bus;
  GstElement *pipeline, *reference_decodebin, *fakesink;
  GstPadTemplate *templ;
  GstPad *request_pad;

  uris = NULL;
  pipeline = gst_pipeline_new ("stream-comparer");
  reference_decodebin = gst_element_factory_make ("uridecodebin", NULL);
  fakesink = gst_element_factory_make ("fakesink", NULL);
  comparator->priv->ssim = gst_element_factory_make ("ssim", NULL);
  comparator->priv->loop = g_main_loop_new (NULL, FALSE);
  comparator->priv->tolerance = tolerance;

  if (!reference_decodebin || !fakesink || !comparator->priv->ssim) {
    GST_ERROR
        ("You might be missing some plugins, please check your installation");
    return uris;
  }

  templ =
      gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS
      (comparator->priv->ssim), "sink_%u");

  g_object_set (fakesink, "sync", FALSE, NULL);
  g_object_set (reference_decodebin, "uri", comparator->priv->reference_uri,
      "caps", gst_caps_from_string ("video/x-raw"), "expose-all-streams", FALSE,
      NULL);

  request_pad =
      gst_element_request_pad (comparator->priv->ssim, templ, NULL, NULL);

  g_signal_connect (reference_decodebin, "pad-added", G_CALLBACK (pad_added_cb),
      request_pad);

  gst_bin_add_many (GST_BIN (pipeline), reference_decodebin,
      comparator->priv->ssim, fakesink, NULL);

  gst_element_link (comparator->priv->ssim, fakesink);

  for (tmp = comparator->priv->streams; tmp; tmp = tmp->next) {
    GstElement *compared_decodebin =
        gst_element_factory_make ("uridecodebin", NULL);

    GMSVideoStreamComparatorResult *res =
        GMS_VIDEO_STREAM_COMPARATOR_RESULT (tmp->data);
    g_object_set (compared_decodebin, "uri", res->priv->uri, "caps",
        gst_caps_from_string ("video/x-raw"), "expose-all-streams", FALSE,
        NULL);
    request_pad =
        gst_element_request_pad (comparator->priv->ssim, templ, NULL, NULL);
    gst_bin_add (GST_BIN (pipeline), compared_decodebin);
    g_signal_connect (compared_decodebin, "pad-added",
        G_CALLBACK (pad_added_cb), request_pad);
  }

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_bus_add_watch (bus, (GstBusFunc) my_bus_callback, comparator);
  gst_object_unref (bus);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  gst_element_get_state (GST_ELEMENT (pipeline), NULL, NULL, -1);

  g_main_loop_run (comparator->priv->loop);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_element_get_state (GST_ELEMENT (pipeline), NULL, NULL, -1);

  for (tmp = comparator->priv->streams; tmp; tmp = tmp->next) {
    GMSVideoStreamComparatorResult *res =
        GMS_VIDEO_STREAM_COMPARATOR_RESULT (tmp->data);
    if (res->priv->total_frames_number > 0)
      res->priv->matching_percentage =
          (gfloat) (res->priv->total_frames_number -
          res->priv->failed_frames_number) / res->priv->total_frames_number;
    if (res->priv->failed_frames_number > 0)
      uris = g_list_append (uris, g_strdup (res->priv->uri));
  }

  return uris;
}

/**
 * gms_video_stream_comparator_print_results_for_uri:
 * @comparator: a #GMSVideoStreamComparator
 * @uri: The uri for which a report needs to be printed in stdout.
 * @level: a #GMSVideoStreamComparatorReportLevel
 *
 * Will print a report for the stream comparison, according to the level
 * of detail requested. By default, it will display the results for all the frames.
 */
void
gms_video_stream_comparator_print_results_for_uri (GMSVideoStreamComparator *
    comparator, const gchar * uri, GMSVideoStreamComparatorReportLevel level)
{
  GMSVideoStreamComparatorResult *res = NULL;
  GList *tmp;
  gboolean failing_only;
  size_t underlined_len = 0;

  failing_only = level & GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_FAILING_ONLY;

  for (tmp = comparator->priv->streams; tmp; tmp = tmp->next) {
    if (!g_strcmp0 (uri,
            GMS_VIDEO_STREAM_COMPARATOR_RESULT (tmp->data)->priv->uri))
      res = GMS_VIDEO_STREAM_COMPARATOR_RESULT (tmp->data);
  }

  if (res == NULL) {
    GST_WARNING_OBJECT (comparator, "Uri %s was not part of the compared uris",
        uri);
    return;
  }

  g_print ("\nMatching results for uri %s:\n", uri);
  g_print ("==========================");
  for (underlined_len = strlen (uri); underlined_len > 0; underlined_len--)
    g_print ("=");

  g_print ("\n\n");

  for (tmp = res->priv->frame_results; tmp; tmp = tmp->next) {
    GstTimedValue *frame_value = (GstTimedValue *) tmp->data;

    if (!failing_only || frame_value->value < comparator->priv->tolerance) {
      g_print ("Frame with timestamp %" GST_TIME_FORMAT " matches at %d %%\n",
          GST_TIME_ARGS (frame_value->timestamp),
          (gint) (frame_value->value * 100));
    }
  }

  if (level & GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_PERCENTAGE_FAILING)
    g_print ("\n%d %% of the frames matched\n\n",
        (gint) (res->priv->matching_percentage * 100));
}

static void
    gms_video_stream_comparator_result_class_init
    (GMSVideoStreamComparatorResultClass * klass)
{
  g_type_class_add_private (klass,
      sizeof (GMSVideoStreamComparatorResultPrivate));
}

static void
gms_video_stream_comparator_class_init (GMSVideoStreamComparatorClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (GMSVideoStreamComparatorPrivate));

  object_class->finalize = gms_video_stream_comparator_finalize;
  object_class->dispose = gms_video_stream_comparator_dispose;
}

GMSVideoStreamComparatorResult *
gms_video_stream_comparator_result_new (void)
{
  GMSVideoStreamComparatorResult *res;

  res = g_object_new (GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT, NULL);

  return res;
}

/**
 * gms_video_stream_comparator_new:
 *
 * Creates a new #GMSVideoStreamComparator
 *
 * Returns: A new #GMSVideoStreamComparator.
 */
GMSVideoStreamComparator *
gms_video_stream_comparator_new (void)
{
  GMSVideoStreamComparator *comp;

  comp = g_object_new (GMS_TYPE_VIDEO_STREAM_COMPARATOR, NULL);

  return comp;
}
