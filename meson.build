project('gst-template', 'c', version : '1.19.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()

api_version = '1.0'

gst_dep = dependency('gstreamer-1.0', version : '>=1.19',
    required : true, fallback : ['gstreamer', 'gst_dep'])
gstbase_dep = dependency('gstreamer-base-1.0', version : '>=1.19',
    fallback : ['gstreamer', 'gst_base_dep'])
gdkpixbuf_dep = dependency('gdk-pixbuf-2.0', version : '>=2.40', required : false)
#gstframesaver_dep = dependency('freeimage')
gstvideo_dep = dependency('gstreamer-video-1.0', version: '>=1.19', required : true)
glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
#jpeg_dep = dependency('libjpeg')


subdir('gst-plugin')