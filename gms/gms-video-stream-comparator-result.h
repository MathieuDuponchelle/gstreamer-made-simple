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


#ifndef _GMS_VIDEO_STREAM_COMPARATOR_RESULT
#define _GMS_VIDEO_STREAM_COMPARATOR_RESULT

#include <glib-object.h>
#include <gst/gst.h>
#include <gms/gms-types.h>
#include <gms/gms-enums.h>

G_BEGIN_DECLS

#define GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT            gms_video_stream_comparator_result_get_type()
#define GMS_VIDEO_STREAM_COMPARATOR_RESULT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT, GMSVideoStreamComparatorResult))
#define GMS_VIDEO_STREAM_COMPARATOR_RESULT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT, GMSVideoStreamComparatorResultClass))
#define GMS_IS_VIDEO_STREAM_COMPARATOR_RESULT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT))
#define GMS_IS_VIDEO_STREAM_COMPARATOR_RESULT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT))
#define GMS_VIDEO_STREAM_COMPARATOR_RESULT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GMS_TYPE_VIDEO_STREAM_COMPARATOR_RESULT, GMSVideoStreamComparatorResultClass))

typedef struct _GMSVideoStreamComparatorResultPrivate GMSVideoStreamComparatorResultPrivate;

/**
 * GMSVideoStreamComparatorResult:
 */
struct _GMSVideoStreamComparatorResult
{
  GObject parent;

  /*< public >*/
  /* READ-ONLY */

  /*< private >*/
  GMSVideoStreamComparatorResultPrivate* priv;
  /* Padding for API extension */
  gpointer         _gms_reserved[GMS_PADDING];
};

/**
 * GMSVideoStreamComparatorResultClass:
 */
struct _GMSVideoStreamComparatorResultClass
{
  /*< private >*/
  GObjectClass parent_class;

  /* Padding for API extension */
  gpointer    _gms_reserved[GMS_PADDING];
};

GType gms_video_stream_comparator_result_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _GMS_VIDEO_STREAM_COMPARATOR_RESULT */
