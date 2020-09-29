PACKAGES= glfw3 opengl freetype2
CFLAGS=$(shell pkg-config --cflags $(PACKAGES)) -Iyaml-cpp/include
CPPFLAGS=$(CFLAGS)
LIBS=$(shell pkg-config --libs $(PACKAGES))

CXX=g++

OBJECTS= main.o RFCIndex.o RFCDoc.o shader.o fontResLib.o fontTexture.o

all: rfcr *.h
rfcr:	$(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o rfcr $(LIBS) libyaml-cpp.a

clean:
	@rm -f *.o rfcr
