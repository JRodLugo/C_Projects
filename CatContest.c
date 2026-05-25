/*
Jayden Rodriguez Lugo
COP 3502C - Computer Science 1
1/21/26
*/

/*
program reads the Cat ID, Name, and three beauty scores for N cats (N <= 500)
dispays each cats ID, Name, their three beauty scores, and an Average beauty score
calculates percentage(2 decimal places) of cat names begin with 
each letter of the english alphabet, if no cat name starts with letter, displays 0 for that letter
*/

//library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cat.h" //header file 
//file I/O
FILE *inp1;

//main func
int main(){
    //varibles
    int numCats = 0;
    //array of structs
    struct cat mCats[MAXCATS];
    //reads data from the file and onto the array
    readData(mCats, &numCats);
    //Displays name of cutest cat
    printf("Cutest cat is %s\n", mCats[getCutestCat(mCats, numCats)].name);

    //varible for the loop
    char cLetter = 'A';
    //loops through the alphabet
    for(int i = 0; i < 26; i++){
        //varibles
        float cPercent = 0.00;
        int cLetterCount = 0;
        //loops through the cats
        for(int j = 0; j < numCats; j++){
            //if letter matches, inciment cLetterCount by 1
            if(cLetter == mCats[j].name[0]){
                cLetterCount++;
            }
        }
        //gets percent by dividing the num of cats that begin with cLetter divided by the total num cats times 100
        cPercent = ((float)cLetterCount/(float)numCats) * 100;//type casting ints to floats to perform calculation
        printf("%c %d %%%.2lf\n", cLetter, cLetterCount, cPercent);//output
        cLetter++;//moves to next letter of alphabet
    }
    
}//end of main

//reads the inputs from the file
void readData(struct cat *cats, int *c){
    int i;//for the for loop
    //opens given file, inputs number of cats into numCats
    inp1 = fopen("in1.txt", "r");
    
    fscanf(inp1, "%d", c);
    //loops through the file inputting data into the array
    for(i = 0; i < *c; i++){
        fscanf(inp1, "%d", &cats[i].cat_ID);
        fscanf(inp1, "%s", &cats[i].name);
        //beauty numbers
        fscanf(inp1, "%d", &cats[i].score1);
        fscanf(inp1, "%d", &cats[i].score2);
        fscanf(inp1, "%d", &cats[i].score3);
        //calculates the average beauty value cat i
        float cAverage = cats[i].score1 + cats[i].score2 + cats[i].score3;
        cats[i].average = (cAverage) / 3;
        //displays data
        printf("%d %s %d %d %d %.2lf\n", cats[i].cat_ID,cats[i].name,
            cats[i].score1,cats[i].score2,cats[i].score3,cats[i].average);
    }//end of loop
    
    //closes the file
    fclose(inp1);
}//end of func

//returns the highest average beauty score
int getCutestCat(struct cat *s, int n){
    //varibles
    int HighestAverage = 0, highestIndex = 0, i;
    //loops through the array to find the highest average
    for(i = 0; i < n; i++){
        //temp var t is equal to current average i
        int t = s[i].average;
        //if t is greater than the current HighestAverage
        if(t > HighestAverage){
            //set HighestAverage to t, logs index
            HighestAverage = t;
            highestIndex = i;
        }//end of if
    }//end of loop
    //returns highest Index
    return highestIndex;
}//end of func


//end of program