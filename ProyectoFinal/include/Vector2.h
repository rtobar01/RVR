#ifndef VECTOR2_H
#define VECTOR2_H

using namespace std;
class Vector2 {
private:
	int x;
	int y;


public:
	Vector2(int x_ = 0, int y_ = 0) :x(x_), y(y_) {}; //Vector2D()
	int getX()const;
	int getY()const;
	void operator+(Vector2& v);
	void operator-(Vector2& v);
	void prodVectorPorEscalar(int x);
	void operator*(Vector2& v);
	bool operator==(const Vector2&)const;
	void setX(int x_);
	void setY(int y_);

};
#endif // VECTOR2_H
