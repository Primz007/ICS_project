#include "linked_list.h"
#include "file_read.h"
#include "uthash.h"
#include <string.h>
#include <stdlib.h>

#ifndef UNIQUE_ELEMENTS_H
#define UNIQUE_ELEMENTS_H

typedef struct
{   
    char word[WORD_LENGTH];
    int count;
    UT_hash_handle hh;
}table;
typedef struct{
    char tok[WORD_LENGTH];
    int ct;
}most_used;

table* get_unique_elements(node* head, int* count_of){
    //entry is the node, words is the head of tha hash table (like a linked list)
    table *words = NULL, *entry = NULL;
    *count_of = 0;

    while(head != NULL){
        // find the word to see if there in hash table
        HASH_FIND_STR(words, head->word, entry);
        if(entry == NULL){//if no then, create a node entry with word and add to hash table
            //similar to linked list i am creating a node and placing word in it
            entry = (table*)malloc(sizeof(table));//without typecasting vs code gives error dk why
            strcpy(entry->word, head->word);

            // now node is created using uthash internal function to add it to
            //hash table just like we add node to head in linked list!
            HASH_ADD_STR(words, word, entry);
            entry->count = 1;
            (*count_of)++;
        }
        else{
            entry->count++;
        }
        head = head->next;
    }
    return words;
}

//THIS CODE IS COPIED FROM GPT, FOR DEBUG ONLY REMOVE LATER
void debug_print_unique_elements(table* words) {
    table *entry, *tmp;

    // Iterate through the hash table
    HASH_ITER(hh, words, entry, tmp) {
        printf("%s: %d\n", entry->word, entry->count);
    }
}

most_used* return_most_used(table* words,int count){
    //most uused list!
    table *entry, *tmp;

    most_used* use_list = calloc(count,sizeof(most_used));
    int i = 0;
    HASH_ITER(hh, words, entry, tmp){
        (use_list +i)->ct = entry->count;
        strcpy((use_list+i)->tok, entry->word);
        i++;
    }
    return use_list;
}
// typedef 

#endif