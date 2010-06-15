ChickenFinger : chickenFinger.o chickUtils.o  eventListen.o
	gcc -o ChickenFinger chickenFinger.o chickUtils.o  eventListen.o -lcurses -lpthread
chickUtils.o  : chickUtils.c
	gcc -c chickUtils.c
eventListen.o : eventListen.c	
	gcc -c eventListen.c
chickenFinger.o : chickenFinger.c
	gcc -c chickenFinger.c

clean:
	rm  chickenFinger.o chickUtils.o  eventListen.o ChickenFinger
