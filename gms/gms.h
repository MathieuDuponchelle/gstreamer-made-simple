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

#ifndef __GMS_H__
#define __GMS_H__

#include <glib.h>
#include <gst/gst.h>

#include <gms/gms-types.h>
#include <gms/gms-enums.h>

#include <gms/gms-video-stream-comparator.h>
#include <gms/gms-waveformer.h>
#include <gms/gms-cairo-waveform-renderer.h>
#include <gms/gms-utils.h>

G_BEGIN_DECLS

#define GMS_VERSION_MAJOR (1)
#define GMS_VERSION_MINOR (0)
#define GMS_VERSION_MICRO (0)
#define GMS_VERSION_NANO  (0)

gboolean gms_init(void);

void     gms_version (guint * major, guint * minor, guint * micro,
                      guint * nano);

G_END_DECLS

#endif /* __GMS_H__ */
