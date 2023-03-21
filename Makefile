build:
	clang++ -Wall --std=c++14 ./srcs/**/*.cpp -I/System/Volumes/Data/sgoinfre/students/alvrodri/homebrew/include/SDL2 -D_THREAD_SAFE -L/System/Volumes/Data/sgoinfre/students/alvrodri/homebrew/lib -lSDL2-2.0.0 -lm -o renderer -g -fsanitize=address

run:
	./renderer

clean:
	rm renderer
