#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>


int main (int argc, char* argv[]) {
    struct addrinfo  hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family= AF_UNSPEC;
    hints.ai_socktype= SOCK_DGRAM;
    
    int rc = getaddrinfo(argv[1], argv[2], NULL, &result);
    if(rc != 0){
        printf("Error getaddrinfo: %s\n", gai_strerror(rc));
        return -1;
    }      

    struct addrinfo *it;
    for (it=result; it!=NULL; it = it->ai_next){
        char host [NI_MAXHOST];
        getnameinfo(it->ai_addr, it->ai_addrlen, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if(host != "\0")
            printf("IP: %s\tFamilia: %d\tTipo: %d\n", host, it->ai_family, it->ai_socktype);
    }

    return 0;
}