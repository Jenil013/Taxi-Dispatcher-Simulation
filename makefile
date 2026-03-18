all:		simulator generator customer stop
		gcc -o simulator simulator.o -lX11 -lpthread
		gcc -o generator generator.o
		gcc -o customer customer.o
		gcc -o stop stop.o

simulator:	simulator.c display.c dispatchCenter.c taxi.c simulator.h
		gcc -c simulator.c

generator:	generator.c simulator.h
		gcc -c generator.c

customer: 	customer.c simulator.h
		gcc -c customer.c

stop:		stop.c simulator.h
		gcc -c stop.c

clean:
		rm -f simulator.o generator.o customer.o stop.o simulator taxi generator customer stop
