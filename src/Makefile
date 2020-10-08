PACKAGES= glfw3 opengl freetype2
CFLAGS=$(shell pkg-config --cflags $(PACKAGES)) -I../yaml-cpp/include
CPPFLAGS=$(CFLAGS)
LIBS=$(shell pkg-config --libs $(PACKAGES))

CXX=g++

OBJECTS= main.o RFCIndex.o RFCDoc.o shader.o fontResLib.o fontTexture.o FontMap.o

all: rfcr *.h
rfcr:	$(OBJECTS)
	$(CXX) -std=c++17 $(CPPFLAGS) $(OBJECTS) -o rfcr $(LIBS) libyaml-cpp.a
	mv rfcr ..

clean:
	@rm -f *.o rfcr
