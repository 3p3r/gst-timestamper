#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "timestamper.h"

#include <gst/app/gstappsrc.h>
#include <gst/video/video.h>

#include <chrono>
#include <iostream>
#include <sstream>

namespace {
void write_to_timestamp_file(void *element, const std::string &data) {
  GstMapInfo map{};
  auto buffer = gst_buffer_new_and_alloc(data.size());
  gst_buffer_map(buffer, &map, GST_MAP_WRITE);
  memcpy(map.data, data.c_str(), map.size);
  gst_buffer_unmap(buffer, &map);
  gst_app_src_push_buffer(GST_APP_SRC(element), buffer);
} // write_to_timestamp_file
} // namespace

GST_DEBUG_CATEGORY_STATIC(timestamper_debug);
#define GST_CAT_DEFAULT timestamper_debug

static GstStaticPadTemplate gst_timestamper_src_template =
    GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw"));

static GstStaticPadTemplate gst_timestamper_sink_template =
    GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw"));

static void gst_timestamper_dispose(GObject *object);
static GstFlowReturn gst_timestamper_transform_ip(GstBaseTransform *vfilter,
                                                  GstBuffer *buffer);

G_DEFINE_TYPE(GstTimeStamper, gst_timestamper, GST_TYPE_BASE_TRANSFORM);

static void gst_timestamper_class_init(GstTimeStamperClass *klass) {
  GObjectClass *gobject_class = (GObjectClass *)klass;
  GstElementClass *element_class = (GstElementClass *)klass;
  GstBaseTransformClass *trans_class = (GstBaseTransformClass *)klass;

  GST_DEBUG_CATEGORY_INIT(timestamper_debug, "timestamper", 0, "timestamper");
  gst_element_class_set_static_metadata(
      element_class, "Time stamper", "Filter/Video",
      "Writes a timestamp for each video frame it receives into a file",
      "Sepehr Laal <info@virtulabs.com");

  gobject_class->dispose = GST_DEBUG_FUNCPTR(gst_timestamper_dispose);
  trans_class->transform_ip = GST_DEBUG_FUNCPTR(gst_timestamper_transform_ip);

  gst_element_class_add_pad_template(
      element_class,
      gst_static_pad_template_get(&gst_timestamper_sink_template));
  gst_element_class_add_pad_template(
      element_class,
      gst_static_pad_template_get(&gst_timestamper_src_template));
}

static void gst_timestamper_init(GstTimeStamper *timestamper) {
  GError *error = nullptr;

  GST_DEBUG("Launching the file writer pipeline");
  timestamper->file_writer_pipeline =
      gst_parse_launch("appsrc is-live=true name=writer ! queue ! filesink "
                       "append=true sync=false location=timestamps.txt",
                       &error);
  g_assert_null(error);
  g_assert_nonnull(timestamper->file_writer_pipeline);

  GST_DEBUG("Acquiring the file writer element");
  timestamper->file_writer_element =
      gst_bin_get_by_name(GST_BIN(timestamper->file_writer_pipeline), "writer");
  g_assert_nonnull(timestamper->file_writer_element);

  GST_DEBUG("Setting file writer pipeline to PLAYING");
  g_assert(GST_STATE_CHANGE_FAILURE !=
           gst_element_set_state(GST_ELEMENT(timestamper->file_writer_pipeline),
                                 GST_STATE_PLAYING));
  std::stringstream ss;
  ss << "=== unit: ns (unix timestamps)" << std::endl;
  write_to_timestamp_file(timestamper->file_writer_element, ss.str());
}

static void gst_timestamper_dispose(GObject *object) {
  GstTimeStamper *timestamper = GST_TIME_STAMPER(object);

  gst_object_unref(timestamper->file_writer_element);
  gst_element_set_state(GST_ELEMENT(timestamper->file_writer_pipeline),
                        GST_STATE_NULL);
  gst_object_unref(timestamper->file_writer_pipeline);

  G_OBJECT_CLASS(gst_timestamper_parent_class)->dispose(object);
}

static GstFlowReturn gst_timestamper_transform_ip(GstBaseTransform *vfilter,
                                                  GstBuffer *buffer) {
  GstTimeStamper *timestamper = GST_TIME_STAMPER(vfilter);
  std::stringstream ss;
  ss << std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count()
     << std::endl;
  write_to_timestamp_file(timestamper->file_writer_element, ss.str());
  return GST_FLOW_OK;
}