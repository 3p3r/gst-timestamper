# gst-timestamper

GStreamer Video TimeStamper Plugin. This plugin writes UNIX timestamps for each
video frame that passes through it into `timestamps.txt`.

The unit of timestamps is nanoseconds. `timestamps.txt` is appended every time a
new pipeline is launched. A header with `===` is written to indicate when a new
pipeline is started.

## sample timestamp format

Each line number represents the frame number plus one (plus one for the header).

```text
=== unit: ns (unix timestamps)
1540328165479649184
1540328165496287656
1540328165512931194
1540328165529621171
1540328165546266634
1540328165562920070
1540328165579584313
1540328165596233232
```

## building

```bash
$ make clean
$ make
gcc -c -fpic -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/orc-0.4 -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include plugin.c -o plugin.o
gcc -c -fpic -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/orc-0.4 -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -std=c++11 timestamper.cpp -o timestamper.o
gcc -shared plugin.o timestamper.o -o libtimestamper.so -Wl,-z,defs -lstdc++ -lgstapp-1.0 -lgstvideo-1.0 -lgstaudio-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
```

## sample pipeline

This command records raw video frames into an avi container and saves frame
timestamps into `timestamps.txt`

```bash
GST_PLUGIN_PATH=$(pwd) gst-launch-1.0 -e -vv v4l2src device=/dev/video5 ! video/x-raw,width=640,height=480,framerate=60/1 ! timestamper ! avimux ! filesink location=test.avi
```

## sample verification

This command prints the number of frames in the recorded video in the sample
pipeline above.

```bash
ffprobe -v error -count_frames -select_streams v:0 -show_entries stream=nb_read_frames -of default=nokey=1:noprint_wrappers=1 test.avi
```