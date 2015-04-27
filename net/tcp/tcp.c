//Contains functions useful for makinf tcp connections
#include "tcp.h"

//Checks if the specified port is valid to open a socket on.
bool valid_port(char *port_in)
{
  //Location to store additional characters following input (see strtol()).
  char *extra;
  //Location to store the converted string (see strtol())
  long result;
  //Attempt to convert the input string to a long int
  result = strtol(port_in, &extra, NUMBER_BASE_10);  
  //Validate converted string to ensure is a valid port.
  if (extra == port_in)
  {
    //If nothing was extracted, input was not decimal.
    syslog(LOG_ERR, "Invalid port '%s': No port number specified.", port_in);
    return false;
  } 
  if (*extra != STRING_TERMINATOR)
  {
    //If the only trailing input characters were not the end string, input had additional data.
    syslog(LOG_ERR, "Invalid port '%s': Non-nuemeric port specified.", port_in);
    return false;
  } 
  if (result > MAX_PORT_NUMBER || result < MIN_PORT_NUMBER)
  {
    //Make sure the specified port number is not outside range of allowed ports.
    syslog(LOG_ERR, "Invalid port '%s': Port outside range of valid ports (%u-%u)\n", port_in, MIN_PORT_NUMBER, MAX_PORT_NUMBER);
    return false;
  }
  return true;
}

//Returns the connection information for the specified port
struct addrinfo* get_server_sockaddr(const char* port_in)
{
  struct addrinfo hints;   //The known data to be used when attempting to get a connection
  struct addrinfo* result; //The discovered connection information
  int check_value; //The result of the lookup
  //Zero the location in memory for the hints information
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family   = AF_INET;     //Specify ipv4 (would be AF_INET6 for ipv6)
  hints.ai_socktype = SOCK_STREAM; //SOCK_STREAM or SOCK_DGRAM
  hints.ai_flags    = AI_PASSIVE;  //Use the local ip address
  check_value = getaddrinfo(NULL, port_in, &hints, &result);
  if (check_value != 0)
  {
    return NULL;
  }
  return result;
}

//Binds the specified socket file descriptor, stores the file descriptor in the memory location of sock_descriptor_location_in
int bind_socket(struct addrinfo* address_list_in, int *sock_descriptor_location_in)
{
  struct addrinfo* address_iterator; //Iterator for looking through connections
  int socket_descriptor; //The file descriptor for the socket
  //Loop through the input addresses and find a working entry
  for (address_iterator = address_list_in; address_iterator != NULL; address_iterator = address_iterator->ai_next)
  {
    //Open a new socket
    socket_descriptor = socket(address_iterator->ai_family, address_iterator->ai_socktype, address_iterator->ai_protocol);
    //Try the next address in the list of recieved addresses
    if (socket_descriptor == -1)
    {
      continue;
    }
    //Free the socket if this application has it locked
    if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      return -1;
    }
    //Attempt to bind on a successfully found socket
    if (bind(socket_descriptor, address_iterator->ai_addr, address_iterator->ai_addrlen) == -1)
    {
      //Close the socket and contiune to the next connection info if binding fails
      close(socket_descriptor);
      continue;
    }
    else
    {
      //We have succsssfully bound to the socket we can stop parsing
      break;
    }
  }
  //Free the address list from memory.
  freeaddrinfo(address_list_in);
  //If address is NULL we went through whole list without much success
  if (address_iterator == NULL)
  {
    return -1;
  }
  *sock_descriptor_location_in = socket_descriptor;
  return 0;
}

//Waits for an incoming connection on the specified port;
int wait_for_connection(int socket_descriptor_in, int *result_descriptor_location)
{
  struct sockaddr_in client_address;               //Incoming client
  socklen_t address_length = sizeof(struct sockaddr_in); //Length of the remote IP structure
  char ip_address[INET_ADDRSTRLEN];                //Buffer to store human-friendly IP address
  int connection_descriptor;                       //Socket file descriptor for the new connection
  //Sit and listen until a new connection occurs
  connection_descriptor = accept(socket_descriptor_in, (struct sockaddr*)&client_address, &address_length);
  //Make sure the connection was established successfully
  if (connection_descriptor == -1)
  {
    //Return error
    return -1;
  }
  //Convert the connecting IP to a human-friendly form and print   it
  inet_ntop(client_address.sin_family, &client_address.sin_addr, ip_address, sizeof(ip_address));
  //Log the connection information to the user
  syslog(LOG_INFO, "Request received from client %s\n", ip_address);
  //Set the found fd in the passed memory location
  *result_descriptor_location = connection_descriptor;
  return 0;
}

//Returns the connection information for the specified port and addres
struct addrinfo* get_client_sockaddr(const char* hostname_in, const char* port_in)
{
  struct addrinfo hints;   //The known data to be used when attempting to get a connection
  struct addrinfo* result; //The discovered connection information
  int check_value; //The result of the lookup
  //Zero the location in memory for the hints information
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family   = AF_INET;     //Specify ipv4 (would be AF_INET6 for ipv6)
  hints.ai_socktype = SOCK_STREAM; //SOCK_STREAM or SOCK_DGRAM
  check_value = getaddrinfo(hostname_in, port_in, &hints, &result);
  if (check_value != 0)
  {
    return NULL;
  }
  return result;
}

int open_connection(struct addrinfo* address_list_in, int *socket_descriptor_location_in)
{
  struct addrinfo* address_iterator; //Iterator for looking through connections
  int socket_descriptor; //Socket opened for communication to server
  //Look through list of addresses until we find a valid address to connect to
  for (address_iterator = address_list_in; address_iterator != NULL; address_iterator = address_iterator->ai_next)
  {
    //Attempt to open a socket with curret information
    socket_descriptor = socket(address_iterator->ai_family, address_iterator->ai_socktype, address_iterator->ai_protocol);
    //Try next address in list if socket will not open
    if (socket_descriptor == -1)
    {
      continue;
    }
    //Upon successful connection, stop looking through list of connections.
    if (connect(socket_descriptor, address_iterator->ai_addr, address_iterator->ai_addrlen) != -1)
    {
      break;
    }
  }
  //Free list of iterators
  freeaddrinfo(address_list_in);
  //If no valid connection information was found, return failure
  if (address_iterator == NULL)
  {
    return -1;
  }
  *socket_descriptor_location_in = socket_descriptor;
  return 0;
}