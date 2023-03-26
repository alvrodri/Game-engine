builds:
	/opt/homebrew/opt/llvm/bin/clang++ -Wall ./srcs/main.cpp ./srcs/engine/engine.cpp ./srcs/color/colorbuffer.cpp ./srcs/vector/vector.cpp  $(shell sdl2-config --cflags) $(shell sdl2-config --libs) -lm -L/opt/homebrew/opt/llvm/lib -I/opt/homebrew/opt/llvm/include -o renderer -g -fsanitize=address
run:
	./renderer
clean:
	rm renderer

