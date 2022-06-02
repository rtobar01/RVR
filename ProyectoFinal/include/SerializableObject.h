#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H
#include "Serializable.h"
#include <memory>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
class SerializableObject : public Serializable
{
public:
enum player
    {
        PLAYER1 = 0,
        PLAYER2 = 1
    };
enum MessageType
    {
        LOGOUT = 0,
        LOGIN   = 1,
        MOVEMENT  = 2,
        START = 3,
        WIN = 4
    };
    int16_t getX() {return x;};
    int16_t getY() {return y;};

    SerializableObject();
    void to_bin();
    int from_bin(char * bobj);
    virtual ~SerializableObject();
    player actualPlayer;
    int16_t x,y;
    MessageType type;
    static const size_t SIZE = sizeof(int) * 2 + sizeof(actualPlayer) + sizeof(type);

    protected:

    private:




};

#endif // SERIALIZABLEOBJECT_H
