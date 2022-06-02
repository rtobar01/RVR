#include "Game.h"
#include "SDL2/SDL_error.h"
#include <string>
using namespace std;

Game::Game(bool p1, const char *  serv_, const char *  puerto_): isPlayer1(p1)  {
	SDL_Init(SDL_INIT_EVERYTHING);
	const char* windowName = p1 ? "JUGADOR1" : "JUGADOR2";
	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	for(int i = 0; i<NUM_TEXTURES; i++){
        texturesArray[i] = new Texture(renderer, TEXTURES_ATRIB[i].filename, TEXTURES_ATRIB[i].numRows, TEXTURES_ATRIB[i].numCols);
	}

	loadObjects();
	cliente = new Client(serv_,puerto_);
    cliente->login();
    login = true;

	std::cout << "Cliente socket sd: " << cliente->getSocket().sd << "\n";
	run();
}

Game::~Game() {
	for (Texture* t : texturesArray)delete t;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::run() {
	int startTime, frameTime;
	startTime = SDL_GetTicks();
	while (!exit_) {
		handleEvents();
		update();
		render();
		SDL_Delay(200);
	}
}

void Game::loadObjects(){
    fstream mapFile;
    mapFile.open("./resources/mapa.txt");
    if (!mapFile.is_open()) {
        mapFile.open("./mapa.txt");
    }
    int numCol, numRow;
	mapFile >> numRow >> numCol;
	mapa = new GameMap(numCol, numRow, texturesArray[Pared_Text], texturesArray[Win_Text], this);
	int num;
	cellW = WIN_WIDTH / numCol;
	cellH = WIN_HEIGHT / numRow;

	for (int r = 0; r < mapa->numRow; ++r) {
		for (int c = 0; c < mapa->numCol; ++c) {
			mapFile >> num;
			if (num > 1) { // o jugador1 o jugador 2 o meta
                if(num ==7) mapa->cell[r][c] = MapCellType(WinWall);
				else mapa->cell[r][c] = Empty;
				if (num == 2)
					player1 = new GameObject(this, Vector2(c,r), texturesArray[Player1_Text]);
                else if(num==3)
                    player2 = new GameObject(this, Vector2(c,r), texturesArray[Player2_Text]);
			}
			else {
				mapa->cell[r][c] = MapCellType(num); // o vacío o pared o winCell
			}
		}

	}
	mapFile.close();
}

void Game::handleEvents(){
    SDL_Event event;
     //std::cout << " Handle Events  \n";
	while (SDL_PollEvent(&event) && !exit_) {
		if (event.type == SDL_QUIT)exit_ = true;
		if(isPlayer1) player1->handleEvents(event);
		else player2->handleEvents(event);
	}
}

void Game::update(){
    //std::cout << "Si quitas este cout se produce un segmentation fault en el jugador 1 100% real no fake\n";
    cliente->net_thread();
    start = cliente->startBuffer;
    if (start) {
        if(isPlayer1) {
            player1->update();
            cliente->SendPosition(player1->getPosAct().getX(),player1->getPosAct().getY(),true);
            player2->setX(cliente->xBuffer);
            player2->setY(cliente->yBuffer);
            if(win()&&start){
                std::cout<<"HAS GANADO :D\n";
                cliente->SendWin(true);
            }
        }
        else {
            player2->update();
            cliente->SendPosition(player2->getPosAct().getX(),player2->getPosAct().getY(),false);
            player1->setX(cliente->xBuffer);
            player1->setY(cliente->yBuffer);
            if(win()&&start){
                std::cout<<"HAS GANADO :D\n";
                cliente->SendWin(false);
            }
         }

    }
}

void Game::render()const {
	SDL_RenderClear(renderer);
	mapa->render();
	player1->render();
    player2->render();
	SDL_RenderPresent(renderer);
}


bool Game::nextCell(Vector2& posAct, Vector2& dir) const {
	posAct + dir;
	int x, y;
	x = posAct.getX();
	y = posAct.getY();

	if (posAct.getX() > mapa->numCol - 1) {
		posAct = Vector2(0, posAct.getY()); //salida derecha
		return true;
	}
	else if (posAct.getX() < 0) {
		posAct = Vector2(mapa->numCol - 1, posAct.getY()); //salida iquierda
		return true;
	}
	else if (posAct.getY() > mapa->numRow - 1) {
		posAct = Vector2(posAct.getX(), 0); //abajo
		return true;
	}
	else if (posAct.getY() < 0) {
		posAct = Vector2(posAct.getX(), mapa->numRow - 1); //arriba
		return true;
	}

	if (mapa->cell[y][x] == Wall) {
		posAct - dir;
		return false;
	}
	else return true;
}


bool Game::checkDir(const Vector2& posAct, const Vector2& dir) const {
	Vector2 pA, d; pA = Vector2(); d = Vector2();
	pA = posAct; d = dir;
	pA + d;
	int x, y;
	x = pA.getX();
	y = pA.getY();
	if (x >= 0 && x < mapa->numCol && y >= 0 && y < mapa->numRow) {
		if (mapa->cell[y][x] == Wall) return false;
	}
	else return true;
}

bool Game::win(){
    if(isPlayer1){
        int x = player1->getPosAct().getX();
        int y = player1->getPosAct().getY();
        if(mapa->cell[y][x])
            return true;
    }
    else{
        int x = player2->getPosAct().getX();
        int y = player2->getPosAct().getY();
        if(mapa->cell[y][x])
            return true;
    }
    return false;
}
