#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Vector2.h"
#include "Texture.h"
#include "Serializable.h"
class Game;

class GameObject {
protected:
	Vector2 posIni, posAct, dirMov, prevDir;
	Texture* text;
	Game* app = nullptr;

public:
	GameObject(Game* g, Vector2 v_, Texture* t);
	~GameObject() {};
	void render();
	void update();
	void handleEvents(SDL_Event& event);
	void setX(int x_);
	void setY(int y_);
	Vector2 getPosAct() {return posAct;};
};

#endif // GAMEOBJECT_H
