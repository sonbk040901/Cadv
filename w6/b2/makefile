main : main.o jrb.o jval.o
	gcc -o main main.o jrb.o jval.o -g
main.o : main.c
	gcc -c main.c
jrb.o : jrb.h
	gcc -c jrb.c
jval.o : jval.h
	gcc -c jval.c
