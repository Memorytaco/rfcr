PACKAGES = glfw3 opengl freetype2
Optmise = -O3
USERFLAGS = $(Optmise) $(shell pkg-config --cflags $(PACKAGES)) -I../yaml-cpp/include
LIBS = $(shell pkg-config --libs $(PACKAGES))
CFLAGS = $(CXXFLAGS) $(USERFLAGS)
CPPFLAGS = -fPIC -fPIE $(USERFLAGS)

OBJECTS= main.o RFCIndex.o RFCDoc.o shader.o fontResLib.o fontTexture.o Window.o FontMap.o

all: rfcr *.h
rfcr:	$(OBJECTS)
	$(CXX) -std=c++11 $(USERFLAGS) $(OBJECTS) $(LIBS) libyaml-cpp.a -o rfcr
	mv rfcr ..

clean:
	@rm -f *.o rfcr
