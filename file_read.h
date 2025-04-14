#include <stdio.h>
#include "linked_list.h"
#include <stdlib.h>
#include <ctype.h>
#ifndef FILE_READ_H
#define FILE_READ_H
//max size of file (how many charector change as fit)
#define size_of_file 262144*8

void do_Nothing(){
    //the function does nothing
    //placeholder
    
}
//Read the file first
node* fileRead(char* filename){
    
    //initialise the linked list head to NULL
    //so that we can add nodes to it later
    node *head = NULL;
    
    //open the file in read mode
    FILE *file_pointer = fopen(filename, "r");
    //get the fail condition
    if(file_pointer == NULL){
    perror("error opening file");
    return head;
    }
    //allocate 2MB for the buffer
    char* buffer = calloc(size_of_file,sizeof(char));
    // memset(buffer, 0, size_of_file);//setting memory of entire buffer to 0 before so garbage values are impossible to send to linked list

    //READ THE LINE AND TERMINATE WHEN FGETS RETURN NULL POINTER
    while(fgets(buffer, size_of_file, file_pointer)){
        //remove the newline character from the end of the line
        //strcspn returns the index of the first occurrence of any character in the second string in the first string
        buffer[strcspn(buffer, "\n")] = ' ';
        //APPLY FOR EACH WORD
        //required as else tokenisation process may end 
        //up including it


        // IGNORING COMMENT and #include
        //strncmp compares two charectors of string we pass with the one we wanna compare
        if(strstr(buffer, "#")){
            buffer[strcspn(buffer, "#")] = '\0';
        }
        if (strstr(buffer, "//"))
        {   
            //we cant skip the whole thing if code is like this: code // comment
            //in this case i'll delete last parts for multiline comment
            
            buffer[strcspn(buffer, "//")] = '\0';
            // remove the thing when // starts so code before // remains!!!
            
        }
        // for */ untill we find */ it will skip adding nodes
        //skips inline */ comment */ but skips code on it
        if(strstr(buffer, "/*") && strstr(buffer, "*/")){
            
            continue;
            //if multiline
            /*
            abc
            */
           //it will check for starting point /* and untill it doesnt find */ 
           // it skips lines, after finding break and skip this line
           //but it will skip code after it
        }
        else if(strstr(buffer, "/*")){
            while(fgets(buffer, size_of_file, file_pointer)){
                if(strstr(buffer, "*/"))
                {
                    break;}
                    continue;
            }
            continue;
        }
        //Now we'll start tokenising,
            //STD STRTOK METHOD, START WITH BUFFER IT REMEMBER WHERE IT LEFT OFF SO PASS NULL
            // AND IT CONTINUE TILL NULL POINTER
            char* token = strtok(buffer, " ,.-\n!:;(){}[]\"/*-+=|");
            while(token != NULL){
                if(strlen(token) == 0){
                   // token = NULL; -> this never works as you are messing with strtok tracking system
                   token = strtok(NULL,  " ,.-\n!:;(){}[]\"/*-+=|");
                    continue;
                   
                }
           
            node* p1 = malloc(sizeof(node));
            if (p1 == NULL) {
                perror("Download more ram nub");
                exit(1);
            }
            
            char lower[WORD_LENGTH] = {0};
             //strcpy(lower, token);
             
            strncpy(lower, token, WORD_LENGTH - 1); // strncpy to prevent overflow for eg if lenght
            //exceeds 128 so it would truncate till max length and add \0 safely copies i.e.
            //strcpy doesnt add \0 at end 
         
            //strncpy does not null-terminate the string if the source is longer than or equal to the destination size.
            lower[WORD_LENGTH - 1] = '\0';
            for(int i = 0; i <strlen(lower); i++){//length of token undefined may exceed 128
                lower[i] = tolower(lower[i]);
            }

            strcpy( p1->word, lower);
            p1->next = NULL;
            //debugging if the token is garbage or not
            //  printf("TOKEN: '%s' (first char ASCII: %d)\n", token, (int)token[0]);

            // printf("TOKEN: %s\n", lower);
           
            add_at_head(&head, p1);
            
            token = strtok(NULL, " ,.-\n!:;(){}[]\"/*-+=|");
            }
        
        
    }
    
    
    free(buffer);
    return head;
}


#endif
