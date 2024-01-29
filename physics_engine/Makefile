SRC = src/main.c src/window/window.c src/game_engine/engine.c
APP = physics_engine

build:
	gcc `pkg-config --cflags --libs sdl2 sdl2_image sdl2_gfx` -o $(APP) $(SRC) -Wall

run:
	make build
	./$(APP)
