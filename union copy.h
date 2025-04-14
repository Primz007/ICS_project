#include "uthash.h"
#include "unique_elements.h"
#include <string.h>
#ifndef UNION_H
#define UNION_H
node* union_intersection_set(table* head1, table* head2, int* union_ct, int* intersection_ct){

node* union_head = NULL;
table* temp_entry_checker;
table* current1, *tmp1;
*union_ct = 0;
*intersection_ct = 0;
if (head1 == NULL && head2 == NULL) {
    printf("Both tables are empty\n");
    return NULL;
}


//iterate ober first table and add all to unionised linked list
HASH_ITER(hh, head1, current1, tmp1){
    node* the_node = malloc(sizeof(node));
    // memset(the_node, 0, sizeof(node));
    

    if (current1 == NULL || current1->word[0] == '\0') {
        printf("Skipping uninitialized node\n");
        continue;
    }
    

    strcpy(the_node->word ,current1->word );
    the_node->next = NULL;
    add_at_head(&union_head, the_node);
    (*union_ct)++;


}
if (head1 == NULL && head2 == NULL) {
    printf("Both tables empty\n");
    return NULL;
}

HASH_ITER(hh, head2, current1, tmp1){
    HASH_FIND_STR(head1, current1->word, temp_entry_checker);//looking if a word from table 2 is in table 1 or not, basically at node
    //current 1 check the word at that node and compare it with words in table 1 if not in table 1 add to unionised linked list
    if(temp_entry_checker == NULL){
    node* the_node2 = malloc(sizeof(node));
 
    strcpy(the_node2->word ,current1->word );
    the_node2->next = NULL;
    add_at_head(&union_head, the_node2);
    (*union_ct)++;

    }
    else{
        (*intersection_ct)++;
        do_Nothing();
    }
}


return union_head;
}
#endif