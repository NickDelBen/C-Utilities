//Contains useful functions for socket programming with UDP.

#include "udp_sockets.h"

//Gets the connection information for a udp socket on the specified location
int get_udp_sockaddr(const char* node_in, const char* port_in, int flags_in, struct addrinfo** result_location_in) {
  syslog(LOG_INFO, "Requesting UDP address information for Node: %s, Port: %s, with Flags: %d", node_in, port_in, flags_in);
  struct addrinfo hints;         //The initial information used to get the result addrinfo
  int result_state;              //The result state of our getaddrinfo
  struct addrinfo* local_result; //Location to store the result of our address lookup

  syslog(LOG_DEBUG, "get_udp_sockaddr: Setting up hints struct for getaddrinfo");
  //Ensure the hints location has to existing information
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;      //Get socket addresses for local IPv4 addresses
  hints.ai_socktype = SOCK_DGRAM; //Return udp socket addresses
  hints.ai_flags = flags_in;      //The addresses we get should be listening sockets
  
  syslog(LOG_DEBUG, "get_udp_sockaddr: Getting connection information from getaddrinfo()");
  //Get the results based on our pre-configured hints addrinfo
  result_state = getaddrinfo(node_in, port_in, &hints, &local_result);

  //Check that the infor was successfully retrieved
  if (result_state != 0)
  {
    syslog(LOG_ERR, "There was an error requesting the socket information.");
    return result_state;
  }

  syslog(LOG_DEBUG, "get_udp_sockaddr: Storing result in input location");
  //Set the return value to our local result
  *result_location_in = local_result;
  syslog(LOG_INFO, "Successfully retrieved address information");
  return 0;
}

//Creates and returnsa new message structure
message_t* create_message()
{
  syslog(LOG_INFO, "Creating new message");
  return (message_t*) malloc(sizeof(message_t));
}

//Receives a message and stores the result in the specified location
int receive_message(int socket_descriptor_in, host_t* source_in, message_t* message_location_in)
{
  syslog(LOG_INFO, "Attempting to receive message");
  //Length of the remote ip structure
  syslog(LOG_DEBUG, "receive_message: Attempting to set length of source address");
  source_in->address_length = sizeof(source_in->address);
  //Read the message storing sender in source_in and message in message_location_in
  syslog(LOG_DEBUG, "receive_message: Listening for message on socket");
  message_location_in->length = recvfrom(socket_descriptor_in, message_location_in->buffer, 
    sizeof(message_location_in->buffer), 0, (struct sockaddr*)&source_in->address, &source_in->address_length);
  //If no message was read display error and return failure
  if (message_location_in->length == 0)
  {
    syslog(LOG_ERR, "No message received.");
    return -1;
  }
  //Convert source address to human readable form and store it
  inet_ntop(source_in->address.sin_family, &source_in->address.sin_addr, source_in->friendly_ip, sizeof(source_in->friendly_ip));
  syslog(LOG_INFO, "Successfully received message");
  return 0;
}

//Recieves a message and stores the result in the specified location times out after waiting specified time
int recieve_message_with_timeout(int socket_descriptor_in, host_t* source_in, int timeout_in, message_t* message_location_in)
{
  syslog(LOG_INFO, "Attempting to receive message with timeout");
  //We will poll the input socket to see if data has been received
  struct pollfd poll_descriptor; //The location to store the poll file descriptor
  int poll_result_state;         //The return value of our poll
  int recieve_result_state;      //The result state from recieving the message on successful poll
  poll_descriptor.fd = socket_descriptor_in;
  poll_descriptor.events = POLLIN;

  syslog(LOG_DEBUG, "recieve_message_with_timeout: Polling specified socket for new data");
  //Poll the socket for the specified time
  poll_result_state = poll(&poll_descriptor, 1, timeout_in);
  if (poll_result_state == -1)
  {
    syslog(LOG_ERR, "There was an error when polling the socket");
    return -1;
  }
  if (poll_result_state == 0)
  {
    syslog(LOG_INFO, "Specified timeout was reached. No message received.");
    return 1;
  }
  if (receive_message(socket_descriptor_in, source_in, message_location_in) != 0)
  {
    syslog(LOG_ERR, "There was an error when receiving the message");
    return -2;
  }
  syslog(LOG_INFO, "Message successfully received before timeout");
}

//Sends a message to the specified recepient
int send_message(int socket_descriptor_in, message_t* message_in, host_t* destination_in)
{
  syslog(LOG_INFO, "Attempting to send message to %s", destination_in->friendly_ip);
  int send_result; //Location to store the result of sending a message

  syslog(LOG_DEBUG, "send_message: Sending message to specified recepiant");
  //Send the message to the specified destination
  send_result = sendto(socket_descriptor_in, message_in->buffer, message_in->length, 0, (struct sockaddr*)&destination_in->address, destination_in->address_length);
  //If there is an error log the error
  if (send_result == -1)
  {
    syslog(LOG_ERR, "There was an error sending the message to %s", destination_in->friendly_ip);
  } else {
    syslog(LOG_INFO, "Successfully delivered message to %s", destination_in->friendly_ip);
  }
  return send_result;
}