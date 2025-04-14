#include "linked_list.h"
#include "file_read.h"
#include "uthash.h"    // header for hash table implementation
#include <string.h>
#include <stdlib.h>

#ifndef UNIQUE_ELEMENTS_H
#define UNIQUE_ELEMENTS_H

// structure for hash table entries
typedef struct
{   
    char word[WORD_LENGTH];    // stores the word
    int count;                 // stores frequency of the word
    UT_hash_handle hh;        // makes this structure hashable
}table;

// structure to store most frequently used words
typedef struct{
    char tok[WORD_LENGTH];     // stores the word
    int ct;                    // stores count of word
}most_used;

// function to get unique elements from linked list and their frequencies
table* get_unique_elements(node* head, int* count_of){
    //entry is the node, words is the head of tha hash table (like a linked list)
    table *words = NULL, *entry = NULL;
    *count_of = 0;            // initialize counter for unique words

    while(head != NULL){
        // find the word to see if there in hash table
        HASH_FIND_STR(words, head->word, entry);
        if(entry == NULL){    // if word not found in hash table
            // create new entry for the word
            entry = (table*)malloc(sizeof(table));
            strcpy(entry->word, head->word);

            // add the new entry to hash table
            HASH_ADD_STR(words, word, entry);
            entry->count = 1;  // initialize count to 1
            (*count_of)++;     // increment unique words counter
        }
        else{
            entry->count++;    // increment count if word already exists
        }
        head = head->next;    // move to next word in linked list
    }
    return words;             // return hash table
}

// debug function to print all unique elements and their counts
void debug_print_unique_elements(table* words) {
    table *entry, *tmp;

    // iterate through the hash table
    HASH_ITER(hh, words, entry, tmp) {
        printf("%s: %d\n", entry->word, entry->count);
    }
}

// function to create array of most used words
most_used* return_most_used(table* words,int count){
    table *entry, *tmp;

    // allocate memory for array of most_used structures
    most_used* use_list = calloc(count,sizeof(most_used));
    int i = 0;
    // iterate through hash table and copy data to array
    HASH_ITER(hh, words, entry, tmp){
        (use_list +i)->ct = entry->count;
        strcpy((use_list+i)->tok, entry->word);
        i++;
    }
    return use_list;         // return array of most used words
}

#endif