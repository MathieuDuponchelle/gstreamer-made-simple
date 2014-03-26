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

#include "gms-video-stream-comparator.h"

struct _GMSVideoStreamComparatorPrivate
{
  gpointer nothing;
};

#define GMS_VIDEO_STREAM_COMPARATOR_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMS_TYPE_VIDEO_STREAM_COMPARATOR, GMSVideoStreamComparator))

G_DEFINE_TYPE (GMSVideoStreamComparator, gms_video_stream_comparator,
    G_TYPE_OBJECT);

static void
gms_video_stream_comparator_init (GMSVideoStreamComparator * comp)
{
}

static void
gms_video_stream_comparator_finalize (GObject * object)
{
  G_OBJECT_CLASS (gms_video_stream_comparator_parent_class)->finalize (object);
}

static void
gms_video_stream_comparator_class_init (GMSVideoStreamComparatorClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gms_video_stream_comparator_finalize;
}

GMSVideoStreamComparator *
gms_video_stream_comparator_new (void)
{
  GMSVideoStreamComparator *comp;

  comp = g_object_new (GMS_TYPE_VIDEO_STREAM_COMPARATOR, NULL);

  return comp;
}
