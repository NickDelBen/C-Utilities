//Contains the node for a double linked list

#include "list_node.h"

//Creates a new node and stores a pointer to the node in the specified location
int new_list_node(struct list_node_t * previous_in, struct list_node_t * next_in, void * data_in, struct list_node_t ** location_in) {
  syslog(LOG_DEBUG, "Creating new list node.");
  
  //Allocate memory for the new npde
  struct list_node_t * new_node = (struct list_node_t *) malloc(sizeof(struct list_node_t));
  new_node->data = data_in;
  new_node->previous = previous_in;
  new_node->next = next_in;

  //If this node had a previous node, link them
  if (previous_in != NULL) {
    previous_in->next = new_node;
  }

  //If this node has a next node, link them
  if (next_in != NULL) {
    next_in->previous = new_node;
  }
  
  //Set the result location to point to the newly created node
  *location_in = new_node;

  return 0;
}

//Frees the memory allocated for the node to avoid memory leaks
int free_list_node(struct list_node_t * node_in) {
  syslog(LOG_DEBUG, "Freeing memory for node");

  //Free the data associated with the node
  free(node_in->data);

  free(node_in);

  return 0;
}