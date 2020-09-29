all:
	cd ./yaml-cpp && pwd && mkdir -p build && cmake -B build && cd build && make && cp libyaml-cpp.a ../../ && cd ../../
	mv libyaml-cpp.a src
	make -C src

clean:
	make -C src clean

