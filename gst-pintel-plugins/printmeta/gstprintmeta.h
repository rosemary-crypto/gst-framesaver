#ifndef __GST_PRINTMETA_H__
#define __GST_PRINTMETA_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_PRINT_META (gst_print_meta_get_type())
G_DECLARE_FINAL_TYPE (GstPrintMeta, gst_print_meta, GST, PRINT_META, GstElement)

#define GST_PRINT_META(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_PRINT_META, GstPrintMeta))
#define GST_PRINT_META_CLASS(kclass) (G_TYPE_CHECK_CLASS_CAST((kclass), GST_TYPE_PRINT_META, GstPrintMetaClass))
#define GST_IS_PRINT_META(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_PRINT_META))
#define GST_IS_PRINT_META_CLASS(kclass) (G_TYPE_CHECK_CLASS_TYPE((kclass), GST_TYPE_PRINT_META))

struct _GstPrintMeta
{
  GstElement     element;

  GstPad        *sinkpad;
  GstPad        *srcpad;
};

struct _GstPrintMetaClass {
  GstElementClass parent_class;
};

G_END_DECLS

#endif /* __GST_PRINTMETA_H__ */