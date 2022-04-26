#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h> 
const int MAX_NAME=80;
class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
        
        alloc_data((2*sizeof(int16_t) + sizeof(char)*MAX_NAME));
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        
        char *aux = _data;
        memcpy(aux, name, sizeof(char)*MAX_NAME);
        aux+= sizeof(char)*MAX_NAME;
        memcpy(aux, &x, sizeof(int16_t));
        aux += sizeof(int16_t);
        memcpy(aux, &y, sizeof(int16_t));   
    }

    int from_bin(char * data)
    {
        char* aux = _data;
        memcpy(name, aux, sizeof(char)*MAX_NAME);
        aux+=sizeof(char)*MAX_NAME;
        memcpy(&x, aux, sizeof(int16_t));
        aux += sizeof(int16_t);
        memcpy(&y, aux, sizeof(int16_t));
        return 0;
    }


public:
    
    char name[MAX_NAME];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    char name [] = "PlayerONE";
    Jugador one_w(name, 123, 789);

    // 1. Serializar el objeto one_w
    one_w.to_bin();
    // 2. Escribir la serialización en un fichero
    int fichero = open("jugador.txt", O_CREAT|O_WRONLY);
    int16_t a = 6;
    write(fichero, one_w.data(), one_w.size());
    close(fichero);
    // 3. Leer el fichero
    int lectura = open("jugador.txt", O_RDONLY);
    int r = read(lectura, one_r.data(), one_w.size());
    // 4. "Deserializar" en one_r
    one_r.from_bin(nullptr);
    // 5. Mostrar el contenido de one_r
    std::cout<< "Name: "<<one_r.name<<"\nX: " << one_r.x <<" Y: "<<one_r.y<<"\n";

    return 0; 
}

