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


#ifndef _GMS_WAVEFORMER
#define _GMS_WAVEFORMER

#include <glib-object.h>
#include <gst/gst.h>
#include <gms/gms-types.h>
#include <gms/gms-enums.h>

G_BEGIN_DECLS

/**
 * SECTION:gms-waveformer
 * @short_description: Object that collects waveform information.
 */

#define GMS_TYPE_WAVEFORMER            gms_waveformer_get_type()
#define GMS_WAVEFORMER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMS_TYPE_WAVEFORMER, GMSWaveformer))
#define GMS_WAVEFORMER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GMS_TYPE_WAVEFORMER, GMSWaveformerClass))
#define GMS_IS_WAVEFORMER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMS_TYPE_WAVEFORMER))
#define GMS_IS_WAVEFORMER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMS_TYPE_WAVEFORMER))
#define GMS_WAVEFORMER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GMS_TYPE_WAVEFORMER, GMSWaveformerClass))

typedef struct _GMSWaveformerPrivate GMSWaveformerPrivate;

/**
 * GMSWaveformer:
 */
struct _GMSWaveformer
{
  GObject parent;

  /*< public >*/
  /* READ-ONLY */

  /*< private >*/
  GMSWaveformerPrivate* priv;
  /* Padding for API extension */
  gpointer         _gms_reserved[GMS_PADDING];
};

/**
 * GMSWaveformerClass:
 */
struct _GMSWaveformerClass
{
  /*< private >*/
  GObjectClass parent_class;

  /* Padding for API extension */
  gpointer    _gms_reserved[GMS_PADDING];
};

GType gms_waveformer_get_type (void) G_GNUC_CONST;
GMSWaveformer * gms_waveformer_new (void);

gboolean
gms_waveformer_set_uri(GMSWaveformer *waveformer, const gchar *uri, gboolean start_waveforming);

gboolean
gms_waveformer_set_pipeline (GMSWaveformer *waveformer, GstPipeline *pipeline, gboolean start_waveforming);

GstClockTime
gms_waveformer_get_current_duration(GMSWaveformer *wf);

G_END_DECLS

#endif /* _GMS_WAVEFORMER */
