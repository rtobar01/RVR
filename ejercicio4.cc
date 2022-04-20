#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

const size_t MAX_BUFFER = 80;

int main (int argc, char* argv[]) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family   = AF_UNSPEC; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* result;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);
    if(rc == -1){
        std::cerr<< "ERROR en getaddrinfo\n";
        return -1;
    }
    
    int sd = socket(result->ai_family, result->ai_socktype, 0);
    bind(sd, (struct sockaddr *) result->ai_addr, result->ai_addrlen);
    listen(sd,5);

    freeaddrinfo(result);

    char host [NI_MAXHOST];
    char serv [NI_MAXSERV];
    char buffer[MAX_BUFFER]; 

    struct sockaddr cliente;
    socklen_t cliente_len;
    int cliente_sd = accept(sd, (struct sockaddr *) &cliente, &cliente_len);
    getnameinfo(&cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);


    
    while (1) {    
        ssize_t received = recv(cliente_sd, (void *) buffer, (MAX_BUFFER-1)* sizeof(char), 0);
        send(cliente_sd, (void*) buffer, received, 0);
    }
    close(cliente_sd);
    close(sd);
}
