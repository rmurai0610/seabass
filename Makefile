all:
	./build.sh;

clean:
	cd build; make clean; cd ..;

test:
	cd build;                    \
	make test ;                  \
	cd ..;                       \

.PHONY: clean test
