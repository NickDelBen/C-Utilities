//Contains useful functions for creating a UDP client.

#include "udp_client.h"

//Attempts to create a client socket on the specified port
int create_client_socket(char* hostname_in, char* port_in, host_t* server_in, int* result_location_in)
{
  syslog(LOG_INFO, "Creating new client socket");
  int socket_descriptor;             //The socket file descriptor returned for our new socket
  int result_state;                  //The result of calling our functions
  struct addrinfo* address_iterator; //For looping through results
  struct addrinfo* results;          //Location to store the results
  //Get the connection information for the specified connection
  result_state = get_udp_sockaddr(hostname_in, port_in, 0, &results);
  if (result_state != 0)
  {
    syslog(LOG_ERR, "Failed to create server socket");
    return -1;
  }  
  //Iterate through all the results until a socket is successfully created
  for (address_iterator = results; address_iterator != NULL; address_iterator = address_iterator->ai_next)
  {
    //Open a socket
    socket_descriptor = socket(address_iterator->ai_family, address_iterator->ai_socktype, address_iterator->ai_protocol);
    //Try the next address if failed to open socket
    if (socket_descriptor == -1)
    {
      syslog(LOG_DEBUG, "DEBUG: socket() returned -1, getting socket file descriptor has failed.");
      continue;
    }
    //Socket has been successfully created
    break;
  }
  //Copy server information to the output location
  memcpy(&server_in->address, address_iterator->ai_addr, address_iterator->ai_addrlen);
  memcpy(&server_in->address_length, &address_iterator->ai_addrlen, sizeof(address_iterator->ai_addrlen));
  //Store the human readable version of the ip address
  inet_ntop(server_in->address.sin_family, &server_in->address.sin_addr, server_in->friendly_ip, sizeof(server_in->friendly_ip));
  //Free the memory allocated for the address list
  freeaddrinfo(results);
  //If we failed to create socket log this and return
  if (address_iterator == NULL)
  {
    syslog(LOG_ERR, "Binding to socket failed.");
    return -2;
  }
  *result_location_in = socket_descriptor;
  syslog(LOG_INFO, "Successfully created new client socket");
  return 0;
}