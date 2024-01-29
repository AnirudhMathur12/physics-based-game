#include "engine.h"
#include <stdlib.h>

//PhysicsObject
void accelerate(PhysicsObject* obj, vec2 acc)
{
    obj->acc.x += acc.x;
    obj->acc.y += acc.y;
}


void UpdatePostion(PhysicsObject* obj, float dt)
{
    const vec2 velocity = {obj->current_pos.x - obj->prev_pos.x, obj->current_pos.y - obj->prev_pos.y};
    obj->prev_pos = obj->current_pos;
    obj->current_pos.x += (velocity.x + (obj->acc.x * dt * dt));
    obj->current_pos.y += (velocity.y + (obj->acc.y * dt * dt));
    obj->acc = (vec2){0, 0};
}

PhysicsObject* createPhysicsObject(int x, int y)
{
    PhysicsObject* temp = malloc(sizeof(PhysicsObject));
    temp->current_pos.x = x;
    temp->current_pos.y = y;
    temp->prev_pos = temp->current_pos;
    return temp;
}

//Solver
void update(Solver* solver, PhysicsObject** objs, const int size, float dt)
{
    applyGravity(solver, objs, size);
    UpdatePositions(solver, objs, size, dt);
    
}

void applyGravity(Solver* solver, PhysicsObject** objs, const int size)
{
    for(int i = 0; i < size; i++)
    {
        accelerate(objs[i], solver->gravity);
    }
}

void UpdatePositions(Solver* solver, PhysicsObject** objs, const int size, float dt)
{
    for(int i = 0; i < size; i++)
    {
        UpdatePostion(objs[i], dt);
    }
}
