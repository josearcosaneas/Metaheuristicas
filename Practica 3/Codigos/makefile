CFLAGS = -g -c

C =  usogenetico.c input.c clustering.c timer.c

O =  usogenetico.o input.o clustering.o timer.o

.c.o:
	g++ $(CFLAGS) $(C)

usogenetico: usogenetico.o $(O)
	g++ -o usogenetico $(O) -lm

clear:
	rm -r *.o