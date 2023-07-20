#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gstframesaver.h"
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
// GST_ELEMENT_REGISTER_DEFINE (framesaver, "framesaver", GST_RANK_PRIMARY, GST_TYPE_FRAME_SAVER);

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink", GST_PAD_SINK, GST_PAD_REQUEST, GST_STATIC_CAPS("video/x-raw"));
static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);


static void 
gst_frame_saver_finalize(GObject *object)
{
  GstFrameSaver *saver = GST_FRAME_SAVER(object);

  GST_DEBUG_OBJECT(saver, "Finalizing");

  G_OBJECT_CLASS(parent_class)->finalize(object);
}

// static GstFlowReturn gst_frame_saver_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer) {

//   // Get the caps of the buffer
//   GstCaps *caps = gst_pad_get_current_caps(pad);
//   if (caps == NULL) {
//     g_print("Failed to get buffer caps\n");
//     return GST_FLOW_ERROR;
//   }

//   // Get the first structure in the caps
//   GstStructure *structure = gst_caps_get_structure(caps, 0);
//   if (structure == NULL) {
//     g_print("Failed to get caps structure\n");
//     gst_caps_unref(caps);
//     return GST_FLOW_ERROR;
//   }

//   // Retrieve metadata values from the structure
//   const gchar *codecName = gst_structure_get_name(structure);
//   gint width, height;
//   gst_structure_get_int(structure, "width", &width);
//   gst_structure_get_int(structure, "height", &height);

//   // Print the retrieved metadata
//   g_print("Codec: %s, Width: %d, Height: %d\n", codecName, width, height);

//   filename = g_strdup_printf("frame-%05d.png", saver->frameCount);
  
//   if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
//     g_file_set_contents(filename, (gchar *) map.data, map.size, NULL);
//     gst_buffer_unmap(buffer, &map);
//   } else {
//     GST_WARNING_OBJECT(saver, "Failed to map buffer");
//   }

//   g_free(filename);

//   timestamp = GST_BUFFER_TIMESTAMP(buffer);
//   GST_DEBUG_OBJECT(saver, "Saved frame %d with timestamp %" GST_TIME_FORMAT, saver->frameCount, GST_TIME_ARGS(timestamp));
//   // Clean up
//   gst_caps_unref(caps);

//   // Continue processing the buffer
//   return GST_FLOW_OK;
// }


