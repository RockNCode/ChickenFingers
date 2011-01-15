ChickenFinger : chickenFinger.o chickUtils.o  eventListen.o dispThread.o
	gcc -o ChickenFinger chickenFinger.o chickUtils.o  eventListen.o dispThread.o -lcurses -lpthread
chickUtils.o  : chickUtils.c
	gcc -c chickUtils.c
eventListen.o : eventListen.c
	gcc -c eventListen.c
dispThread.o : dispThread.c
	gcc -c dispThread.c
chickenFinger.o : chickenFinger.c
	gcc -c chickenFinger.c

clean:
	rm  chickenFinger.o chickUtils.o  eventListen.o ChickenFinger dispThread.o
