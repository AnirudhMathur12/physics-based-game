#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>

typedef struct vec2
{
    float x,y;
} vec2;

typedef struct PhysicsObject 
{
    vec2 current_pos, prev_pos, acc;

} PhysicsObject;

typedef struct Solver
{
    vec2 gravity;
} Solver;

//vec2
float mod(vec2* vec);

//Physics Object
PhysicsObject* createPhysicsObject(int x, int y);
void UpdatePostion(PhysicsObject* obj, float dt);
void accelerate(PhysicsObject* obj, vec2 acc);

//Solver
void update(Solver* solver, PhysicsObject** objs, const int size, float dt);
void applyGravity(Solver* solver, PhysicsObject** objs, const int size);
void UpdatePositions(Solver* solver, PhysicsObject** objs, const int size, float dt);
void ApplyConstraint(Solver* solver, PhysicsObject** objs, int size);

#endif