static GstFlowReturn gst_frame_saver_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer) {
  g_print("============ Chain function ===========");
  GstFrameSaver *saver = GST_FRAME_SAVER(parent);
  GstFlowReturn ret;
  GstCaps *caps = gst_pad_get_current_caps(pad);
  GstMapInfo map;
  gchar *filename;

  if (caps) {
    // Assuming there is only one structure in the caps, we can get its format
    GstStructure *structure = gst_caps_get_structure(caps, 0);
    const gchar *format = gst_structure_get_name(structure);

    // Check if the format is supported for frame extraction (e.g., RGB, YUV)
  //   if (g_strcmp0(format, "video/x-raw") == 0) {
  //     gint width, height, stride;
  //     gst_structure_get_int(structure, "width", &width);
  //     gst_structure_get_int(structure, "height", &height);
  //     gst_structure_get_int(structure, "stride", &stride);

  //     gint frame_size = height * stride; // Assuming no padding
  //     gint buffer_size = gst_buffer_get_size(buffer);
  //     if (buffer_size >= frame_size) {
  //       gconstpointer buffer_data = gst_buffer_get_data(buffer);
  //       gint num_frames = buffer_size / frame_size;
  //       g_print("Num frames > ", num_frames);
  //       for (gint i = 0; i < num_frames; i++) {
  //         gint frame_offset = i * frame_size;
  //         // gconstpointer frame_data = buffer_data + frame_offset;
  //         // GstBuffer *frame_buffer = gst_buffer_copy_region(buffer, GST_BUFFER_COPY_ALL, frame_offset, frame_size);
  //         // filename = g_strdup_printf("frame-%06d.jpg", i);
  //         // if (gst_buffer_map(frame_buffer, &map, GST_MAP_READ)) {
  //         //   g_file_set_contents(filename, (gchar *) map.data, map.size, NULL);
  //         //   gst_buffer_unmap(frame_buffer, &map);
  //         // } else {
  //         //   GST_WARNING_OBJECT(saver, "Failed to map buffer");
  //         // }

  //         // gst_buffer_unref(frame_buffer);
  //       }
  //       // gst_buffer_remove_range(buffer, 0, frame_size * num_frames);
  //     }
  //   }

  //   gst_caps_unref(caps);
  // }


    //   if (g_strcmp0(format, "video/x-raw") == 0) {
    //     gint width, height, stride;
    //     gst_structure_get_int(structure, "width", &width);
    //     gst_structure_get_int(structure, "height", &height);
    //     gst_structure_get_int(structure, "stride", &stride);

    //     GstVideoFrame frame;
    //     if (gst_video_frame_map(&frame, gst_video_info_new(), buffer, GST_MAP_READ)) {
    //         // 'frame' now represents the video frame data
    //         // You can access the video frame properties like width, height, stride, etc.
    //         guint8 *data = GST_VIDEO_FRAME_PLANE_DATA(&frame, 0);
    //         guint stride = GST_VIDEO_FRAME_PLANE_STRIDE(&frame, 0);
    //         // ...

    //         // Unmap the frame when done using it
    //         gst_video_frame_unmap(&frame);
    //     } else {
    //         // Error handling if mapping fails
    //         g_print("Failed to map buffer to video frame.\n");
    //     }
    //   } else {
    //       // Unsupported video format
    //       g_print("Unsupported video format: %s\n", format);
    //   }
    // }


    if (g_strcmp0(format, "video/x-raw") == 0) {
      gint width, height, stride;
      gst_structure_get_int(structure, "width", &width);
      gst_structure_get_int(structure, "height", &height);
      gst_structure_get_int(structure, "stride", &stride);
      g_print("Width %d \t Height %d \t stride %d\n", width, height, stride);
      gint frame_size = height * width; // Assuming no padding
      gint buffer_size = gst_buffer_get_size(buffer);
      g_print("frame size %d \n", frame_size);
      g_print("Buffer size %d \n", buffer_size);
      if (buffer_size >= frame_size) {
        g_print("Buffer size is larger than frame_size\n");

        // gconstpointer buffer_data = gst_buffer_get_data(buffer);
        gint num_frames = buffer_size / frame_size;
        g_print("Number of frames %d \n", num_frames);
        for (gint i = 0; i < num_frames; i++) {
          gint frame_offset = i * frame_size;
          filename = g_strdup_printf("frame-%06d.png", i);
          GstBuffer *frame_buffer = gst_buffer_copy_region(buffer, GST_BUFFER_COPY_ALL, frame_offset, frame_size);
          if (gst_buffer_map(frame_buffer, &map, GST_MAP_READ)) {
            g_print("Map frame buffer into map\n");
            g_file_set_contents(filename, (gchar *) map.data, map.size, NULL);
            gst_buffer_unmap(frame_buffer, &map);
          } else {
            GST_WARNING_OBJECT(saver, "Failed to map buffer");
          }
          g_print("push frame buffer from source ");
          ret = gst_pad_push (saver->srcpad, frame_buffer);
          gst_buffer_unref(frame_buffer);
        }
        // gst_buffer_remove_range(buffer, 0, frame_size * num_frames);
      }else{
        g_print("Buffer size is smaller than frame size ******\n");
        g_print("frame size %d \n", frame_size);
        g_print("Buffer size %d \n", buffer_size);
        ret = gst_pad_push (saver->srcpad, buffer);

      }
    }
  }
  gst_caps_unref(caps);
  ret = gst_pad_push (saver->srcpad, buffer);
  return ret;
}


static GstPad * gst_frame_saver_request_new_pad(GstElement *element, GstPadTemplate *templ, const gchar *name, const GstCaps *caps){
  g_print("========== A new sink pad is requested ============");
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
  g_print(">>>>>>>> Instance Init");
  saver->srcpad = gst_pad_new_from_static_template (&src_template, "src");
  gst_element_add_pad (GST_ELEMENT (saver), saver->srcpad);

  saver->frameCount = 0;
}

static void
gst_frame_saver_class_init (GstFrameSaverClass * klass)
{
  g_print(">>>>>>> Class Init ");
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
  return gst_element_register (plugin, "framesaver", GST_RANK_NONE, GST_TYPE_FRAME_SAVER);
}


GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    framesaver,
    "Frame Saver Plugin",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN
    )
