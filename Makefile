CFLAGS = -fpic $(shell pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0  gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-audio-1.0)
LDFLAGS = -Wl,-z,defs -lstdc++ $(shell pkg-config --libs gstreamer-1.0 gstreamer-app-1.0  gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-audio-1.0)

all:
	  gcc -c $(CFLAGS) plugin.c -o plugin.o
	  gcc -c $(CFLAGS) -std=c++11 timestamper.cpp -o timestamper.o
		gcc -shared plugin.o timestamper.o -o libtimestamper.so $(LDFLAGS)

clean:
		rm plugin.o timestamper.o libtimestamper.so