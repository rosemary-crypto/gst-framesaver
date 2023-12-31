#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gstsharedmetadata.h"
#include <gst/gst.h>
#include <gst/gstbuffer.h>
#include <gst/video/video.h>
#include <gst/video/gstvideopool.h>
#include <gst/video/gstvideofilter.h>
#include <gst/video/gstvideosink.h>

#include <gst/video/gstvideometa.h>
#include <gst/video/gstvideodecoder.h>
#include <glib.h>
#include <stdio.h>


GType 
gst_stream_id_meta_api_get_type (void)
{
    static GType type;
    static const gchar *tags[] = { "id", NULL };
    
    if (g_once_init_enter (&type)) {
        GType _type = gst_meta_api_type_register ("GstStreamIdMetaAPI", tags);
        g_once_init_leave (&type, _type);
    }

    return type;
}

gboolean
gst_stream_id_meta_init (GstMeta * meta, gpointer params, GstBuffer * buffer)
{
    GstStreamIdMeta *emeta = (GstStreamIdMeta*) meta;
    //emeta->stream_id = 109;
    emeta->stream_id = NULL;//For string

    return TRUE;
}

gboolean
gst_stream_id_meta_transform (GstBuffer *transbuf, GstMeta *meta, GstBuffer *buffer, GQuark type, gpointer data) 
{
    GstStreamIdMeta * smeta = (GstStreamIdMeta *) meta;
    GstStreamIdMeta * dmeta = gst_buffer_get_stream_id_meta(transbuf);
    if(dmeta){
        dmeta->stream_id = g_strdup (smeta->stream_id);
    }
    else{
        dmeta = gst_buffer_add_stream_id_meta (transbuf, smeta->stream_id);
    }
    return TRUE;
}

void
gst_stream_id_meta_free (GstMeta * meta, GstBuffer * buffer){
    GstStreamIdMeta * emeta = (GstStreamIdMeta *) meta;

    g_free (emeta->stream_id); //if string
    emeta->stream_id = NULL; //if string
}

const GstMetaInfo *
gst_stream_id_meta_get_info (void) {
    static const GstMetaInfo *meta_info = NULL;

    if (g_once_init_enter ((GstMetaInfo **) & meta_info)) {
        const GstMetaInfo *meta =
            gst_meta_register (GST_STREAM_ID_META_API_TYPE, "GstStreamIdMeta", sizeof (GstStreamIdMeta),
                            (GstMetaInitFunction) gst_stream_id_meta_init, (GstMetaFreeFunction) gst_stream_id_meta_free, (GstMetaTransformFunction) gst_stream_id_meta_transform);
        g_once_init_leave ((GstMetaInfo **) & meta_info, (GstMetaInfo *) meta);
    } 
    return meta_info;
}

GstStreamIdMeta *gst_buffer_add_stream_id_meta(GstBuffer *buffer, gchar *id)
{
    GstStreamIdMeta * meta;

    g_return_val_if_fail ( GST_IS_BUFFER ( buffer ), NULL );
    
    meta = ( GstStreamIdMeta* ) gst_buffer_add_meta ( buffer ,
            GST_STREAM_ID_META_GET_INFO , NULL );
    
    //meta->stream_id = id ;
    meta -> stream_id = g_strdup ( id ); //for string
    
    return meta ;
}