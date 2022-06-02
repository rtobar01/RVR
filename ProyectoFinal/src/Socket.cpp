#include <string.h>

#include "Serializable.h"
#include "SerializableObject.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    //Construir un socket de tipo AF_INET y SOCK_DGRAM usando getaddrinfo.
    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    struct addrinfo hints;
    struct addrinfo* result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;



    int rc = getaddrinfo(address, port, &hints, &result);
    if (rc != 0) {
        std::cerr << "ERROR en getaddrinfo\n";
         //printf("Error: %s\n",strerror(errno));
    }

    sd = socket(result->ai_family, result->ai_socktype, 0);

    if(sd==-1){
        std::cerr<< "ERROR al crear el socket\n";
    }
    sa = (*result->ai_addr);
    sa_len = result->ai_addrlen;
    freeaddrinfo(result);
}

int Socket::recv(Serializable &obj, Socket * &sock, bool wait) {
    struct sockaddr sa_cliente;
    socklen_t sa_len_cliente = sizeof(sa);
    char buffer[SerializableObject::SIZE];
    ssize_t bytes = 0;
    if(wait)
    bytes = ::recvfrom(sd, buffer, SerializableObject::SIZE, 0, &sa_cliente, &sa_len_cliente);
    else bytes = ::recvfrom(sd, buffer, SerializableObject::SIZE, MSG_DONTWAIT, &sa_cliente, &sa_len_cliente);
    //std::cout<< "Bytes: " << bytes<<"\n";
    if ( bytes <= 0 ) {
        //printf("Error: %s\n",strerror(errno));
        return -1;
    }
    if(sock!=0){
        sock = new Socket(&sa_cliente, sa_len_cliente);
        sock->sd = sd;
    }
    obj.from_bin(buffer);
    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    //Serializar el objeto
    //Enviar el objeto binario a sock usando el socket sd
    //std::cout << "Socket send " << "sa del propio socket: "  << *this << "socket pasado como param: " << sock << "\n";
    obj.to_bin();
    //std::cout << "Socket send " << "sd del socket: "  << sock.sd << "\n";
    ssize_t bytes = ::sendto(sock.sd, obj.data(), SerializableObject::SIZE, 0, &sock.sa, sock.sa_len);

    if ( bytes <= 0 )
    {
        printf("Error en el send: %s\n",strerror(errno));
        return -1;
    }
    return 0;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    if (s1.sa.sa_family != s2.sa.sa_family) return false;
    if (s1.sa.sa_data != s2.sa.sa_data) return false;
    return true;
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

