PACKAGES= glfw3 opengl freetype2
CFLAGS=$(shell pkg-config --cflags $(PACKAGES))
CPPFLAGS=$(CFLAGS)
LIBS=$(shell pkg-config --libs $(PACKAGES))

CXX=g++

OBJECTS= main.o rfcindex.o rfcdocument.o shader.o fontResLib.o fontTexture.o

all: rfcr *.h
rfcr:	$(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o rfcr $(LIBS)

clean:
	@rm -f *.o rfcr
