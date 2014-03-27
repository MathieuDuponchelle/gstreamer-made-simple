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
#include "gms-waveformer.h"

struct _GMSWaveformerPrivate
{
  gpointer nothing;
};

G_DEFINE_TYPE (GMSWaveformer, gms_waveformer, G_TYPE_OBJECT);

static void
gms_waveformer_init (GMSWaveformer * wf)
{
  wf->priv =
      G_TYPE_INSTANCE_GET_PRIVATE (wf, GMS_TYPE_WAVEFORMER,
      GMSWaveformerPrivate);
}

static void
gms_waveformer_class_init (GMSWaveformerClass * klass)
{
  g_type_class_add_private (klass, sizeof (GMSWaveformerPrivate));
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
