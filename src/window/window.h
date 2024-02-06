#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

typedef struct Window
{
    SDL_Window* win;
    SDL_Renderer* rend;
} Window;

Window* createWindow(const char* name, int w, int h, int posx, int posy);
#endif
