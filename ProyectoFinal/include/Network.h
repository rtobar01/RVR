#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>
#include "Socket.h"
#include "GameObject.h"
#include "SerializableObject.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 *  Mensaje del protocolo de la aplicación de Chat
 *
 *  +-------------------+
 *  | Tipo: uint8_t     | 0 (login), 1 (mensaje), 2 (logout)
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */

/**
 *  Clase para el servidor de chat
 */
class Server {
public:
    Server(const char * s, const char * p): socket(s, p)
    {
        cout << "constructora chat server" << "\n";
        socket.bind();
        update();
    };

    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void update();

private:
    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */
    std::vector<std::unique_ptr<Socket>> clients;

    /**
     * Socket del servidor
     */
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el cliente de chat
 */
class Client {
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     */
    Client(const char * s, const char *  p);

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void SendPosition(int x, int y,bool player1);
    void SendWin(bool player1);
    Socket getSocket() {return socket;};
    void net_thread();
    bool startBuffer = false;
    int16_t xBuffer,yBuffer;
private:

    /**
     * Socket para comunicar con el servidor
     */
    Socket socket;

};

