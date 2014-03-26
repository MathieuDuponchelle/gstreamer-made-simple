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


#ifndef __GMS_ENUMS_H__
#define __GMS_ENUMS_H__

G_BEGIN_DECLS

/**
 * SECTION:gms-enums
 * @short_description: Various enums for GStreamer made simple.
 */

/**
 * GMSVideoStreamComparatorReportLevel:
 * @GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_FAILING_ONLY: The report will only show
 *  non-matching frames.
 * @GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_PERCENTAGE_FAILING: The report will
 * display a percentage of non-matching frames at the end.
 *
 * The various levels of reporting for a stream comparison
 */
typedef enum {
  GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_FAILING_ONLY = 1 << 0,
  GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_PERCENTAGE_FAILING = 1 << 1,
} GMSVideoStreamComparatorReportLevel;

G_END_DECLS

#endif /* __GMS_ENUMS_H__ */
