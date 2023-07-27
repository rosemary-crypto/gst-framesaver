gstsharedmetadata_sources = [
  'gstsharedmetadata.c',
]

gstsharedmetadata_headers = [
  'gstsharedmetadata.h',
]

install_headers(gstsharedmetadata_headers)


gstsharedmetadata = static_library('gstsharedmetadata',
  gstsharedmetadata_sources,
  include_directories: [configinc],
  c_args: [plugin_c_args,'/Zi'],
  dependencies: [
    gst_dep
  ],
  install: true,
  install_dir : plugins_install_dir,
)