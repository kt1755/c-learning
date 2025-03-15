.PHONY: fork-sem clean
fork-sem:
	gcc -o fork-sem fork_semaphores_sync.c

all: fork-sem
	gcc -o 47-5.out 47-5.c

clean:
	rm -f fork-sem
	rm -f 47-5.out