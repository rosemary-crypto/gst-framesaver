#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gstprintmeta.h"
#include "gstsharedmetadata.h"
#include <gst/gstbuffer.h>
#include <gst/video/video.h>
#include <gst/video/gstvideopool.h>
#include <gst/video/gstvideofilter.h>
#include <gst/video/gstvideosink.h>

#include <gst/video/gstvideometa.h>
#include <gst/video/gstvideodecoder.h>
#include <glib.h>
#include <stdio.h>

//#include <FreeImage.h>

GST_DEBUG_CATEGORY_STATIC (gst_print_meta_debug);
#define GST_CAT_DEFAULT gst_print_meta_debug

#define gst_print_meta_parent_class parent_class
#define FORMATS " { AYUV, VUYA, BGRA, ARGB, RGBA, ABGR, Y444, Y42B, YUY2, UYVY, "\
                "   YVYU, I420, YV12, NV12, NV21, Y41B, RGB, BGR, xRGB, xBGR, "\
                "   RGBx, BGRx } "
G_DEFINE_TYPE (GstPrintMeta, gst_print_meta, GST_TYPE_ELEMENT);

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink", GST_PAD_SINK, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);
static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);



// Define the printmeta element's metadata query function
static gboolean gst_print_meta_sink_event(GstPad *pad, GstObject *parent, GstEvent *event) {
    GstPrintMeta *printmeta = GST_PRINT_META(parent);

    switch (GST_EVENT_TYPE(event)) {
        case GST_EVENT_CAPS: {
            GstCaps *caps;
            gst_event_parse_caps(event, &caps);
            // Do something with the caps if needed
            break;
        }
        case GST_EVENT_EOS:
            // Handle end-of-stream event if needed
            break;
        default:
            break;
    }

    return gst_pad_event_default(pad, parent, event);
}

static void 
gst_print_meta_finalize(GObject *object)
{
  GstPrintMeta *printmeta = GST_PRINT_META(object);

  GST_DEBUG_OBJECT(printmeta, "Finalizing");

  G_OBJECT_CLASS(parent_class)->finalize(object);
}

static GstFlowReturn gst_print_meta_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer) {
 
  GstPrintMeta *printmeta = GST_PRINT_META(parent);
  // Print the received buffer details
  g_print("\nPrintMeta: Received buffer on pad: %s\n", gst_pad_get_name(pad));
  g_print("\tPrintMeta: BUFFER\n\tPTS : %f\n", GST_BUFFER_PTS(buffer));
  g_print("\tPrintMeta: Timestamp : %f\n", GST_BUFFER_TIMESTAMP(buffer));
  g_print("\tPrintMeta: Duration : %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(GST_BUFFER_DURATION(buffer)));
      
  // Check if the buffer has the custom metadata attached
  const gchar * impl = "GstStreamIdMeta";
  const GstMetaInfo *info = gst_meta_get_info(impl);
  GType type = info->api; 
  GstStreamIdMeta *stream_id_meta =  (GstStreamIdMeta*) gst_buffer_get_meta(buffer, type);
  if (stream_id_meta) {
      // Print the stream_id from the custom metadata
    g_print("PrintMeta: Stream ID from videomux: %s\n", stream_id_meta->stream_id);
  } else {
    g_print("PrintMeta: Custom metadata (stream_id) not found in the buffer.\n");
  }

  /* just push out the incoming buffer without touching it */
  return gst_pad_push(printmeta->srcpad, buffer);
}

static void
gst_print_meta_init (GstPrintMeta * printmeta)
{
    printmeta->sinkpad = gst_pad_new_from_static_template (&sink_template, "sink");
    GST_PAD_SET_PROXY_CAPS (printmeta->sinkpad);
    gst_element_add_pad (GST_ELEMENT (printmeta), printmeta->sinkpad);

    gst_pad_set_event_function(printmeta->sinkpad, gst_print_meta_sink_event);
    gst_pad_set_chain_function (printmeta->sinkpad,
        GST_DEBUG_FUNCPTR (gst_print_meta_chain));

    printmeta->srcpad = gst_pad_new_from_static_template (&src_template, "src");
    GST_PAD_SET_PROXY_CAPS (printmeta->srcpad);
    gst_element_add_pad (GST_ELEMENT (printmeta), printmeta->srcpad);
}

static void
gst_print_meta_class_init (GstPrintMetaClass * klass)
{
  
  GObjectClass *gobject_class = (GObjectClass *) klass;
  GstElementClass *gstelement_class = (GstElementClass *) klass;

  gobject_class->finalize = gst_print_meta_finalize;
  
  gst_element_class_add_static_pad_template (gstelement_class, &sink_template);
  gst_element_class_add_static_pad_template (gstelement_class, &src_template);
  
  gst_element_class_set_details_simple (gstelement_class,
      "PrintMeta",
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
  GST_DEBUG_CATEGORY_INIT(gst_print_meta_debug, "printmeta", 0, "Print Meta");
  g_print("\nPrintMETA: Plugin Register");
  // gst_stream_id_meta_api_get_type();
  return gst_element_register (plugin, "printmeta", GST_RANK_NONE, GST_TYPE_PRINT_META);
}


GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    printmeta,
    "Print Meta Plugin",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN
    )