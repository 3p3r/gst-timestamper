#ifndef __GST_TIME_STAMPER_H__
#define __GST_TIME_STAMPER_H__

#include <gst/gst.h>
#include <gst/video/video.h>

G_BEGIN_DECLS

#define GST_TYPE_TIME_STAMPER (gst_timestamper_get_type())
#define GST_TIME_STAMPER(obj)                                                  \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_TIME_STAMPER, GstTimeStamper))
#define GST_TIME_STAMPER_CLASS(klass)                                          \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_TIME_STAMPER, GstTimeStamperClass))
#define GST_TIME_STAMPER_GET_CLASS(obj)                                        \
  (G_TYPE_INSTANCE_GET_CLASS((obj), GST_TYPE_TIME_STAMPER, GstTimeStamperClass))
#define GST_IS_TIME_STAMPER(obj)                                               \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_TIME_STAMPER))
#define GST_IS_TIME_STAMPER_CLASS(klass)                                       \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_TIME_STAMPER))

typedef struct _GstTimeStamper GstTimeStamper;
typedef struct _GstTimeStamperClass GstTimeStamperClass;

/**
 * GstTimeStamper:
 *
 * Opaque data structure.
 */
struct _GstTimeStamper {
  GstBaseTransform videofilter;

  /* < private > */
  gpointer file_writer_pipeline;
  gpointer file_writer_element;
};

struct _GstTimeStamperClass {
  GstBaseTransformClass parent_class;
};

GType gst_timestamper_get_type(void);

G_END_DECLS
#endif /* __GST_TIME_STAMPER_H__ */