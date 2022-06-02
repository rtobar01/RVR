#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "Texture.h"
enum MapCellType { Empty, Wall, WinWall};
using namespace std;
class Game;
class GameMap {
friend class Game;
private:
	Game* gamePoint;
	int numCol, numRow;
	MapCellType** cell;
	Texture* texture, * texture2;

public:
	GameMap(int nr, int nc, Texture* t1, Texture* t2, Game* g);
	~GameMap();
	void render()const;

};
#endif // GAMEMAP_H
