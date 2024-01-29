#include "window.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <stdlib.h>

Window* createWindow(const char* name, int w, int h, int posx, int posy)
{
    Window* windw_ptr = malloc(sizeof(Window));
    windw_ptr->win = SDL_CreateWindow(name, w, h, posx, posy, 0);
    windw_ptr->rend = SDL_CreateRenderer(windw_ptr->win, -1, SDL_RENDERER_ACCELERATED);
    return windw_ptr;
}
