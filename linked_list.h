#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define WORD_LENGTH 128
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node{
    char word[WORD_LENGTH];
    struct node* next;
}node;

//This function is unused
node* create_node(char* word){
    node* new = malloc(sizeof(node));
    new->next = NULL;
    strcpy(new->word, word);
    return new;
}

void add_at_head(node** head, node* new){
    new->next = *head;
    *head = new;   
}

//FOR DEBUGGING PURPOSE REMOVE LATER
// void debug_printlist(node *head){
//     if(head == NULL){
//         return;
//     }
//     while(head != NULL){
//         printf("%s ", head->word);
//         head = head->next;
//     }
// }

// void debug_printlist(node *head){
//     while(head != NULL){
//         if (head->word[0] != '\0')
//             printf("[%s] -> ", head->word);
//         else
//             printf("[???] -> ");
//         head = head->next;
//     }
//     printf("NULL\n");
// }

void debug_printlist(node *head){
    while(head != NULL){
        printf("%s ", head->word);
        head = head->next;
    }
}



#endif