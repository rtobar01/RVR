#include <thread>
#include "Chat.h"

Client::Client(const char * s, const char *  p):socket(s, p) {
    xBuffer = 0;
    yBuffer = 0;
    std::cout<<"Datos del cliente: server->"<<s<<" port->"<<p<<"\n";
}

