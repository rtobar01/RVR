#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>




int main (int argc, char* argv[]) {
    struct addrinfo hints;
    hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family   = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *result;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);
    int sd = socket(result->ai_family, result->ai_socktype, 0);

    bind(sd, (struct sockaddr *) result->ai_addr, result->ai_addrlen);
    char c = ' ';
    while (1) {
        socklen_t cliente_len;
        struct sockaddr* cliente;
 
        char host [NI_MAXHOST];
        char serv [NI_MAXSERV];
       
        char buffer [80]; 
        ssize_t bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr *) &cliente, &cliente_len);
        if(bytes==2){
            if(buffer[0] != 'd' && buffer[0] != 't' && buffer[0] != 'q'){
                std::cout<< "Comando no soportado\n";
                continue;
            }
           
            getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
            serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);
            std::cout<< bytes<<" de "<<host<<"::"<<serv<<"\n";
            char *respuesta;
            if(buffer[0]=='q'){
                char outstr[200];
                time_t t;
                struct tm *tmp;

                t = time(NULL);
                tmp = localtime(&t);

                int format = '%k';
                // strftime(outstr, sizeof(outstr), format, tmp);
                // std::cout<<tmp->tm_sec;
            }
            char c[6]= "nada";
            sendto(sd, c, 100, 0, (struct sockaddr *) &cliente, cliente_len);
            
        }
    }
}
