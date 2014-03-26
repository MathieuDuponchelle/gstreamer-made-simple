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

#include "gms-utils.h"


/**
 * gms_generate_test_file_audio_video_sync:
 * @location: the location at which to save the generated file.
 * @audio_encoder: (allow-none): the name of the audio encoder, such as "vorbisenc",
 *   or NULL if no audio encoding.
 * @video_encoder: the name of the video encoder, such as "theoraenc"
 * @muxer: the name of the muxer, such as "oggmux"
 * @video_pattern: (allow-none): the name of the video pattern, such as "ball", or NULL
 *   for default.
 * @audio_wave: (allow-none): the name of the audio wave, such as "blue-noise", or NULL
 *  for default.
 * @duration: the duration in seconds of the resulting file.
 *
 * Returns: TRUE if the file was created or already exited, FALSE in
 * case of any errors.
 *
 * Will create an audio / video file at the specified location.
 */
gboolean
gms_generate_test_file_audio_video_sync (const gchar * location,
    const gchar * audio_encoder,
    const gchar * video_encoder,
    const gchar * muxer, const gchar * video_pattern, const gchar * audio_wave,
    gint duration)
{
  GError *error = NULL;
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *message;
  gchar *pipeline_str;
  gboolean done = FALSE;
  gboolean ret = FALSE;
  gint num_buffers_audio = 43 * duration;
  gint num_buffers_video = 30 * duration;

  if (g_file_test (location, G_FILE_TEST_EXISTS)) {
    GST_INFO ("The file %s already existed.", location);
    return TRUE;
  }

  g_return_val_if_fail (video_encoder != NULL, FALSE);
  g_return_val_if_fail (muxer != NULL, FALSE);

  pipeline_str = g_strdup_printf ("audiotestsrc num-buffers=%d wave=%s "
      "%c %s ! %s name=m ! filesink location=%s "
      "videotestsrc pattern=%s num-buffers=%d ! %s ! m.",
      num_buffers_audio,
      audio_wave ? audio_wave : "sine",
      audio_encoder ? '!' : ' ',
      audio_encoder ? audio_encoder : "",
      muxer,
      location,
      video_pattern ? video_pattern : "smpte",
      num_buffers_video, video_encoder);

  pipeline = gst_parse_launch (pipeline_str, &error);

  if (pipeline == NULL)
    return FALSE;

  g_free (pipeline_str);

  bus = gst_element_get_bus (GST_ELEMENT (pipeline));
  gst_bus_add_signal_watch (bus);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  while (!done) {
    message = gst_bus_poll (bus, GST_MESSAGE_ANY, GST_CLOCK_TIME_NONE);
    if (GST_MESSAGE_TYPE (message) == GST_MESSAGE_EOS) {
      done = TRUE;
      ret = TRUE;
    } else if (GST_MESSAGE_TYPE (message) == GST_MESSAGE_ERROR) {
      gchar *debug = NULL;
      GError *err = NULL;

      gst_message_parse_error (message, &err, &debug);
      done = TRUE;
      ret = FALSE;
      GST_ERROR ("Got error %s from %s fron the bus while generation: %s"
          "debug infos: %s", GST_OBJECT_NAME (message->src), err->message,
          debug ? debug : "none", location);
      g_clear_error (&err);
      g_free (debug);
    }
  }

  gst_bus_remove_signal_watch (bus);
  gst_object_unref (bus);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);

  return ret;
}
