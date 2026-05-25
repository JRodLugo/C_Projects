/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
11/28/25
*/

/*  
Records using a dynamically allocated array of structs, 
where each struct contains a dynamically allocated array of strings 
representing that member’s hobbies
*/

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Members struct
typedef struct {
int numHobbies;
char **hobbies;
} Member;

//functions to add and print hobbies
void addHobbies(Member* thisMemb, int* numMemb, int* nhobbies);
void printHobbies(Member* thisMemb, int numMemb, int nhobbies);

//main 
int main(){
    //variables
    int numMembers = 0;
    //user input
    printf("Enter number of members: ");
    scanf("%d", &numMembers);
    //dynamic array of structs
    Member *memb = malloc(sizeof(Member) * numMembers);
    int numberhobbies = 0;
    //adds hobbies to the collection
    addHobbies(memb, &numMembers, &numberhobbies);

    //prints all of the hobbies
    printHobbies(memb, numMembers, numberhobbies);

    //freeing hashspace for every level
    for(int i = 0; i < numMembers; i++){
        for(int j = 0; j < memb[i].numHobbies; j++){
            free(memb[i].hobbies[j]);
        }
        free(memb[i].hobbies);
    }
    free(memb);
}//end of main

//adds hobbies to the members struct 
void addHobbies(Member* thisMemb, int* numMemb, int* nhobbies){
     //gets input for each member and there hobbies
    for(int i = 0; i < *numMemb; i++){
        printf("Member %d hobbies: ", i + 1);
        scanf("%d", &thisMemb[i].numHobbies);
        *nhobbies += thisMemb[i].numHobbies;
        //allocating space for the array of strings
        thisMemb[i].hobbies = malloc(sizeof(char*) * 50);
        //gets input for the number of hobbies a member i has
        for(int j = 0; j < thisMemb[i].numHobbies; j++){
            //dynamic memory for the strings themselves
            thisMemb[i].hobbies[j] = malloc(sizeof(char) * 50);
            scanf("%s", thisMemb[i].hobbies[j]);
        }//end of nested loop
    }//end of loop
}
//prints the collected Hobbies
void printHobbies(Member* thisMemb, int numMemb, int nhobbies){
    int SharedHobConfirmation = 0;
    char** totalHobbies = malloc(sizeof(char*) * (nhobbies));
    int hobbiesAdded = 0;
    
    //printing out the shared hobbies
    printf("Shared Hobbies:\n");
    for(int i = 0; i < numMemb; i++){
        //loops through the number of hobbies member i has
        for(int j = 0; j < thisMemb[i].numHobbies; j++){
            int count = 0;
            for(int k = 0; k < hobbiesAdded; k++){
                //if the hobby in member i section j is the same as hobby k in total hobbies
                if(strcmp(thisMemb[i].hobbies[j], totalHobbies[k]) == 0){
                    count++;//increase count by one
                }// end of if
            }//end of k
            //allocating size for totalHobbies
            totalHobbies[hobbiesAdded]=malloc(sizeof(char)*50);
        
            //adds the hobby from thisMemb into totalHobbies, inriments hobbiesAdded by one
            //this is done so the code dosn't print the same hobby twice
            strcpy(totalHobbies[hobbiesAdded], thisMemb[i].hobbies[j]); 
            hobbiesAdded++;

           //if the count is greater than one, print hobby
            if(count == 1){
                printf("%s\n",thisMemb[i].hobbies[j]);
                SharedHobConfirmation = 1;//confirms if any shared hobbies
            }
        }//end of j
    }//end of loop

    //freeing hashspace for every level
    for(int i = 0; i < nhobbies; i++){
        free(totalHobbies[i]);
    }
    free(totalHobbies);
    //if no shared hobbies
    if(SharedHobConfirmation == 0){
        printf("No shared hobbies found.");
    }
}

//end of code