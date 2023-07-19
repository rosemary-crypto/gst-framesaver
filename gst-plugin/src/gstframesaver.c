#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gstframesaver.h"

#include <gst/video/video.h>
#include <gst/video/gstvideopool.h>
#include <gst/video/gstvideofilter.h>
#include <gst/video/gstvideosink.h>
//#include <gst/video/gstvideoparsers.h>
#include <gst/video/gstvideometa.h>
#include <gst/video/gstvideodecoder.h>
#include <glib.h>
#include <stdio.h>

//#include <FreeImage.h>

GST_DEBUG_CATEGORY_STATIC (gst_frame_saver_debug);
#define GST_CAT_DEFAULT gst_frame_saver_debug

#define gst_frame_saver_parent_class parent_class
G_DEFINE_TYPE (GstFrameSaver, gst_frame_saver, GST_TYPE_ELEMENT);

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE
    ("sink", GST_PAD_SINK, GST_PAD_ALWAYS, GST_STATIC_CAPS("video/x-raw"));

static void 
gst_frame_saver_finalize(GObject *object)
{
  GstFrameSaver *saver = GST_FRAME_SAVER(object);

  GST_DEBUG_OBJECT(saver, "Finalizing");

  G_OBJECT_CLASS(parent_class)->finalize(object);
}

static GstFlowReturn gst_frame_saver_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer) {
  // Get the caps of the buffer
  GstCaps *caps = gst_pad_get_current_caps(pad);
  if (caps == NULL) {
    g_print("Failed to get buffer caps\n");
    return GST_FLOW_ERROR;
  }

  // Get the first structure in the caps
  GstStructure *structure = gst_caps_get_structure(caps, 0);
  if (structure == NULL) {
    g_print("Failed to get caps structure\n");
    gst_caps_unref(caps);
    return GST_FLOW_ERROR;
  }

  // Retrieve metadata values from the structure
  const gchar *codecName = gst_structure_get_name(structure);
  gint width, height;
  gst_structure_get_int(structure, "width", &width);
  gst_structure_get_int(structure, "height", &height);

  // Print the retrieved metadata
  g_print("Codec: %s, Width: %d, Height: %d\n", codecName, width, height);

  // Clean up
  gst_caps_unref(caps);

  // Continue processing the buffer
  return GST_FLOW_OK;
}

static void
gst_frame_saver_init (GstFrameSaver * saver)
{
  saver->sinkpad = gst_pad_new_from_static_template (&sink_template, "sink");
  gst_pad_set_chain_function (saver->sinkpad,
      GST_DEBUG_FUNCPTR (gst_frame_saver_chain));
  gst_element_add_pad (GST_ELEMENT (saver), saver->sinkpad);

  saver->frameCount = 0;
}

static void
gst_frame_saver_class_init (GstFrameSaverClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;
  
  gobject_class->finalize = gst_frame_saver_finalize;

  gst_element_class_set_details_simple (gstelement_class,
      "FrameSaver",
      "FIXME:Generic",
      "FIXME:Generic Template Element", "U-DESKTOP-I8GIKDLPINTEL <<user@hostname.org>>");

}



/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT(gst_frame_saver_debug, "framesaver", 0, "Frame Saver");
  return gst_element_register (plugin, "framesaver", GST_RANK_NONE, GST_TYPE_FRAME_SAVER);
}


GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    framesaver,
    "Frame Saver Plugin",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN
    )
