#include <gms/gms.h>
#include <gst/check/gstcheck.h>

static void
generate_samples (void)
{
  gms_generate_test_file_audio_video_sync (g_strconcat (g_get_current_dir (),
          "/ball_sine.ogg", NULL), "vorbisenc", "theoraenc", "oggmux", "ball",
      NULL, 1);

  gms_generate_test_file_audio_video_sync (g_strconcat (g_get_current_dir (),
          "/smpte_sine_1.ogg", NULL), "vorbisenc", "theoraenc", "oggmux", NULL,
      NULL, 1);
}

GST_START_TEST (same_files)
{
  GList *res;
  GMSVideoStreamComparator *comp;
  gchar *uri;

  comp = gms_video_stream_comparator_new ();
  uri = g_filename_to_uri (g_strconcat (g_get_current_dir (),
          "/ball_sine.ogg", NULL), NULL, NULL);

  gms_video_stream_comparator_set_reference_uri (comp, uri, 0, 0);
  gms_video_stream_comparator_add_compared_uri (comp, uri, 0, 0);
  res = gms_video_stream_comparator_compare_sync (comp, 0.95, 0);

  gms_video_stream_comparator_print_results_for_uri (comp, uri, 0);
  fail_unless (res == NULL);
}

GST_END_TEST;

GST_START_TEST (different_files)
{
  GList *res;
  GMSVideoStreamComparator *comp = gms_video_stream_comparator_new ();
  gchar *reference_uri = g_filename_to_uri (g_strconcat (g_get_current_dir (),
          "/ball_sine.ogg", NULL), NULL, NULL);
  gchar *compared_uri = g_filename_to_uri (g_strconcat (g_get_current_dir (),
          "/smpte_sine_1.ogg", NULL), NULL, NULL);

  gms_video_stream_comparator_set_reference_uri (comp, reference_uri, 0, 0);
  gms_video_stream_comparator_add_compared_uri (comp, compared_uri, 0, 0);
  res = gms_video_stream_comparator_compare_sync (comp, 0.95, 0);
  gms_video_stream_comparator_print_results_for_uri (comp, compared_uri,
      GMS_VIDEO_STREAM_COMPARATOR_REPORT_LEVEL_PERCENTAGE_FAILING);
  fail_unless (res != NULL);
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
  tcase_add_test (tc_chain, different_files);

  return s;
}

GST_CHECK_MAIN (ges);
