#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>

int main(int argc, char* argv[]) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family = AF_UNSPEC; // IPv4
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo* result;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (rc == -1) {
        std::cerr << "ERROR en getaddrinfo\n";
        return -1;
    }

    int sd = socket(result->ai_family, result->ai_socktype, 0);
    bind(sd, (struct sockaddr*)result->ai_addr, result->ai_addrlen);

    freeaddrinfo(result);

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    char buffer[80];

    while (1) {
        struct sockaddr cliente;
        socklen_t cliente_len;

        ssize_t bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr*)&cliente, &cliente_len);

        getnameinfo(&cliente, cliente_len, host, NI_MAXHOST,
            serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << "Conexión desde Host: " << host << "\tPuerto: " << serv << "\n";
        std::cout << "\tMensaje(" << bytes << "):" << buffer << "\n";


        getnameinfo(&cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << bytes << " de " << host << "::" << serv << "\n";
        if (buffer[0] == 'q')
        {
            break;
        }
        char outstr[200];
        time_t t;
        struct tm* tmp;
        t = time(NULL);
        tmp = localtime(&t);
        int timeSize = 0;
        if (buffer[0] == 't')
        {
            timeSize = strftime(outstr, 200, "%R", tmp);
            sendto(sd, outstr, timeSize, 0, &cliente, cliente_len);
        }
        else if (buffer[0] == 'd')
        {
            timeSize = strftime(outstr, 200, "%D", tmp);
            sendto(sd, outstr, timeSize, 0, &cliente, cliente_len);
        }
        if (timeSize == -1) return -1;
    }
    return 0;
}
