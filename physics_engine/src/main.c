#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "SDL_video.h"
#include "window/window.h"
#include "game_engine/engine.h"

#define OBJLIMIT 300

void INIT();
void UPDATE();

int running = 1;

int count = 0;

float ticksperframe = 1000.0f/60.0f;
SDL_Window* win;
SDL_Renderer* rend;
Solver* solver;

SDL_Rect pos = {100, 100, 50, 50};
SDL_Color color = {0,0,0,0};

SDL_Texture* background;

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
void renderObjects(SDL_Renderer* rend, PhysicsObject** objs, int size);
void spawnball(PhysicsObject** objs, int size, int* count);

PhysicsObject* objs[OBJLIMIT];

int main()
{
    INIT();
    //objs[0] = createPhysicsObject(850, 300);
    //objs[1] = createPhysicsObject(400, 300);
    //objs[2] = createPhysicsObject(650, 300);
    solver = malloc(sizeof(Solver));
    solver->gravity = (vec2){0.0f, 3000.0f};
    
    //win= createWindow("Physics Engine Demo", 1280, 720, 1280, 1280);
    win = SDL_CreateWindow("Window", 20, 20, 1280, 720, SDL_WINDOW_RESIZABLE);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    background = IMG_LoadTexture(rend, "background-grayishwhite.png");

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
                case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        spawnball(objs, 10, &count);
                        break;
                }
                break;
                default:
                break;
            }
        }
        UPDATE();
        int spawnticks = SDL_GetTicks();
        if(SDL_GetTicks()-spawnticks >= 0.5f)
        {
            spawnball(objs, count, &count);
        }
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
    SDL_RenderCopy(rend, background, NULL, NULL);
    filledCircleRGBA(rend, 640, 360, 300, 0, 0, 0, 255);
    renderObjects(rend, objs, count);
    //filledCircleRGBA(rend, ball->current_pos.x, ball->current_pos.y, 50, 255, 255, 255, 255);
    SDL_RenderPresent(rend);
    update(solver, objs, count, 1/60.0f);
}

void renderObjects(SDL_Renderer* rend, PhysicsObject** objs, int size)
{
    for(int i = 0; i < size; i++)
    {
        filledCircleRGBA(rend, objs[i]->current_pos.x, objs[i]->current_pos.y, objs[i]->radius, 255, 255, 255, 255);
    }
}

void spawnball(PhysicsObject** objs, int size, int* count)
{
    if(*count >= OBJLIMIT)
    {
        return;
    }
    objs[*count] = createPhysicsObject(850, 300, 10);
    (*count)++;
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
