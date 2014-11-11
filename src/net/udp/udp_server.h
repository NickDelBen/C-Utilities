/******************************************************
 * Contains useful functions for creating a UDP server.
 *
 * Created by: Nick DelBen
 * Created on: November 10, 2014
 ******************************************************/

#ifndef H_UDP_SERVER
#define H_UDP_SERVER

#include <string.h>
#include <syslog.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "udp_sockets.h"

//Attempts to bind a socket file descriptor to one of the specified addresses
//Stores the result in a specified location
//Parameters:
//  address_list_in    the sockaddr list to try and bind to
//  sockfd_location_in the location to store the result socket file descriptor
//Returns 0 on success
int bind_socket(struct addrinfo* address_list_in, int* sockfd_location_in);

//Attempts to create a server socket on the specified port
//Stores the result in the specified location
//Parameters:
//  port_in            the port we are attempting to bind on
//  result_location_in the location to store the result file descriptor
//Return 0 on success
int create_server_socket(char* port_in, int* result_location_in);

#endif