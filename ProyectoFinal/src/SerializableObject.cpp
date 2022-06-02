#include "SerializableObject.h"

SerializableObject::SerializableObject() : x(0),y(0), actualPlayer(PLAYER1),type(LOGIN)
{
    //ctor
}

SerializableObject::~SerializableObject()
{
    //dtor
}


void SerializableObject::to_bin()
{
    alloc_data(SIZE);

    memset(_data, 0, SIZE);

    //Serializar los campos type, nick y message en el buffer _data
     char *aux = _data;
        memcpy(aux, &x, sizeof(uint16_t));
        aux+= sizeof(uint16_t);
        memcpy(aux, &y, sizeof(uint16_t));
        aux+= sizeof(uint16_t);
        memcpy(aux, &actualPlayer, sizeof(player));
        aux+= sizeof(player);
        memcpy(aux, &type, sizeof(type));
}


int SerializableObject::from_bin(char * bobj)
{
    alloc_data(SIZE);

    memcpy(static_cast<void *>(_data), bobj, SIZE);

    //Reconstruir la clase usando el buffer _data
    char* aux = _data;
    memcpy(&x, aux, sizeof(int16_t));
    aux+=sizeof(int16_t);
    memcpy(&y, aux, sizeof(int16_t));
    aux+=sizeof(int16_t);
    memcpy(&actualPlayer, aux, sizeof(player));
    aux+=sizeof(player);
    memcpy(&type, aux, sizeof(type));
    return 0;
}
