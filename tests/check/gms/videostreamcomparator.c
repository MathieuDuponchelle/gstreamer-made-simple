#include <gms/gms.h>
#include <gst/check/gstcheck.h>

static void
generate_samples (void)
{
  gms_generate_test_file_audio_video_sync (g_strconcat (g_get_current_dir (),
          "/ball_sine.ogg", NULL), "vorbisenc", "theoraenc", "oggmux", "ball",
      NULL, 5);
}

GST_START_TEST (same_files)
{
}

GST_END_TEST;

static Suite *
ges_suite (void)
{
  Suite *s = suite_create ("Video Stream Comparator");
  TCase *tc_chain = tcase_create ("video-stream-comparator");

  gms_init ();
  suite_add_tcase (s, tc_chain);

  generate_samples ();

  tcase_add_test (tc_chain, same_files);

  return s;
}

GST_CHECK_MAIN (ges);
