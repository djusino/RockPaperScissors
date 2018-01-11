all: referee play player
	touch make.log
referee: referee.o readstr.o
	gcc -o referee readstr.o referee.o
player: player.o readstr.o player.o
	gcc -o player readstr.o player.o
play: play.c
	gcc -o play play.c
readstr.o: readstr.c
	gcc -c readstr.c
referee.o: referee.c
	gcc -c referee.c
player.o: player.c
	gcc -c player.c
clean:
	rm -f *.o
	rm -f referee
	rm -f play
	rm -f player
	rm -f make.log
