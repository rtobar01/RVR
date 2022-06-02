#ifndef GAME_H
#define GAME_H
#include<array>
#include<fstream>
#include "GameObject.h"
#include "GameMap.h"
#include "Network.h"

using uint = unsigned int;
const uint WIN_WIDTH = 720;
const uint WIN_HEIGHT = 720;
const int FRAME_RATE = 500;
const uint NUM_TEXTURES = 4;
enum TextureType { Player1_Text, Player2_Text, Pared_Text,Win_Text };
struct TextureAtrb {
	string filename;
	int numRows;
	int numCols;
};

struct TextureFrame {
	int x; int y;
};

const TextureAtrb TEXTURES_ATRIB[NUM_TEXTURES] = { {"./player_1.png",1,1}, {"./player_2.png",1,1}, {"./pared.png",1,1}, {"./rojo.png",1,1}};



class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	array<Texture*, NUM_TEXTURES> texturesArray;

	int cellW, cellH;
	bool exit_ = false;
    bool start = false;
    bool login = false;
	GameMap* mapa;
	GameObject* player1, *player2;

	void loadObjects();

    bool isPlayer1;
    Client* cliente;
public:
	Game(bool p1,const char *  serv,const char *  puerto);
	~Game();
	void run();
	void handleEvents();
	void render()const;
	void update();
	Texture* getTexture(TextureType type) { return nullptr; }
	void exit() { exit_ = true; }
   	bool nextCell(Vector2& posAct, Vector2& dir)const;
   	bool checkDir(const Vector2& posAct, const Vector2& dir)const;
   	bool win();

	//GETTERS / SETTERS
	int getCellW()const { return cellW; }
	int getCellH()const { return cellH; }
	int getWinW() const { return WIN_WIDTH; }
};

#endif // GAME_H
