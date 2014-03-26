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

  return ret;
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
  GList *res = NULL;

  return res;
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
}


static void
gms_video_stream_comparator_class_init (GMSVideoStreamComparatorClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gms_video_stream_comparator_finalize;
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
