
all:
		gcc -g util.c -o util -lm
		mkdir -p predicoes/

run:
		./util
