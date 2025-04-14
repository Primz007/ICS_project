
#ifndef PLAG_CHECK_H
#define PLAG_CHECK_H

#include <stdio.h>
#include "file_read.h"
#include "linked_list.h"
#include "uthash.h"
#include "unique_elements.h"
#include "union.h"

// Function to sort and return top 5 most frequently used words
// Takes an array of most_used struct and its length as parameters
most_used* give_me_top5_used_words(most_used* arr, int len){
    // Bubble sort implementation to sort words by frequency (count)
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            if( (arr + i)->ct >= (arr + j)->ct){
                // Swap counts
                int temp = (arr + i)->ct;
                // Temporary storage for word
                char temp2[WORD_LENGTH];
                strcpy(temp2,(arr + i)->tok);
                // Perform the swap
                (arr + i)->ct = (arr + j)->ct;
                strcpy((arr + i)->tok , (arr + j)->tok);
                (arr + j)->ct = temp;
                strcpy((arr + j)->tok , temp2);
            }
        }
    }
    return arr;
}

// Main function to calculate plagiarism percentage
double plag_check() {
    // Read input files and create linked lists of words
    node* head1 = fileRead("INPUT_FOLDER/first.txt");
    node* head2 = fileRead("INPUT_FOLDER/second.txt");

    // Variables to store count of unique words in each file
    int unique_count_1, unique_count_2;
    
    // Create hash tables of unique words from both files
    table* unique_head_1 = get_unique_elements(head1, &unique_count_1);
    table* unique_head_2 = get_unique_elements(head2, &unique_count_2);

    // Get arrays of most frequently used words from both files
    most_used* mostused_txt1 = return_most_used(unique_head_1, unique_count_1);
    most_used* mostused_txt2 = return_most_used(unique_head_2, unique_count_2);
    
    // Sort the arrays to get top 5 most used words
    mostused_txt1 = give_me_top5_used_words(mostused_txt1, unique_count_1);
    mostused_txt2 = give_me_top5_used_words(mostused_txt2, unique_count_2);

    // Variables to store counts for union and intersection
    int union_ct = 0;
    int intersection_ct = 0;
    int* q = &intersection_ct;
    int* p = &union_ct;
    
    // Calculate union and intersection of unique words
    node* union_head = union_intersection_set(unique_head_1, unique_head_2, p, q);

    // Calculate plagiarism score as (intersection/union)*100
    double plag_score = (double)((double)intersection_ct / (double)union_ct);
    plag_score *= 100.000;

    return plag_score;
}

#endif
