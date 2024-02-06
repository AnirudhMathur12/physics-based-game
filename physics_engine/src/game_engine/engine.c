#include "engine.h"
#include <math.h>
#include <stdlib.h>

// Vector
float mod(vec2 *vec)
{
    return sqrt((vec->x) * (vec->x) + (vec->y) * (vec->y));
}

// PhysicsObject
void accelerate(PhysicsObject *obj, vec2 acc)
{
    obj->acc.x += acc.x;
    obj->acc.y += acc.y;
}

void UpdatePostion(PhysicsObject *obj, float dt)
{
    const vec2 velocity = {obj->current_pos.x - obj->prev_pos.x, obj->current_pos.y - obj->prev_pos.y};
    obj->prev_pos = obj->current_pos;
    obj->current_pos.x += (velocity.x + (obj->acc.x * dt * dt));
    obj->current_pos.y += (velocity.y + (obj->acc.y * dt * dt));
    obj->acc = (vec2){0, 0};
}

PhysicsObject *createPhysicsObject(int x, int y, float rad, int freefalling)
{
    PhysicsObject *temp = malloc(sizeof(PhysicsObject));
    temp->current_pos.x = x;
    temp->current_pos.y = y;
    temp->prev_pos = temp->current_pos;
    temp->radius = rad;
    temp->freefalling = freefalling;
    return temp;
}

// Solver
void update(Solver *solver, Link* link, PhysicsObject **objs, const int size, float dt)
{
    int substeps = 8;
    const float sub_dt = dt / (float)substeps;

    for (int i = substeps; i > 0; i--)
    {
        applyGravity(solver, objs, size);
        ApplyConstraint(solver, objs, size);
        solveCollisions(solver, objs, size);
        UpdatePositions(solver, objs, size, sub_dt);
        for(int i = size-1; i >= 1; i--)
        {
            applyLink(link, objs[i-1], objs[i]);
        }
    }
}

void applyGravity(Solver *solver, PhysicsObject **objs, const int size) {
    for (int i = 0; i < size; i++)
    {
        if(objs[i]->freefalling)
        {
            accelerate(objs[i], solver->gravity);
        }
    }
}

void UpdatePositions(Solver *solver, PhysicsObject **objs, const int size, float dt)
{
    for (int i = 0; i < size; i++)
    {
        if(objs[i]->freefalling)
        {
            UpdatePostion(objs[i], dt);
        }
    }
}

void ApplyConstraint(Solver *solver, PhysicsObject **objs, int size)
{
    const vec2 position = (vec2){640, 360};
    const float radius = 300.0f;
    for (int i = 0; i < size; i++)
    {
        const vec2 to_obj = {objs[i]->current_pos.x - position.x, objs[i]->current_pos.y - position.y};
        const float dist = mod((vec2 *)&to_obj);
        if (dist > radius - objs[i]->radius)
        {
            const vec2 n = {to_obj.x / dist, to_obj.y / dist};
            objs[i]->current_pos.x = position.x + n.x * (radius - objs[i]->radius);
            objs[i]->current_pos.y = position.y + n.y * (radius - objs[i]->radius);
        }
    }
}

void solveCollisions(Solver *solver, PhysicsObject **objs, int size)
{
    for (int i = 0; i < size; i++)
    {
        PhysicsObject *obj_1 = objs[i];
        for (int k = i + 1; k < size; k++)
        {
            PhysicsObject *obj_2 = objs[k];
            const vec2 collision_axis = {obj_1->current_pos.x - obj_2->current_pos.x, obj_1->current_pos.y - obj_2->current_pos.y};
            const float dist = mod((vec2 *)&collision_axis);
            const float min_dist = obj_1->radius + obj_2->radius;
            if (dist < min_dist)
            {
                const vec2 n = {collision_axis.x / dist, collision_axis.y / dist};
                const float delta = min_dist - dist;
                obj_1->current_pos.x += (0.5f) * delta * n.x;
                obj_1->current_pos.y += (0.5f) * delta * n.y;
                obj_2->current_pos.x -= (0.5f) * delta * n.x;
                obj_2->current_pos.y -= (0.5f) * delta * n.y;
            }
         }
    }
}

// Link
void applyLink(Link *link, PhysicsObject *obj1, PhysicsObject *obj2)
{
    link->obj_1 = obj1;
    link->obj_2 = obj2;

    const vec2 axis = {obj1->current_pos.x - obj2->current_pos.x, obj1->current_pos.y - obj2->current_pos.y};
    const float dist = mod((vec2*)&axis);

    const vec2 n = {axis.x/dist, axis.y/dist};
    const float delta = link->dist - dist;

    obj1->current_pos.x += 0.5f * n.x * delta;
    obj1->current_pos.y += 0.5f * n.y * delta;
    obj2->current_pos.x -= 0.5f * n.x * delta;
    obj2->current_pos.y -= 0.5f * n.y * delta;
}
