#include "uthash.h"
#include "unique_elements.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef UNION_H
#define UNION_H

// function to find union and intersection of two hash tables
// returns a linked list containing union elements and updates intersection count
node* union_intersection_set(table* head1, table* head2, int* union_ct, int* intersection_ct){

node* union_head = NULL;
table* temp_entry_checker;
table* current1, *tmp1;
*union_ct = 0;
*intersection_ct = 0;

// check if both tables are empty
if (head1 == NULL && head2 == NULL) {
    printf("Both tables are empty\n");
    return NULL;
}


// iterate over first table and add all elements to union linked list
HASH_ITER(hh, head1, current1, tmp1){
    node* the_node = malloc(sizeof(node));
    // memset(the_node, 0, sizeof(node));
    
    // skip if current node is null or empty
    if (current1 == NULL || current1->word[0] == '\0') {
        printf("Skipping uninitialized node\n");
        continue;
    }
    
    // copy word from table to new node
    strcpy(the_node->word ,current1->word );
    the_node->next = NULL;
    // add node to union list
    add_at_head(&union_head, the_node);
    (*union_ct)++;
}

// redundant check, can be removed
if (head1 == NULL && head2 == NULL) {
    printf("Both tables empty\n");
    return NULL;
}

// iterate over second table
HASH_ITER(hh, head2, current1, tmp1){
    // check if current word exists in first table
    HASH_FIND_STR(head1, current1->word, temp_entry_checker);
    
    // if word not found in first table, add to union list
    if(temp_entry_checker == NULL){
    node* the_node2 = malloc(sizeof(node));
 
    strcpy(the_node2->word ,current1->word );
    the_node2->next = NULL;
    add_at_head(&union_head, the_node2);
    (*union_ct)++;

    }
    // if word found in first table, increment intersection count
    else{
        (*intersection_ct)++;
    }
}

// return the union list
return union_head;
}
#endif