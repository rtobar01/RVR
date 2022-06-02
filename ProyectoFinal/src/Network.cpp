#include "Network.h"

void Server::update()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        SerializableObject obj;
        Socket* cliente;
        cliente->sa_len = socket.sa_len;
        socket.recv(obj,cliente,true);

        if (obj.type == SerializableObject::LOGIN)
        {
            if (clients.size() < 2){
                clients.push_back(std::move(std::make_unique<Socket>(*cliente)));
                std::cout << "Se recibe login: " << clients.size() << "\n";
            }
            if (clients.size() == 2) {
                std::cout << "SendStart\n";
                SerializableObject start;
                start.type = SerializableObject::START;
                socket.send(start,*clients[0].get());
                socket.send(start,*clients[1].get());
            }
        }

        else if (obj.type == SerializableObject::LOGOUT) {
           std::vector<std::unique_ptr<Socket>>::iterator it = clients.begin();
           while (it != clients.end() && !(*it->get() == *cliente))
                it++;
            if (it != clients.end()) clients.erase(it);
        }
        else if (obj.type == SerializableObject::MOVEMENT && clients.size() == 2) {
            if (obj.actualPlayer == SerializableObject::PLAYER1) socket.send(obj,*clients[1].get());
            else socket.send(obj,*clients[0].get());
        }
        else if(obj.type == SerializableObject::WIN){
            if(obj.actualPlayer == SerializableObject::PLAYER1) socket.send(obj, *clients[1].get());
            else socket.send(obj, *clients[0].get());
        }

    }

}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

Client::Client(const char * s, const char *  p):socket(s, p) {
    xBuffer = 0;
    yBuffer = 0;
    std::cout<<"Datos del cliente: server->"<<s<<" port->"<<p<<"\n";
}


void Client::login()
{
    SerializableObject em = SerializableObject();
    em.type = SerializableObject::LOGIN;
    cout << "Login jugador enviado en el scoket: " <<socket<< "\n";
    std::cout<< socket.sd<<"\n";
    socket.send(em, socket);
}

void Client::logout()
{
    SerializableObject em;
    em.type = SerializableObject::LOGOUT;

    socket.send(em, socket);
}


void Client::net_thread()
{
    //Recibir Mensajes de red
    SerializableObject obj;
    Socket* cliente;
    socket.recv(obj,cliente,false);
    if(obj.type == SerializableObject::LOGIN) return;

    if (obj.type == SerializableObject::START) startBuffer = true;
    if (obj.type == SerializableObject::MOVEMENT) {
        xBuffer = obj.getX();
        yBuffer = obj.getY();
    }
    if (obj.type == SerializableObject::WIN) {
        startBuffer = false;
        std::cout<< "HAS PERDIDO :(\n";
    }
}

void Client::SendPosition(int x, int y,bool player1)
{
    SerializableObject em = SerializableObject();
    em.type = SerializableObject::MOVEMENT;
    em.x = x;
    em.y = y;
    if (player1) em.actualPlayer = SerializableObject::PLAYER1;
    else em.actualPlayer = SerializableObject::PLAYER2;
    socket.send(em, socket);
}

void Client::SendWin(bool player1)
{
    SerializableObject em = SerializableObject();
    em.type = SerializableObject::WIN;
    if (player1) em.actualPlayer = SerializableObject::PLAYER1;
    else em.actualPlayer = SerializableObject::PLAYER2;
    startBuffer = false;
    socket.send(em, socket);
}
