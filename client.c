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
#include "src/net/udp/udp_client.h"
#include "calculator.h"

//Creates the calculator message to be sent
message_t* create_calc_request(uint16_t operand1_in, uint16_t operand2_in, uint16_t operand3_in, uint16_t operand4_in)
{
  //Create 16 byte message
  calc_message_t* message = (calc_message_t*) create_message();
  //Store the operand count in the first byte of the message
  message->operand_count = 4;
  //Convert operands to network order and store them in message
  message->operands[0] = htons(operand1_in);
  message->operands[1] = htons(operand2_in);
  message->operands[2] = htons(operand3_in);
  message->operands[3] = htons(operand4_in);
  //Set the length of the message
  message->length = 16;
  //Return the message
  return (message_t*)message;
}

int main()
{
  //Initilize the logger
  openlog("calc-client", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
  setlogmask(LOG_UPTO(LOG_DEBUG));

  host_t server;            //Address of the server
  calc_message_t* response; //Response returned by the server
  int sockfd;               //Location to store the created socket file descriptor
  //Create socket to listen on port 5000
  create_client_socket("localhost", "5000", &server, &sockfd);
  //Encode the message to be sent
  message_t* request = create_calc_request(10, 20, 30, 40);
  //Send the 16 byte request to the server and free it's memory
  send_message(sockfd, request, &server);
  free(request);
  //Read response from server
  response = (calc_message_t*)create_message();
  receive_message(sockfd, &server, (message_t*)response);
  //Convert the sum to host order
  response->sum = ntohl(response->sum);
  //Print result and close the socket
  printf("Result: %d\n", response->sum);
  free(response);
  close(sockfd);
  return 0;
}