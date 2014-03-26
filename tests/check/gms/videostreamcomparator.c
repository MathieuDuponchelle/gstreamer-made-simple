#include <gms/gms.h>
#include <gst/check/gstcheck.h>

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

  tcase_add_test (tc_chain, same_files);

  return s;
}

GST_CHECK_MAIN (ges);
