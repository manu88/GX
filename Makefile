CC = g++

#OBJS=src/main.o src/piGL.o src/GXContext.o src/DisplayDispMan.o src/GXRenderer.o src/GXLayer.o src/nanovg/nanovg.o src/nanovg/nanovg_gl.o
#BIN=test.bin


CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE 
CFLAGS+=-D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe
CFLAGS+=-DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi

CFLAGS+=-DUSE_DISPMAN -DNANOVG_GLES2_IMPLEMENTATION  -DUSE_UNIFORMBUFFER

LDFLAGS+=-L$(SDKSTAGE)/opt/vc/lib/ -lbrcmGLESv2 -lbrcmEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread 
LDFLAGS+=-lrt -lm -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont
LDFLAGS+= -lGLEW -shared

INCLUDES+=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux 
INCLUDES+=-I./ -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont

INCLUDES+= -Isrc/ -Isrc/nanovg/ -Iinclude/

CXXSOURCES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
CSOURCES =$(wildcard src/*.c) $(wildcard src/nanovg/*.c)

CSOURCES += src/Display/piGL.c src/Display/DisplayDispMan.c

COBJECTS=$(CSOURCES:.c=.o)
CXXOBJECTS=$(CXXSOURCES:.cpp=.o)


TEST_SOURCES = $(wildcard Tests/*.cpp)

EXECUTABLE= libGX.so
TEST = TestGX


all: $(CSOURCES) $(CXXSOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(COBJECTS) $(CXXOBJECTS)
	$(CXX)  -o $@ $(CXXOBJECTS)  $(COBJECTS)  $(LDFLAGS)

%.o: %.c
	@rm -f $@
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

%.o: %.cpp
	@rm -f $@
	$(CXX) -std=c++11 $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

%.bin: $(OBJS)
	$(CC) -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic

%.a: $(OBJS)
	$(AR) r $@ $^

test:
	$(CXX) -std=c++11 -DUSE_DISPMAN -L. -L/opt/vc/lib/ -lGX -lbrcmGLESv2 -Isrc/  -Iinclude/ -I/usr/local/include/GroundBase -lGroundBase-cpp -I/opt/vc/include/  $(TEST_SOURCES) -o $(TEST)

clean:
	rm -f $(COBJECTS)
	rm -f $(CXXOBJECTS)
	rm  $(EXECUTABLE)


install:
	cp $(EXECUTABLE) /usr/local/lib
	mkdir -p /usr/local/include/GX
	cp include/*.h* /usr/local/include/GX/

uninstall:
	rm -f /usr/local/lib/$(EXECUTABLE)
