#include "GameMap.h";
#include"Game.h";

GameMap::GameMap(int numC, int numR, Texture* t1, Texture* t2, Game* g) {
	texture = t1;
	texture2 = t2;
	numCol = numC; numRow = numR;
	gamePoint = g;
	cell = new MapCellType * [numRow];
	for (int i = 0; i < numRow; ++i)
		cell[i] = new MapCellType[numCol];
}

GameMap::~GameMap() {
	for (int i = 0; i < numRow; ++i)
		delete cell[i];
	delete cell;
	texture = nullptr; delete texture;
	gamePoint = nullptr; delete gamePoint;
}

void GameMap::render()const {
	SDL_Rect destRect;
	for (int r = 0; r < numRow; ++r) {
		for (int c = 0; c < numCol; ++c) {
			destRect.x = c * gamePoint->getCellW();
			destRect.y = r * gamePoint->getCellH();
			destRect.w = gamePoint->getCellW(); destRect.h = gamePoint->getCellH();
            if(cell[r][c] == Wall) {
                texture->render(destRect);
			}
			if(cell[r][c]==WinWall){
                texture2->render(destRect);
			}
		}
	}
}
