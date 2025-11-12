#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <raylib.h>
#include <raymath.h>
using namespace std;
using Frame = Rectangle;
using Size = Vector2;
using Position = Vector2;

struct Hitbox {
    Position pos;
    float radius;
};

#endif // GEOMETRY_HPP
