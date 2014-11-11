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

int main()
{
  //Initilize the logger
  openlog("calc-server", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(LOG_INFO));

  message_t* message;    //Message read from the client
  host_t client;         //Client's address
  int socket_descriptor; //Descriptor for the socket to listen on
  int listen_timeout;    //Milliseconds to check for data before timing out
  int listen_result;     //Ther result of listening for a message on a socket

  //Create a socket to listen on port 5000
  create_server_socket("5000", &socket_descriptor);

  //Create space for the received message
  message = create_message();
  //Set timeout to 10 seconds
  listen_timeout = 10000;
  //Check for a message for the specified timeout duration
  listen_result = recieve_message_with_timeout(socket_descriptor, &client, listen_timeout, message);
  if (listen_result == 0)
  {
    message->buffer[message->length] = '\0';
    printf("Message received from %s: %s\n", client.friendly_ip, message->buffer);
  }
  else if (listen_result == 1)
  {
    printf("No message received in 10s. Exiting.\n");
  }
  free(message);
  close(socket_descriptor);
  return 0;
}