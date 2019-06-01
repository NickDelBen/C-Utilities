/******************************************************
 * Contains useful functions for socket programming with UDP.
 *
 * Created by: Nick DelBen
 * Created on: November 10, 2014
 *
 * To retrieve socket parameters needed for a server that is to listen on port 5000:
 *   get_udp_sockaddr(NULL, "5000", AI_PASSIVE)
 * To get socket parameters for client communicating with server on port 5000 at example.com:
 *   get_udp_sockaddr("example.com", "5000", 0)
 ******************************************************/

#ifndef H_UDP_SOCKETS
#define H_UDP_SOCKETS

#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>

//Maximum amount of data in each UDP datagram
#define UDP_MSS 65535

//Defines a message to be received or sent.
typedef struct
{
  int length;
  uint8_t buffer[UDP_MSS];
} message_t;

//Stores the source of a message
typedef struct
{
  struct sockaddr_in address;        //Compact binary address
  socklen_t address_length;          //The length of the compact binary address
  char friendly_ip[INET_ADDRSTRLEN]; //Human-readable IP address
} host_t;

//Gets the connection information for a udp socket on the specified location
//Stores the result in a specified lcoation
//Parameters:
//  node_in            the address of the host we wish to connect to
//  port_in            the port of the host we wish to connect to
//  flags_in           allows for the function to be used by a server and client
//                     client would send 0
//                     server would send AI_PASSIVE
//  result_location_in memory location to store the result sockaddr
//Returns 0 if successful.           
int get_udp_sockaddr(const char* node_in, const char* port_in, int flags_in, struct addrinfo** result_location_in);

//Creates and returns a pointer to a new new message structure
message_t* create_message();

//Receives a message and stores the result in the specified location
//Parameters:
//  socket_descriptor_in the file descriptor of the socket from which to read the message
//  source_in            stores the source of the message when the function returns
//  result_location_in   stores the contents of the message when the function returns
//Returns 0 if successful
int receive_message(int socket_descriptor_in, host_t* source_in, message_t* message_location_in);

//Recieves a message and stores the result in the specified location times out after waiting specified time
//Parameters:
//  socket_descriptor_in the file descriptor of the socket from which to read the message
//  source_in            stores the source of the message when the function returns
//  timeout_in           number of milliseconds to wait before timing out
//  result_location_in   stores the contents of the message when the function returns
//Return 0 on success
//Return 1 on timeout
int recieve_message_with_timeout(int socket_descriptor_in, host_t* source_in, int timeout_in, message_t* message_location_in);

//Sends a message to the specified recepient
//Parameters:
//  socket_descriptor_in the socket to send the message over
//  message_in           the message to be sent
//  destination_in       the intended recepient of the message
//Returns 0 on success
int send_message(int socket_descriptor_in, message_t* message_in, host_t* destination_in);

#endif