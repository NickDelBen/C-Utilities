/*************************************************************************
 * Contains functions useful for makinf tcp connections
 *
 * Author: Nick DelBen
 ************************************************************************/

#ifndef __H_CONNECTION
#define __H_CONNECTION

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int yes = 1;

//Largest port a user can specify
#define MAX_PORT_NUMBER 65535
//Smallest port a user can specify
#define MIN_PORT_NUMBER 0
//Specifies end of string terminator
#define STRING_TERMINATOR '\0'
#define NUMBER_BASE_10 10

/**************************
* Checks if the specified port is valid to open a socket on.
* @param port_in - string containing port to be checked.
* @return true  - is valid port
* @return false - is not valid port
**************************/
bool valid_port(char *port_in);

//Returns the connection information for the specified port
struct addrinfo* get_server_sockaddr(const char* port_in);

//Binds the specified socket file descriptor, stores the file descriptor in the memory location of sock_descriptor_location_in
//Return -1 - Binding was unsuccessful
//Return 0 - Binding was succesfull
int bind_socket(struct addrinfo* address_list_in, int *sock_descriptor_location_in);

//Waits for an incoming connection on the specified port;
int wait_for_connection(int socket_descriptor_in, int *result_descriptor_location);

//Returns the connection information for the specified port and addres
struct addrinfo* get_client_sockaddr(const char* hostname_in, const char* port_in);

int open_connection(struct addrinfo* address_list_in, int *socket_descriptor_location_in);

#endif