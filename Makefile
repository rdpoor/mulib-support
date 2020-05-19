# see http://martinvseticka.eu/temp/make/presentation.html

.PHONY:	test clean

export MU_ROOT=.

all :
	cd source && make all

clean :
	cd source && make clean
	cd test && make clean

test :
	cd test && make test
