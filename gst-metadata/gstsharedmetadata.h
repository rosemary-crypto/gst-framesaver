#ifndef __GST_SHARED_METADATA_H__
#define __GST_SHARED_METADATA_H__

#include <gst/gst.h>
#include <gst/gstbuffer.h>

G_BEGIN_DECLS

GType gst_stream_id_meta_api_get_type(void);
#define GST_STREAM_ID_META_API_TYPE (gst_stream_id_meta_api_get_type())

//GstMeta* gst_buffer_get_stream_id_meta(GstBuffer *b);

const GstMetaInfo *gst_stream_id_meta_get_info(void);
#define GST_STREAM_ID_META_GET_INFO (gst_stream_id_meta_get_info())

#define gst_buffer_get_stream_id_meta(b) ((GstStreamIdMeta*)gst_buffer_get_meta((b), GST_STREAM_ID_META_API_TYPE))

gboolean gst_stream_id_meta_init(GstMeta *meta, gpointer params, GstBuffer *buffer);
gboolean gst_stream_id_meta_transform(GstBuffer *transbuf, GstMeta *meta, GstBuffer *buffer, GQuark type, gpointer data);
void gst_stream_id_meta_free(GstMeta *meta, GstBuffer *buffer);

typedef struct _GstStreamIdMeta GstStreamIdMeta;

struct _GstStreamIdMeta {
  GstMeta meta;
  //gint stream_id;
  gchar *stream_id;
};

GstStreamIdMeta* gst_buffer_add_stream_id_meta(GstBuffer *buffer, gchar * id);

G_END_DECLS

#endif /* __GST_SHARED_METADATA_H__ */
