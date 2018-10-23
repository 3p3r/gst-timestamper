#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define PACKAGE "vl-plugins"
#endif

#include "timestamper.h"

static gboolean
plugin_init(GstPlugin *plugin)
{
  gboolean ret;

  ret = gst_element_register(plugin, "timestamper", GST_RANK_NONE,
                             GST_TYPE_TIME_STAMPER);

  return ret;
}

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  timestamper,
                  "Buffer Timestamp Recorder",
                  plugin_init,
                  "0.0.0",
                  GST_LICENSE_UNKNOWN,
                  "VirtuLabs",
                  "http://virtulabs.com/");
