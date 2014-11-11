
#include "udp_server.h"

int yes = 1;

//Attempts to bind a socket file descriptor to one of the specified addresses
int bind_socket(struct addrinfo* address_list_in, int* sockfd_location_in)
{
  syslog(LOG_INFO, "Attempting to bind socket");
  struct addrinfo* address_iterator; //Pointer for iterating through the input sockaddr list
  int socket_descriptor;             //The descriptor for our socket file
  //Iterate through the inout sockaddr list until we find on1e that is bindable
  for (address_iterator = address_list_in; address_iterator != NULL; address_iterator = address_iterator->ai_next)
  {
    //Open a socket with the current information
    socket_descriptor = socket(address_iterator->ai_family, address_iterator->ai_socktype, address_iterator->ai_protocol);
    //Try the next address in the list if we were unable to open a socket
    if (socket_descriptor == -1)
    {
      syslog(LOG_DEBUG, "DEBUG: socket() returned -1, getting socket file descriptor has failed.");
      continue;
    }
    //Free the socket if this application has it locked
    if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      syslog(LOG_ERR, "Failed to free socket.");
      return -1;
    }
    //Attempt to bind the socket to the address and port specified
    if (bind(socket_descriptor, address_iterator->ai_addr, address_iterator->ai_addrlen) == -1)
    {
      syslog(LOG_DEBUG, "DEBUG: bind() returned -1, binding port has failed.");
      //If binding fails close the socket and try the next address
      close(socket_descriptor);
      continue;
    }
    else
    {
      //If we reach here the socket is successfully bound
      break;
    }
  }
  //Free the memory that was allocated to our address list
  freeaddrinfo(address_list_in);
  //If we reached the end of the list, we tried every connection with no success
  if (address_iterator == NULL)
  {
    syslog(LOG_ERR, "Binding to socket failed.");
    return -2;
  }
  syslog(LOG_INFO, "Binding socket successful");
  //Store our socket location in the specified location
  *sockfd_location_in = socket_descriptor;
  return 0;  
}

//Attempts to create a server socket on the specified port
int create_server_socket(char* port_in, int* result_location_in)
{
  syslog(LOG_INFO, "Creating new server socket");
  int result_sockfd;        //The socket file descriptor returned for our new socket
  struct addrinfo* results; //The connection information for our socket
  //Get a list of connections we can open on
  if (get_udp_sockaddr(NULL, port_in, AI_PASSIVE, &results) != 0)
  {
    //If failure occurs log it and terminate
    syslog(LOG_ERR, "Failed to create server socket");
    return -1;
  }
  //Open a socket and bind to it
  if (bind_socket(results, &result_sockfd) != 0)
  {
    //If binding fails log it and terminate
    syslog(LOG_ERR, "Failed to create server socket");
    return -2;
  }
  syslog(LOG_INFO, "Successfully created new server socket");
  *result_location_in = result_sockfd;
  return 0;
}