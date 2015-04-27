//Double linked list. That is each node has a link to the previous and next node.

#include "linked_list.h"

//Creates a new list and stores pointer to list in specified location
int new_list(linked_list_t ** location_in) {
  syslog(LOG_DEBUG, "Creating new linked list.");

  linked_list_t * list;

  //Allocate heap memory for the new linked list
  if ((list = (linked_list_t *) malloc(sizeof(linked_list_t))) == NULL) {
    return 1;
  }

  //Set the default values of the linked list
  list->first = NULL;
  list->last = NULL;
  list->count = 0;

  //Set the result value
  *location_in = list;

  return 0;
}

//Frees all the memory associated with a linked list
int free_list(linked_list_t * list_in) {
  syslog(LOG_DEBUG, "Freeing linked list of size '%d'", list_in->count);

  struct list_node_t * current_node = list_in->first;
  struct list_node_t * next_node;

  //Free the nodes in the list
  while (current_node != NULL) {
    next_node = current_node->next;
    syslog(LOG_DEBUG, "Freeing a node");
    free_list_node(current_node);
    current_node = next_node;
  }    

  //Free the list controller
  free(list_in);

  return 0;
}

//Adds an element to the front of the list
int add_front(linked_list_t * list_in, void * data_in) {
  syslog(LOG_DEBUG, "Adding element to front of list");

  struct list_node_t * new_node;

  //Create a new node that will hold the specified data
  if (new_list_node(NULL, list_in->first, data_in, &new_node) != 0) {
    return 1;
  }

  //Set the first element of the list to be the new node
  list_in->first = new_node;
  
  //If the list is empty the first element is the last element
  if (list_in->last == NULL) {
    list_in->last = new_node;
  }

  //Increment the number of elements in the list
  list_in->count = list_in->count + 1;

  return 0;
}

//Adds an element to the front of the list
int add_back(linked_list_t * list_in, void * data_in) {
  syslog(LOG_DEBUG, "Adding element to back of list");

  struct list_node_t * new_node;

  //Create the new node that will contain the specified data
  if (new_list_node(list_in->last, NULL, data_in, &new_node) != 0) {
    return 1;
  }

  //Set the last element of the list to be the new node
  list_in->last = new_node;
  
  //If the list is empty the last element is the first element
  if (list_in->first == NULL) {
    list_in->first = new_node;
  }

  //Increment the number of elements in the list
  list_in->count = list_in->count + 1;

  return 0;
}

//Removes the first item from the list
int remove_front(linked_list_t * list_in) {
  syslog(LOG_DEBUG, "Removing first element from list");
  
  //New first item in list is old second item in list
  list_in->first = list_in->first->next;

  //Free the old first item from memory to avoid leaks
  free(list_in->first->previous);

  //There is no previous item for the first item in a list
  list_in->first->previous = NULL;

  //Decrement the list element counter
  list_in->count = list_in->count - 1;

  return 0;
}

//Removes the last item from the list
int remove_back(linked_list_t * list_in) {
  syslog(LOG_DEBUG, "Removing last element from list");
  
  //New last item in list is old second last item in list
  list_in->last = list_in->last->previous;

  //Free the old last item from memory to avoid leaks
  free(list_in->last->next);

  //There is no next item for the last item in a list
  list_in->last->next = NULL;

  //Decrement the list element counter
  list_in->count = list_in->count - 1;

  return 0;
}