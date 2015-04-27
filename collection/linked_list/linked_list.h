/***********************************************************
 * Double linked list. That is each node has a link to the 
 * previous and next node.
 *
 * Created By: Nick DelBen
 * Created On: January 14, 2015
 *
 * Last Modified: January 14, 2014
 *   - Created Initially
 **********************************************************/

#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <syslog.h>
#include <stdlib.h>

#include "list_node.h"

//The structure for a linked list
typedef struct {
  struct list_node_t * first; //The first node of the list (head)
  struct list_node_t * last;  //The last node in the list (tail)
  int count;                  //The number of elements in the list
} linked_list_t;

//Creates a new list and stores pointer to list in specified location
//DO NOT FORGET TO FREE THE MEMORY USING free_list() when done
//Parameters
//  location_in the memory location to store the new list
//Return
//  0 success
//  1 on failure
int new_list(linked_list_t ** location_in);

//Frees all the memory associated with a linked list
//Parameters:
//  list_in the list to be freed
//Return
//  0 success
int free_list(linked_list_t * list_in);

//Adds an element to the front of the list
//Parameters
//  list_in the list to add the item to
//  data_in the data to put in the new node
//Return
//  0 success
//  1 failed to add new node
int add_front(linked_list_t * list_in, void * data_in);

//Adds an element to the back of the list
//Parameters
//  list_in the list to add the item to
//  data_in the data to put in the new node
//Return
//  0 success
int add_back(linked_list_t * list_in, void * data_in);

//Removes the first item from the list
//Parameters
//  list_in the lsit to remove the item from
//Return
//  0 successful
int remove_front(linked_list_t * list_in);

//Removes the last item from the list
//Parameters
//  list_in the lsit to remove the item from
//Return
//  0 successful
int remove_back(linked_list_t * list_in);

#endif