#ifndef __GST_VIDEOMUX_H__
#define __GST_VIDEOMUX_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_VIDEO_MUX (gst_video_mux_get_type())
G_DECLARE_FINAL_TYPE (GstVideoMux, gst_video_mux, GST, VIDEO_MUX, GstElement)

#define GST_VIDEO_MUX(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_VIDEO_MUX, GstVideoMux))
#define GST_VIDEO_MUX_CLASS(kclass) (G_TYPE_CHECK_CLASS_CAST((kclass), GST_TYPE_VIDEO_MUX, GstVideoMuxClass))
#define GST_IS_VIDEO_MUX(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_VIDEO_MUX))
#define GST_IS_VIDEO_MUX_CLASS(kclass) (G_TYPE_CHECK_CLASS_TYPE((kclass), GST_TYPE_VIDEO_MUX))

struct _GstVideoMux
{
  GstElement     element;

  GstPad        *sinkpad;
  GstPad        *srcpad;

  GList        *sinkpad_list;
  
  guint         pad_count;
  GMutex        lock;
  guint frameCount;
};

struct _GstVideoMuxClass {
  GstElementClass parent_class;
};

G_END_DECLS

#endif /* __GST_VIDEOMUX_H__ */