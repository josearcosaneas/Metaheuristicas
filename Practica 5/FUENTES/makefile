CFLAGS = -g -c

C =  memetico.c input.c clustering.c timer.c

O =  memetico.o input.o clustering.o timer.o

.c.o:
	g++ $(CFLAGS) $(C)

memetico: memetico.o $(O)
	g++ -o memetico $(O) -lm

clear:
	rm -r *.o
