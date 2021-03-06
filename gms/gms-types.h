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

#ifndef __GMS_TYPES_H__
#define __GMS_TYPES_H__

/* Padding */
#define GMS_PADDING         4

/* Type definitions */

typedef struct _GMSVideoStreamComparator GMSVideoStreamComparator;
typedef struct _GMSVideoStreamComparatorClass GMSVideoStreamComparatorClass;

typedef struct _GMSVideoStreamComparatorResult GMSVideoStreamComparatorResult;
typedef struct _GMSVideoStreamComparatorResultClass GMSVideoStreamComparatorResultClass;

typedef struct _GMSWaveformer GMSWaveformer;
typedef struct _GMSWaveformerClass GMSWaveformerClass;

typedef struct _GMSCairoWaveformRenderer GMSCairoWaveformRenderer;
typedef struct _GMSCairoWaveformRendererClass GMSCairoWaveformRendererClass;

#endif /* __GMS_TYPES_H__ */
