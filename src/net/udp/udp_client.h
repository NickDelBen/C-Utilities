/******************************************************
 * Contains useful functions for creating a UDP client.
 *
 * Created by: Nick DelBen
 * Created on: November 10, 2014
 ******************************************************/

#ifndef H_UDP_CLIENT
#define H_UDP_CLIENT

#include <string.h>
#include <syslog.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "udp_sockets.h"

//Attempts to create a client socket on the specified port
//Stores the result in the specified location
//Parameters:
//  hostname_in        the address of the host we are connecting to
//  port_in            the port the destination server is listening on
//  server_in          the location to will pack the server information on function return
//  result_location_in the location to store the result file descriptor
//Return 0 on success
int create_client_socket(char* hostname_in, char* port_in, host_t* server_in, int* result_location_in);

#endif