#include "gms.h"

GST_DEBUG_CATEGORY (_gms_debug);


/**
 * SECTION:gms-common
 * @short_description: Initialization.
 */

/**
 * gms_init:
 *
 * Does nothing. Might do something in the future so it's a good idea
 * to include it from the get go.
 */
gboolean
gms_init (void)
{
  return TRUE;
}

/**
 * gms_version:
 * @major: (out): pointer to a guint to store the major version number
 * @minor: (out): pointer to a guint to store the minor version number
 * @micro: (out): pointer to a guint to store the micro version number
 * @nano:  (out): pointer to a guint to store the nano version number
 *
 * Gets the version number of the GStreamer Made Simple library.
 */
void
gms_version (guint * major, guint * minor, guint * micro, guint * nano)
{
  g_return_if_fail (major);
  g_return_if_fail (minor);
  g_return_if_fail (micro);
  g_return_if_fail (nano);

  *major = GMS_VERSION_MAJOR;
  *minor = GMS_VERSION_MINOR;
  *micro = GMS_VERSION_MICRO;
  *nano = GMS_VERSION_NANO;
}
