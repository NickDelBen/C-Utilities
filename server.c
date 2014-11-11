#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <syslog.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "src/net/udp/udp_sockets.h"
#include "src/net/udp/udp_server.h"
#include "calculator.h"

//Function to encode the comuted sum in a response message
message_t* create_response_message(calc_message_t* request_in)
{
  syslog(LOG_INFO, "Creating response message");
  int i;
  //Create response message and initilize it
  calc_message_t* response = (calc_message_t*)create_message();
  response->operand_count = 0;
  response->sum = 0;
  //Compute sum for the clients request
  for(i = 0; i < request_in->operand_count; i++)
  {
    response->sum += ntohs(request_in->operands[i]);
  }
  //Convert the sum to network order
  response->sum = htonl(response->sum);
  //Set response length of the message
  response->length = 8;
  syslog(LOG_INFO, "Successfully created response message");
  return (message_t*)response;
}

int main()
{
  //Initilize the logger
  openlog("calc-server", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(LOG_INFO));

  message_t* request; //Clients request message
  message_t* response;     //Servers response message
  host_t client;           //Clients address
  int socket_descriptor;
  //Create a socket to listen on port 5000

  //Create socket to listen on port 5000
  create_server_socket("5000", &socket_descriptor);

  //Read request message and generate response
  //Read response from server
  request = create_message();
  receive_message(socket_descriptor, &client, request);
  response = create_response_message((calc_message_t*)request);

  //Send request and free memory allocated to messages
  send_message(socket_descriptor, response, &client);

  //Preform program cleanup
  free(request);
  free(response);
  close(socket_descriptor);

  return 0;
}