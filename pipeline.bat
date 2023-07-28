REM WORKING
REM gst-launch-1.0 ^
    REM filesrc location=vid1.mp4 ! decodebin name=d1 ^
    REM filesrc location=vid3.mp4 ! decodebin name=d2 ^
    REM d1. ! video/x-raw ! queue ! videomux name=mux ^
    REM d2. ! video/x-raw ! queue ! mux. ^
    REM mux.src ! printmeta ! fakesink sync=false
	

gst-launch-1.0 ^
    filesrc location=vid1.mp4 ! decodebin name=d1 ^
    filesrc location=vid3.mp4 ! decodebin name=d2 ^
    d1. ! video/x-raw ! queue ! videomux name=mux ^
    d2. ! video/x-raw ! queue ! mux. ^
    mux.src ! printmeta ! autovideosink