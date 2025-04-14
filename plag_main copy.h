/*
NOTE FOR OTHER MEMBERS

ADD VARIABLE NORMALISATION AS IT CONSIDERS VARIABLES TO BE DIFFERENT



*/
//color coding printf for debugging lite le

#ifndef PLAG_CHECK_H
#define PLAG_CHECK_H


#include <stdio.h>
#include "file_read.h"
#include "linked_list.h"
#include "uthash.h"
#include "unique_elements.h"
#include "union.h"
most_used* give_me_top5_used_words(most_used* arr, int len){
    
    
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            if( (arr + i)->ct >= (arr + j)->ct){
                int temp = (arr + i)->ct;
                char temp2[WORD_LENGTH];
                strcpy(temp2,(arr + i)->tok);
                (arr + i)->ct = (arr + j)->ct;
                strcpy((arr + i)->tok , (arr + j)->tok);
                (arr + j)->ct = temp;
                strcpy((arr + j)->tok , temp2);
                
            }
        }
    }

    return arr;
}

double plag_check() {

    //tldr; change path acc.
    node* head1 = fileRead("INPUT_FOLDER/first.txt");
    node* head2 = fileRead("INPUT_FOLDER/second.txt");
    // debug_printlist(head1);
    // debug_printlist(head2);

    // THE HASH TABLE
int unique_count_1, unique_count_2;
table* unique_head_1 = get_unique_elements(head1, &unique_count_1);
table* unique_head_2 = get_unique_elements(head2, &unique_count_2);
// debug_print_unique_elements(unique_head_1);
// debug_print_unique_elements(unique_head_2);

//get most used words ki list
most_used* mostused_txt1 = return_most_used(unique_head_1, unique_count_1);
most_used* mostused_txt2 = return_most_used(unique_head_2, unique_count_2);
//SORTING TIME
mostused_txt1 = give_me_top5_used_words(mostused_txt1, unique_count_1);
mostused_txt2 = give_me_top5_used_words(mostused_txt2, unique_count_2);

//GET RESPECTIVE counts
int union_ct = 0;
int intersection_ct = 0;
int* q = &intersection_ct;
int* p = &union_ct;
node* union_head = union_intersection_set(unique_head_1, unique_head_2, p, q);
// printf("union %d and intersection %d\n", *p, *q);

//getting plag score 
double plag_score = (double)((double)intersection_ct / (double)union_ct);
plag_score *= 100.000;
//plag score calc and printing most used elements
// You may print top 5 most used words if u feel like

//  debug_printlist(union_head);
// debug_print_unique_elements(unique_head_2);

//lite

return plag_score;
}

#endif