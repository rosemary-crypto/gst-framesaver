gst-launch-1.0 ^
	videomux name=saver ! videoconvert ! autovideosink ^
	filesrc location=vid1.mp4 ! decodebin name=d1 ^
	filesrc location=vid3.mp4 ! decodebin name=d2 ^
	d1. ! video/x-raw ! queue ! saver. ^
	d2. ! video/x-raw ! queue ! saver.