project('gst-template', 'c', version : '1.19.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()

api_version = '1.0'

plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-template-plugin')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer template Plug-ins')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cdata)

 # Add this line to include the path to gst-metadata directory


gst_dep = dependency('gstreamer-1.0', version : '>=1.19',
    required : true, fallback : ['gstreamer', 'gst_dep'])
gstbase_dep = dependency('gstreamer-base-1.0', version : '>=1.19',
    fallback : ['gstreamer', 'gst_base_dep'])
gdkpixbuf_dep = dependency('gdk-pixbuf-2.0', version : '>=2.40', required : false)
#gstframesaver_dep = dependency('freeimage')
gstvideo_dep = dependency('gstreamer-video-1.0', version: '>=1.19', required : true)
glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gstapp_dep = dependency('gstreamer-app-1.0', 
    fallback : ['gst-plugins-base', 'app_dep'])
#jpeg_dep = dependency('libjpeg', required : true)
gstaudio_dep = dependency('gstreamer-audio-1.0',
    fallback: ['gst-plugins-base', 'audio_dep'])

include_directories('gst-metadata') 
subdir('gst-metadata')
include_directories('gst-plugin')
subdir('gst-plugin')