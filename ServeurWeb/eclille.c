
#include "eclille.h"

#define _BSD_SOURCE // inet_aton

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

int eclille_createSocket()
{
    // create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("eclille_createSocket: socket failed");
        return -1;
    }

    fprintf(stderr, "eclille_createSocket: creating socket (pid %d, socket %d)\n", getpid(), sock);

    return sock;
}

int eclille_openConnection(int client_sock, const char *server_ip, int server_port)
{
    int ret;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;

    // init server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_aton(server_ip, &server_addr.sin_addr);

    // connect to server
    ret = connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1)
    {
        perror("eclille_openConnection: connect failed");
        return -1;
    }

    // get client information
    client_addr_size = sizeof(client_addr);
    ret = getsockname(client_sock, (struct sockaddr *) &client_addr, &client_addr_size);
    if( ret == -1)
    {
        perror("eclille_openConnection: getsockname failed");
        return -1;
    }

    // display connection state
    fprintf(stderr, "eclille_openConnection: requesting a connection to (server_ip=%s, server_port=%d)\n",
            inet_ntoa(server_addr.sin_addr),  ntohs(server_addr.sin_port));
    fprintf(stderr, "\t from (client_ip=%s, client_port=%d)\n",
            inet_ntoa(client_addr.sin_addr),  ntohs(client_addr.sin_port));
    fprintf(stderr, "\t (client_pid %d, client_socket %d)\n", getpid(), client_sock);

    return 0;
}

int eclille_testConnection(const char *server_ip, int server_port)
{
    int ret;
    struct sockaddr_in server_addr;

    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return 0;
    }

    // init server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_aton(server_ip, &server_addr.sin_addr);

    // connect to server
    ret = connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1)
    {
        return 0;
    }
    else
    {
        close(sock);
        return 1;
    }
}

int eclille_sendAndReceive(const char *server_ip, int server_port, const char * send_buffer, char * receive_buffer, int buffer_size)
{
    int ret;

    int sock = eclille_createSocket();
    if (sock < 0) return -1;

    ret = eclille_openConnection(sock, server_ip, server_port);
    if (ret != 0) return -1;

    ret = eclille_sendToSocket(sock, send_buffer);
    if (ret != 0) return -1;

    ret = eclille_receiveFromSocket(sock, receive_buffer, buffer_size);
    if (ret != 0) return -1;

    ret = eclille_closeConnection(sock);
    if (ret != 0) return -1;

    return 0;
}


int eclille_closeConnection(int sock)
{
    int ret;

    // close the socket
    ret = close(sock);
    if (ret == -1)
    {
        fprintf(stderr, "eclille_closeConnection: close failed\n");
        return -1;
    }

    fprintf(stderr, "eclille_closeConnection: closing connection (pid %d, socket %d)\n", getpid(), sock);
    return 0;
}

int eclille_sendToSocket(int sock, const char *msg)
{
    // write msg into sock
    int data_written = write(sock, msg, strlen(msg));
    if (data_written != strlen(msg))
    {
        perror("eclille_sendToSocket: write failed");
        return -1;
    }

    fprintf(stderr, "eclille_sendToSocket: sending [%s] (pid %d, socket %d)\n", msg, getpid(), sock);
    return 0;
}

int eclille_sendBinaryToSocket(int sock, const char *msg, int size)
{
    // write msg into sock
    int data_written = write(sock, msg, size);
    if (data_written != size)
    {
        perror("eclille_sendBinaryToSocket: write failed");
        return -1;
    }

    fprintf(stderr, "eclille_sendBinaryToSocket: sending [%d] (pid %d, socket %d)\n", size, getpid(), sock);
    return 0;
}

int eclille_receiveFromSocket(int sock, char *msg, int size)
{
    // read msg from sock
    int data_read = read(sock, msg, size);
    if (data_read == -1)
    {
        perror("eclille_receiveFromSocket: read failed");
        return -1;
    }
    msg[data_read] = '\0';

    fprintf(stderr, "eclille_receiveFromSocket: receiving [%s] (pid %d, socket %d)\n", msg, getpid(), sock);
    return 0;
}

int eclille_startServer(int server_sock, const char *server_ip, int server_port)
{
    int ret;
    struct sockaddr_in server_addr;

    // init server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_aton(server_ip, &server_addr.sin_addr);

    // bind address to a server socket
    ret = bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1)
    {
        perror("eclille_startServer: bind failed");
        return -1;
    }

    // listen for client connections
    ret = listen(server_sock, 2);
    if (ret == -1)
    {
        perror("eclille_startServer: listen failed");
        return -1;
    }

    fprintf(stderr, "eclille_startServer: starting server (server_ip %s, server_port %d) (server_pid %d, server_socket %d)\n",
            inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), getpid(), server_sock);
    return 0;
}

int eclille_waitConnection(int server_sock)
{
    struct sockaddr_in client_addr;

    socklen_t client_addr_size = sizeof(client_addr);

    // create a socket for a client connection
    int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
    if (client_sock == -1)
    {
        perror("eclille_waitConnection: accept failed");
        return -1;
    }

    fprintf(stderr, "eclille_waitConnection: requesting connection (client_ip %s, client_port %d) (server_pid %d, server_socket %d, client_socket %d)\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), getpid(), server_sock, client_sock);

    return client_sock;
}