#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(Game* g, Vector2 v_, Texture* t): app(g), posIni(v_),
                    posAct(v_),dirMov(Vector2()), prevDir(Vector2()), text(t) { }

void GameObject::handleEvents(SDL_Event& event){
	if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_w) dirMov = Vector2(0, -1);
			else if (event.key.keysym.sym == SDLK_a) dirMov = Vector2(-1, 0);
			else if (event.key.keysym.sym == SDLK_s) dirMov = Vector2(0, 1);
			else if (event.key.keysym.sym == SDLK_d) dirMov = Vector2(1, 0);
		}
}

void GameObject::render(){
    SDL_Rect destRect;
    destRect.x = posAct.getX()* app->getCellW();
    destRect.y = posAct.getY() * app->getCellH();
	destRect.w = app->getCellW();
	destRect.h = app->getCellH();
    text->render(destRect);
}

void GameObject::update(){
    if (app->nextCell(posAct, dirMov))
		prevDir = dirMov;
	else app->nextCell(posAct, prevDir);
}

void GameObject::setX(int x_)
{
    posAct.setX(x_);
}

void GameObject::setY(int y_)
{
    posAct.setY(y_);
}
