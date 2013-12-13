
#ifndef _EC_LILLE_H_
#define _EC_LILLE_H_


////////////////////////////////
// client functions
////////////////////////////////

// to connect a socket to a host
int eclille_openConnection(int client_sock, const char *server_ip, int server_port);

// to verify that the host can be reached
int eclille_testConnection(const char *server_ip, int server_port);

// to send a request and receive the corresponding answer
int eclille_sendAndReceive(const char *server_ip, int server_port, const char * send_buffer, char * receive_buffer, int buffer_size);


////////////////////////////////
// server functions
////////////////////////////////

// to bind a socket to an address (name a socket)
int eclille_startServer(int server_sock, const char *server_ip, int server_port);

// to wait for a client connection
int eclille_waitConnection(int server_sock);


////////////////////////////////
// common functions
////////////////////////////////

// to create a socket
int eclille_createSocket();

// to close a connection
int eclille_closeConnection(int sock);

// to send a message to a socket
int eclille_sendToSocket(int sock, const char *msg);

// to send binary data to a socket
int eclille_sendBinaryToSocket(int sock, const char *msg, int size);

// to receive a message from a socket
int eclille_receiveFromSocket(int sock, char *msg, int size);


#endif

