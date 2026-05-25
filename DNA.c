/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
11/08/25
*/

/*

*/
#include <stdio.h>
#include <string.h>
FILE *Inp1;

int main(){
    //variables
    char DNA[30];
    char userInput[30];
    int LargestCanidate = 0;
    int L = 0;
    int location = 0;
    int score_Global = 0;
    char ComparedDNA[100];
    //gets witch input file to read from
    printf("What file stores the DNA test cases?\n");
    scanf("%s", userInput);

    //reads from the file
    Inp1 = fopen(userInput ,"r");
    //Scans the first dna sequence into the string
    //Scans the number of comparable sequences into the varible
    fscanf(Inp1, "%s", DNA);
    fscanf(Inp1, "%d", &LargestCanidate);
    
    //loops through the file to see if the sequences match
    while(fscanf(Inp1, "%s" , ComparedDNA) != EOF){
        //says what DNA sequence can be compared
        location++;
        int score_Local = 0;
        //If the sequence dosn't match
        if(strcmp(DNA, ComparedDNA) != 0){
            //stores the smaller length into the L varible
            if(strlen(ComparedDNA) < strlen(DNA)){
                L = strlen(ComparedDNA);
            }else{
                L = strlen(DNA);
            }
            //calcualting the scores
            for(int i = 0; i < L; i++){
                //If they are a match
                if(DNA[i] == ComparedDNA[i]){
                    score_Local += 3;
                //if they are a specific pairs
                } else if((DNA[i] == 'A' && ComparedDNA[i] == 'T') || (DNA[i] == 'T' && ComparedDNA[i] == 'A')){
                    score_Local += 1;
                } else if((DNA[i] == 'C' && ComparedDNA[i] == 'G') || (DNA[i] == 'G' && ComparedDNA[i] == 'C')){
                    score_Local += 1;
                }  
            }
            //if global is less than local
            //stores what canidate was the biggest
            if(score_Global < score_Local){
                    score_Global = score_Local;
                    LargestCanidate = location;
            }
        } else{
            //if the sequence matches perfectly
            printf("Candidate %d is a perfect match.\n", location);
            break;
        }
    }

    //displays results if they where not equal to each other
    if(strcmp(DNA, ComparedDNA) != 0){
        score_Global = (score_Global * 100) / (3*L);
        printf("The best match is Candidate %d, with a score of %d.\n", LargestCanidate, score_Global);
    }
    //closes the file
    fclose(Inp1);
}