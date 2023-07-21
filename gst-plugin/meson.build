plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-template-plugin')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer template Plug-ins')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cdata)

gstaudio_dep = dependency('gstreamer-audio-1.0',
    fallback: ['gst-plugins-base', 'audio_dep'])

 gstframesaver_sources = [
  'src/gstframesaver.c',
  ]

#jpeg_include_dir = jpeg_dep.get_pkgconfig_variable('includedir')

gstframesaverexample = library('gstframesaver',
  gstframesaver_sources,
  c_args: plugin_c_args,
  #include_directories: jpeg_include_dir,
  dependencies : [gdkpixbuf_dep, gst_dep, gstbase_dep, gstvideo_dep, glib_dep, gobject_dep, gstapp_dep],
  install : true,
  install_dir : plugins_install_dir,
)
