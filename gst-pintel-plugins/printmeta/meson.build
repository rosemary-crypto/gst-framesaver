gstprintmeta_sources = [
  'gstprintmeta.c',
]

gstsharedmetadata_dep = dependency('gstsharedmetadata', version: '>=1.0.0', required: true)

gstprintmeta = library('gstprintmeta',
  gstprintmeta_sources,
  #include_directories: include_directories('.','../gst-metadata','../'), 
  include_directories: [gst_metadata_inc, configinc, include_directories('.')],
  c_args: plugin_c_args,
  dependencies: [gdkpixbuf_dep, gst_dep, gstbase_dep, gstvideo_dep, glib_dep, gobject_dep, gstapp_dep, gstsharedmetadata_dep],
  install: true,
  install_dir: plugins_install_dir,
)
