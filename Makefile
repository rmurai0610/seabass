all:
	mkdir -p build;              \
	cd build;                    \
	cmake ..;                    \
	make -j;                     \
	cp compile_commands.json ..; \
	cd ..;                       \

clean:
	cd build; make clean; cd ..;

test:
	cd build;                    \
	make test ;                  \
	cd ..;                       \

.PHONY: clean test
