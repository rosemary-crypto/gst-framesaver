#ifndef __GST_FRAMESAVER_H__
#define __GST_FRAMESAVER_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_FRAME_SAVER (gst_frame_saver_get_type())
G_DECLARE_FINAL_TYPE (GstFrameSaver, gst_frame_saver, GST, FRAME_SAVER, GstElement)

#define MAX_INPUT_STREAMS 4 // Just considering this as the max number of input streams for now

struct _GstFrameSaver
{
    GstElement element;

    GstPad *sinkpad;
    GstPad *srcpad;

    guint frameCount;

    GstElement *tee;
    GstElement *queues[MAX_INPUT_STREAMS];
};

struct _GstFrameSaverClass {
    GstElementClass parent_class;
};


G_END_DECLS

#endif /* __GST_FRAMESAVER_H__ */
