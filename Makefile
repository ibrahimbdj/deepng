pngfall: src/main.c src/png.c src/chunk.c src/chunk.h src/png.h src/utilities.c src/utilities.h
	gcc src/*.c -Wall -Wextra -o pngfall

clean:
	rm pngfall
