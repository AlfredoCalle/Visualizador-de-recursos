procesos: main.c
	gcc -o procesos main.c

.PHONY: clean
clean:
	rm -f procesos