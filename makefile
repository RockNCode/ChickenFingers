ChickenFinger : chickenFinger.o chickUtils.o  eventListen.o displayThread.o
	gcc -o ChickenFinger chickenFinger.o chickUtils.o  eventListen.o displayThread.o -lcurses -lpthread
chickUtils.o  : chickUtils.c
	gcc -c chickUtils.c
eventListen.o : eventListen.c	
	gcc -c eventListen.c
chickenFinger.o : chickenFinger.c
	gcc -c chickenFinger.c
displayThread.o : displayThread.c
	gcc -c displayThread.c


clean:
	rm  chickenFinger.o chickUtils.o  eventListen.o displayThread.o ChickenFinger
