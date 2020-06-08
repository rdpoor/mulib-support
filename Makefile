# see http://martinvseticka.eu/temp/make/presentation.html

.PHONY:	test clean

export MU_ROOT=.

all :
	@cd source && make all
	@cd ports/ubuntu && make all

clean :
	@cd source && make clean
	@cd ports/ubuntu && make clean
	@cd test && make clean

test : all
	@cd test && make test
