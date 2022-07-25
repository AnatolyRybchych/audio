
out		:= run.exe

objects	:= obj/main.o
objects	+= obj/wav.o

build:$(objects)
	gcc -o $(out) $^ -lwinmm

obj/%.o:src/%.c
	gcc -c -o $@ $<

run: build
	./$(out)
