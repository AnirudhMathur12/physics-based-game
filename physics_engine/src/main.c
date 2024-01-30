#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_events.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "SDL_video.h"
#include "window/window.h"
#include "game_engine/engine.h"


void INIT();
void UPDATE();

int running = 1;

float ticksperframe = 1000.0f/60.0f;
SDL_Window* win;
SDL_Renderer* rend;
Solver* solver;

SDL_Rect pos = {100, 100, 50, 50};
SDL_Color color = {0,0,0,0};

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);

PhysicsObject* objs[10];
PhysicsObject* ball;

int main()
{
    INIT();
    ball = createPhysicsObject(800, 450);
    objs[0] = ball;
    solver = malloc(sizeof(Solver));
    solver->gravity = (vec2){0.0f, 4000.0f};
    
    //win= createWindow("Physics Engine Demo", 1280, 720, 1280, 1280);
    win = SDL_CreateWindow("Window", 20, 20, 1280, 720, 0);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    while(running)
    {
        int startTick = SDL_GetTicks();
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                running = 0;
                break;
                default:
                break;
            }
        }
        UPDATE();
        int frameticks = SDL_GetTicks() - startTick;
        if(frameticks < ticksperframe)
        {
            SDL_Delay(ticksperframe - frameticks);
        }
    }

    printf("Hello, world!\n");
    return 0;
}

void INIT()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Failed to init SDL. Error: %s", SDL_GetError());
    }

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("Failed to init IMG. Error: %s", IMG_GetError());
    }
}


void UPDATE()
{
    SDL_RenderClear(rend);
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    DrawCircle(rend, ball->current_pos.x, ball->current_pos.y, pos.w);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderPresent(rend);
    update(solver, objs, 1, 1/60.0f);
}


void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
