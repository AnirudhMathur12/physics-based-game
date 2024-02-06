#include "SDL_hints.h"
#ifdef __APPLE__
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "game_engine/engine.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#endif

#ifdef __linux__
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include "SDL2/game_engine/engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#define OBJLIMIT 17

#define PI 3.14159265359

void INIT();
void UPDATE();

int running = 1;

int count = 9;

float ticksperframe = 1000.0f / 60.0f;
SDL_Window* win;
SDL_Renderer* rend;
Solver* solver;
Link* link;

SDL_Rect pos = {100, 100, 50, 50};
SDL_Color color = {0, 0, 0, 0};

SDL_Texture* background;

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);
void renderObjects(SDL_Renderer *rend, PhysicsObject **objs, int size);
void spawnball(PhysicsObject **objs, int *count);

PhysicsObject* objs[OBJLIMIT];

int main()
{
    INIT();
    // objs[0] = createPhysicsObject(850, 300);
    // objs[1] = createPhysicsObject(400, 300);
    // objs[2] = createPhysicsObject(650, 300);
    solver = malloc(sizeof(Solver));
    solver->gravity = (vec2){0.0f, 3000.0f};

    link = malloc(sizeof(Link));
    link->dist = 21;

    objs[0] = createPhysicsObject(550, 300, 10, 0);
    objs[1] = createPhysicsObject(560, 300, 10, 1);
    objs[2] = createPhysicsObject(570, 300, 10, 1);
    objs[3] = createPhysicsObject(580, 300, 10, 1);
    objs[4] = createPhysicsObject(590, 300, 10, 1);
    objs[5] = createPhysicsObject(600, 300, 10, 1);
    objs[6] = createPhysicsObject(610, 300, 10, 1);
    objs[7] = createPhysicsObject(620, 300, 10, 1);
    objs[8] = createPhysicsObject(630, 300, 10, 1);
    

    // win= createWindow("Physics Engine Demo", 1280, 720, 1280, 1280);
    win = SDL_CreateWindow("Window", 20, 20, 1280, 720, SDL_WINDOW_RESIZABLE);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    background = IMG_LoadTexture(rend, "background-grayishwhite.png");
    int spawnticks = SDL_GetTicks();

    SDL_Event e;
    while (running)
    {
        int startTick = SDL_GetTicks();
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                            spawnball(objs, &count);
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        UPDATE();
        /*
        if(SDL_GetTicks() - spawnticks>300)
        {
            spawnball(objs, &count);
            spawnticks = SDL_GetTicks();
        }
        */
        int frameticks = SDL_GetTicks() - startTick;
        if (frameticks < ticksperframe)
        {
            SDL_Delay(ticksperframe - frameticks);
        }
    }

    printf("Hello, world!\n");
    SDL_Quit();
    return 0;
}

void INIT()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Failed to init SDL. Error: %s", SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
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
    SDL_RenderPresent(rend);
    update(solver, link, objs, count, 1 / 60.0f);
    objs[0]->current_pos.x = 550;
    objs[0]->current_pos.y = 300;
}

void renderObjects(SDL_Renderer *rend, PhysicsObject **objs, int size)
{
    for (int i = 0; i < size; i++)
    {
        filledCircleRGBA(rend, objs[i]->current_pos.x, objs[i]->current_pos.y,
                         objs[i]->radius, 255, 255, 255, 255);
    }
}

void spawnball(PhysicsObject **objs, int *count)
{
    if (*count >= OBJLIMIT)
    {
        return;
    }
    objs[*count] = createPhysicsObject(700, 300, 10, 1);
    //accelerate(objs[*count], (vec2){500000*cos(*count), 500000*sin(*count)});
    (*count)++;
}

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
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
