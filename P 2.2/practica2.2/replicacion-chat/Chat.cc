#include "Chat.h"



// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------



void ChatMessage::to_bin()

{

    alloc_data(MESSAGE_SIZE);



    memset(_data, 0, MESSAGE_SIZE);



    //Serializar los campos type, nick y message en el buffer _data

    char* aux = _data;

    memcpy(aux, &type, sizeof(uint8_t));

    aux += sizeof(uint8_t);

    memcpy(aux, &nick, 8);

    aux += 8;

    memcpy(aux, &message, 80);

}



int ChatMessage::from_bin(char* bobj)

{

    alloc_data(MESSAGE_SIZE);



    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE);



    //Reconstruir la clase usando el buffer _data

    char* aux = _data;

    memcpy(&type, aux, sizeof(uint8_t));

    aux += sizeof(char) * sizeof(uint8_t);

    memcpy(&nick, aux, 8);

    aux += 8;

    memcpy(&message, aux, 80);

    return 0;

}



// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------



void ChatServer::do_messages()

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



        std::unique_ptr<ChatMessage> obj(new ChatMessage());

        Socket* cliente;

        socket.recv(*obj.get(), cliente);

        if (obj.get()->type == ChatMessage::LOGIN) clients.push_back(std::unique_ptr<Socket>(std::move(cliente)));

        else if (obj.get()->type == ChatMessage::LOGOUT)

        {

            std::vector<std::unique_ptr<Socket>>::iterator it = clients.begin();

            while (it != clients.end() && !(*it->get() == *cliente))

            {

                it++;

            }

            if (it != clients.end()) clients.erase(it);

        }

        else if (obj.get()->type == ChatMessage::MESSAGE)

        {

            for (int i = 0; i < clients.capacity(); i++)

            {

                if (clients[i].get() != cliente)

                    socket.send(*obj.get(), *clients[i]);

            }

        }

    }

}



// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------



void ChatClient::login()

{

    std::string msg;



    ChatMessage em(nick, msg);

    em.type = ChatMessage::LOGIN;



    socket.send(em, socket);

}



void ChatClient::logout()

{

    std::string msg;



    ChatMessage em(nick, msg);

    em.type = ChatMessage::LOGOUT;



    socket.send(em, socket);

}



void ChatClient::input_thread()

{

    while (true)

    {

        // Leer stdin con std::getline

        std::string msg;

        std::getline(std::cin, msg);

        // Enviar al servidor usando socket

        if (msg == "LOGIN") login();

        else if (msg == "LOGOUT") logout();

        else

        {



            ChatMessage em(nick, msg);

            em.type = ChatMessage::MESSAGE;



            socket.send(em, socket);

        }



    }

}



void ChatClient::net_thread()

{

    while (true)

    {

        //Recibir Mensajes de red

        ChatMessage obj;

        Socket* cliente;

        socket.recv(obj, cliente);

        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"

        //obj.from_bin();

        std::cout << obj.nick << ": " << obj.message;

    }

}



