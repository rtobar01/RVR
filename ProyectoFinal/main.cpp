#include "Game.h"
#include <iostream>
#include <string>

// argv[0] ejecutable
// argv[1] jugador
// argv[2] servidor
// argv[3] puerto

int main(int argc, char* argv[]){
    if(argc <= 1) {
        std::cout<<"Ruta sin argumentos: se inicia el servidor\n";
        Server* server = new Server("localhost","3000");
    }
    else{
        std::cout<<"Ruta con argumentos: "<< argv[1]<<"\n";
        bool player1;

        if(std::stoi(argv[1])==0){
            std::cout<<"JUGADOR 1 SELECCIONADO\n";
            player1=true;
            Game g = Game(player1,argv[2],argv[3]);
        }
        else if(std::stoi(argv[1])==1){
            std::cout<<"JUGADOR 2 SELECCIONADO\n";
            player1=false;
            Game g = Game(player1,argv[2],argv[3]);
        }
        else if(std::stoi(argv[1])==2)
        {
            std::cout<<"SERVIDOR\n";
            Server* server = new Server(argv[2],argv[3]);
        }

    }
    return 0;

}
