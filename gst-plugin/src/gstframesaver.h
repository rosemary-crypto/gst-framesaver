#ifndef __GST_FRAMESAVER_H__
#define __GST_FRAMESAVER_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_FRAME_SAVER (gst_frame_saver_get_type())

#define GST_FRAME_SAVER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_FRAME_SAVER, GstFrameSaver))
#define GST_FRAME_SAVER_CLASS(kclass) (G_TYPE_CHECK_CLASS_CAST((kclass), GST_TYPE_FRAME_SAVER, GstFrameSaverClass))
#define GST_IS_FRAME_SAVER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_FRAME_SAVER))
#define GST_IS_FRAME_SAVER_CLASS(kclass) (G_TYPE_CHECK_CLASS_TYPE((kclass), GST_TYPE_FRAME_SAVER))

G_DECLARE_FINAL_TYPE (GstFrameSaver, gst_frame_saver, GST, FRAME_SAVER, GstElement)

struct _GstFrameSaver
{
  GstElement     element;

  GstPad        *sinkpad;
  // GstPad        *srcpad;

  GSList        *sinkpad_list;
  
  guint         pad_count;
  GMutex        lock;
  guint frameCount;
};

struct _GstFrameSaverClass {
  GstElementClass parent_class;
};

GST_ELEMENT_REGISTER_DECLARE(framesaver)

G_END_DECLS

#endif /* __GST_FRAMESAVER_H__ */
