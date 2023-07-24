gst-launch-1.0 -v videotestsrc pattern=snow ! videoconvert ! framesaver name=saver1 ^
    videotestsrc pattern=ball ! videoconvert ! framesaver name=saver2 ^
   saver1.src ! videoconvert ! autovideosink ^
   saver2.src ! videoconvert ! autovideosink