#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gstframesaver.h"
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

GST_DEBUG_CATEGORY_STATIC (gst_frame_saver_debug);
#define GST_CAT_DEFAULT gst_frame_saver_debug

#define gst_frame_saver_parent_class parent_class
#define FORMATS " { AYUV, VUYA, BGRA, ARGB, RGBA, ABGR, Y444, Y42B, YUY2, UYVY, "\
                "   YVYU, I420, YV12, NV12, NV21, Y41B, RGB, BGR, xRGB, xBGR, "\
                "   RGBx, BGRx } "
G_DEFINE_TYPE (GstFrameSaver, gst_frame_saver, GST_TYPE_ELEMENT);

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink", GST_PAD_SINK, GST_PAD_REQUEST, GST_STATIC_CAPS_ANY);
static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);

static void 
gst_frame_saver_finalize(GObject *object)
{
  GstFrameSaver *saver = GST_FRAME_SAVER(object);

  GST_DEBUG_OBJECT(saver, "Finalizing");

  G_OBJECT_CLASS(parent_class)->finalize(object);
}

static GstFlowReturn gst_frame_saver_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer) {
  g_print("\nCHAIN FUNCTION\n");
  GstFrameSaver *saver = GST_FRAME_SAVER(parent);
  GstFlowReturn ret;
  GstCaps *caps = gst_pad_get_current_caps(pad);
  GstMapInfo map;
  gchar *filename;

  if (caps) {
    // Assuming there is only one structure in the caps, we can get its format
    GstStructure *structure = gst_caps_get_structure(caps, 0);
    const gchar *format = gst_structure_get_name(structure);

    if (g_strcmp0(format, "video/x-raw") == 0) {
      gint width, height, stride;
      gst_structure_get_int(structure, "width", &width);
      gst_structure_get_int(structure, "height", &height);
      gst_structure_get_int(structure, "stride", &stride);

      //gint frame_size = height * width; // Assuming no padding
      //gint buffer_size = gst_buffer_get_size(buffer);
      
      //gint frame_offset = 0;

      //GstBuffer *frame_buffer = gst_buffer_copy_region(buffer, GST_BUFFER_COPY_ALL, frame_offset, frame_size);
      const gchar* pad_name = gst_pad_get_name(pad);

      g_print("Received buffer on pad: %s\n", pad_name);
      //GstStreamIdMeta *stream_id_meta = (GstStreamIdMeta *)gst_buffer_add_meta(buffer, GST_STREAM_ID_META_GET_INFO, NULL);
      GstStreamIdMeta *stream_id_meta = (GstStreamIdMeta *)gst_buffer_get_meta(buffer, GST_STREAM_ID_META_API_TYPE);
      stream_id_meta = gst_buffer_add_stream_id_meta (buffer, 1);
      if (!stream_id_meta)
        return GST_FLOW_ERROR;
      g_print("\nAfter stream_id , id now = %d\n", stream_id_meta->stream_id);
      g_print("\n\nHERE\n\n");
      
      g_print("\nBefore pad push");
      ret = gst_pad_push (saver->srcpad, buffer);
      g_print("\nAfter pad push");
      
      gst_caps_unref(caps);
      return ret;
    }
  }
  gst_caps_unref(caps);
  //ret = gst_pad_push (saver->srcpad, buffer);
  return GST_FLOW_NOT_SUPPORTED;
}


static GstPad * gst_frame_saver_request_new_pad(GstElement *element, GstPadTemplate *templ, const gchar *name, const GstCaps *caps){
  
  GstFrameSaver *saver = GST_FRAME_SAVER(element);
  GstPad *newpad;
  GstElementClass *kclass;

  saver->pad_count++;
  gchar *pad_name = g_strdup_printf("sink_%u", saver->pad_count);
  
  // newpad = (GstPad)* GST_ELEMENT_CLASS(parent_class)->request_new_pad(element, templ, pad_name, caps);
  newpad = gst_pad_new_from_template (templ, pad_name);
  g_free(pad_name);

  /*
  - new pad => chain function
  - new pad => event function
  - new pad => query function
  - new pad => flag_proxy_caps
  - new pad => flag_proxy_allocation

  */
 gst_pad_set_chain_function(newpad, GST_DEBUG_FUNCPTR (gst_frame_saver_chain));
 GST_OBJECT_FLAG_SET(newpad, GST_PAD_FLAG_PROXY_CAPS);
 GST_OBJECT_FLAG_SET(newpad, GST_PAD_FLAG_PROXY_ALLOCATION);

 gst_pad_set_active(newpad, TRUE);
 saver->sinkpad_list = g_list_append(saver->sinkpad_list, gst_object_ref(newpad));
 gst_element_add_pad(element, newpad);

 return newpad;

}

static void
gst_frame_saver_init (GstFrameSaver * saver)
{
  
  saver->srcpad = gst_pad_new_from_static_template (&src_template, "src");
  gst_element_add_pad (GST_ELEMENT (saver), saver->srcpad);

  saver->frameCount = 0;
}

static void
gst_frame_saver_class_init (GstFrameSaverClass * klass)
{
  
  GObjectClass *gobject_class = (GObjectClass *) klass;
  GstElementClass *gstelement_class = (GstElementClass *) klass;

  gobject_class->finalize = gst_frame_saver_finalize;
  
  gst_element_class_add_static_pad_template (gstelement_class, &sink_template);
  gst_element_class_add_static_pad_template (gstelement_class, &src_template);
  
  gstelement_class->request_new_pad = GST_DEBUG_FUNCPTR (gst_frame_saver_request_new_pad);
  
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
  g_print("\nPlugin REgister");
  // gst_stream_id_meta_api_get_type();
  return gst_element_register (plugin, "framesaver", GST_RANK_NONE, GST_TYPE_FRAME_SAVER);
}


GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    framesaver,
    "Frame Saver Plugin",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN
    )