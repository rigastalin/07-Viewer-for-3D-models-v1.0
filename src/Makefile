# .PHONY: all install uninstall clean dvi dist tests gcov_report lint leaks

CC = gcc
# CFLAGS=-fsanitize=address -Wall -Werror -Wextra -std=c11
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
DIR=./build/
NAME=3D_Viewer_FLAPPYBOOBS
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit
else
	LIBS=-lcheck
endif

all: clean build

build: clean
	mkdir $(DIR); cd $(DIR); qmake ../QT/QT_3D/; make; rm -rf *.c *.h *.cpp *.ui *.o

install: build
	mkdir $(HOME)/Desktop/$(NAME)/
	cp -rf $(DIR)QT_3D.app $(HOME)/Desktop/$(NAME)/
	make clean

uninstall:
	rm -rf $(HOME)/Desktop/$(NAME)/

dvi:
	open -a "Google Chrome" ./html/description.html

dist:
	rm -rf archive_v1
	mkdir archive_v1
	cp Makefile *.c *.h archive_v1/
	cp -a QT archive_v1
	cp -a html archive_v1
	tar -cvzf $(HOME)/Desktop/archive_v1.tgz archive_v1/
	rm -rf archive_v1/

tests: 
	$(CC) $(CFLAGS) s21_test.c parse.c afins.c $(LIBS) -o parse_test.out
	./parse_test.out

gcov_report:
	$(CC) $(GCOV) s21_test.c parse.c afins.c $(LIBS) -o GCOV_test.out
	./GCOV_test.out
	lcov -t "GCOV_test" -o GCOV_test.info -c -d . 
	genhtml -o ./report GCOV_test.info
	open -a "Google Chrome" report/index.html

leaks:
	CK_FORK=no leaks --atExit -- ./parse_test.out
	
check:
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	cppcheck --enable=all --suppress=missingIncludeSystem parse.c parse.h afins.c afins.h s21_test.c
	python3 ../materials/linters/cpplint.py --extensions=c parse.c parse.h afins.c afins.h s21_test.c
	@rm -f *.cfg

clean:
	rm -f *.o *.a *.gcno *.gcda *.info *.out
	rm -rf ./report
	rm -rf ./build
	rm -rf ./Documentation
	rm -rf $(HOME)/Desktop/$(NAME)_archive.tgz


