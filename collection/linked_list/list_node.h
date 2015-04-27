/***********************************************************
 * Contains the node for a double linked list
 *
 * Created By: Nick DelBen
 * Created On: January 14, 2015
 *
 * Last Modified: January 14, 2014
 *   - Created Initially
 **********************************************************/

#ifndef _LIST_NODE_H
#define _LIST_NODE_H

#include <syslog.h>
#include <stdlib.h>

//A node in a linked list
struct list_node_t {
  struct list_node_t * previous;
  struct list_node_t * next;
  void * data;
};

//Creates a new node and stores a pointer to the node in the specified location
//DO NOT FOGET TO free() THE NODE WHEN IT IS NO LONGER NEEDED
//Parameters
//  previous_in the previous node this node will link to
//  next_in     the next node this node will link to
//  data_in     the data to be stored in this node
//  location_in the memory location to store the new node
//Return
//  0 successful
int new_list_node(struct list_node_t * previous_in, struct list_node_t * next_in, void * data_in, struct list_node_t ** location_in);

//Frees the memory allocated for the node to avoid memory leaks
//Parameters
//  node_in the node to be freed
//Return
//  0 successful
int free_list_node(struct list_node_t * node_in);

#endif