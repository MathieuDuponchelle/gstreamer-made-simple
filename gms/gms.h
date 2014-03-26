#ifndef __GMS_H__
#define __GMS_H__

#include <glib.h>
#include <gst/gst.h>

#include <gms/gms-types.h>

#include <gms/gms-video-stream-comparator.h>

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
