# Add additional compiler flags here
CFLAGS=-O3 -Wall -Wextra -DNDEBUG -g
## 
all: peano
peano: peano.c peano.S #testAssembler.c mirror.S reverse.S
	$(CC) $(CFLAGS) -o $@ $^ -lm
clean:
	rm -f peano
