#include "Vector2.h"

int Vector2::getX()const { return x; }
int Vector2::getY()const { return y; }
void Vector2::operator+(Vector2& v) { x += v.x; y += v.y; }
void Vector2::operator-(Vector2& v) { x -= v.x; y -= v.y; }
bool Vector2::operator==(const Vector2& v)const {
    return (x == v.x && y == v.y); }
void Vector2::prodVectorPorEscalar(int p) { x *= p; y *= p; }
void Vector2::operator*(Vector2& v) { x *= v.x; y *= v.y; }
void Vector2::setX(int x_) { x = x_; }
void Vector2::setY(int y_) { y = y_; }
