#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gstframesaver.h"

#include <gst/video/video.h>
#include <gst/video/gstvideopool.h>

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define VIDEO_FPS 30

GST_DEBUG_CATEGORY_STATIC (gst_frame_saver_debug);
#define GST_CAT_DEFAULT gst_frame_saver_debug

#define gst_frame_saver_parent_class parent_class
G_DEFINE_TYPE (GstFrameSaver, gst_frame_saver, GST_TYPE_ELEMENT);

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE(
    "sink", GST_PAD_SINK, GST_PAD_REQUEST,
    GST_STATIC_CAPS("video/x-raw, format=RGB, width=" GST_VIDEO_SIZE_RANGE ", height=" GST_VIDEO_SIZE_RANGE ", framerate=" GST_VIDEO_FPS_RANGE)
);

static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE
    ("src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS("video/x-raw"));

static void gst_frame_saver_finalize(GObject *object) {
    GstFrameSaver *saver = GST_FRAME_SAVER(object);
    GST_DEBUG_OBJECT(saver, "Finalizing");

    G_OBJECT_CLASS(gst_frame_saver_parent_class)->finalize(object);
}

static GstFlowReturn gst_frame_saver_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer) {
    GstFrameSaver *saver = GST_FRAME_SAVER(parent);
    saver->frameCount++;

    // Extract video properties from the incoming buffer
    GstVideoInfo info;
    gst_video_info_init(&info);
    gst_video_info_from_caps(&info, gst_pad_get_current_caps(pad));

    gint width = GST_VIDEO_INFO_WIDTH(&info);
    gint height = GST_VIDEO_INFO_HEIGHT(&info);
    gint fps_numerator = GST_VIDEO_INFO_FPS_N(&info);
    gint fps_denominator = GST_VIDEO_INFO_FPS_D(&info);

    // Print the retrieved metadata
    g_print("Width: %d, Height: %d, FPS: %d/%d\n", width, height, fps_numerator, fps_denominator);

    // Update the capabilities of the source pad
    GstCaps *src_caps = gst_caps_new_simple("video/x-raw",
        "format", G_TYPE_STRING, "RGB",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height,
        "framerate", GST_TYPE_FRACTION, fps_numerator, fps_denominator,
        NULL);
    gst_pad_set_caps(saver->srcpad, src_caps);
    gst_caps_unref(src_caps);

    // Convert the frame to grayscale (assuming it's in RGB format)
    GstMapInfo map;
    gst_buffer_map(buffer, &map, GST_MAP_WRITE);
    guint8 *data = map.data;
    for (gsize i = 0; i < map.size; i += 3) {
        guint8 gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
        data[i] = data[i + 1] = data[i + 2] = gray;
    }
    gst_buffer_unmap(buffer, &map);

    // Push the processed buffer downstream
    return gst_pad_push(saver->srcpad, buffer);
}

static void gst_frame_saver_init(GstFrameSaver *saver) {
    saver->sinkpad = gst_pad_new_from_static_template(&sink_template, "sink");
    gst_pad_set_chain_function(saver->sinkpad, GST_DEBUG_FUNCPTR(gst_frame_saver_chain));
    gst_element_add_pad(GST_ELEMENT(saver), saver->sinkpad);
    saver->srcpad = gst_pad_new_from_static_template(&src_template, "src");
    gst_element_add_pad(GST_ELEMENT(saver), saver->srcpad);
    saver->frameCount = 0;

    // Create tee element for parallel processing
    saver->tee = gst_element_factory_make("tee", "tee");
    gst_bin_add(GST_BIN(saver), saver->tee);

    // Create queues for buffering processed frames
    for (int i = 0; i < MAX_INPUT_STREAMS; i++) {
        gchar *queue_name = g_strdup_printf("queue_%d", i);
        saver->queues[i] = gst_element_factory_make("queue", queue_name);
        g_free(queue_name);
        gst_bin_add(GST_BIN(saver), saver->queues[i]);
    }

    // Link elements
    gst_element_link_many(saver->tee, saver->queues[0], saver->srcpad, NULL);

    // Link tee pads to queues
    for (int i = 0; i < MAX_INPUT_STREAMS; i++) {
        GstPad *tee_pad = gst_element_get_request_pad(saver->tee, "src_%u");
        gchar *queue_pad_name = g_strdup_printf("sink_%d", i);
        GstPad *queue_pad = gst_element_get_static_pad(saver->queues[i], queue_pad_name);
        g_free(queue_pad_name);
        gst_pad_link(tee_pad, queue_pad);
        gst_object_unref(tee_pad);
        gst_object_unref(queue_pad);
    }
}

static void gst_frame_saver_class_init (GstFrameSaverClass * klass)
{
    GObjectClass *gobject_class;
    GstElementClass *gstelement_class;

    gobject_class = G_OBJECT_CLASS(klass);
    gstelement_class = GST_ELEMENT_CLASS(klass);
    
    gobject_class->finalize = gst_frame_saver_finalize;

    // Set the pad templates to allow video input and output
    gst_element_class_add_pad_template(gstelement_class,
        gst_static_pad_template_get(&sink_template));
    gst_element_class_add_pad_template(gstelement_class,
        gst_static_pad_template_get(&src_template));
    gst_element_class_set_details_simple (gstelement_class,
        "FrameSaver",
        "FIXME:Generic",
        "FIXME:Generic Template Element", "U-DESKTOP-I8GIKDLPINTEL <<user@hostname.org>>");
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean plugin_init(GstPlugin *plugin) {
    GST_DEBUG_CATEGORY_INIT(gst_frame_saver_debug, "framesaver", 0, "Frame Saver");
    return gst_element_register(plugin, "framesaver", GST_RANK_NONE, GST_TYPE_FRAME_SAVER);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    framesaver,
    "Frame Saver Plugin",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN
)