/* COP 3502C PA2 This program is written by: Jayden Rodriguez Lugo */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORES 5 // size of a cat scores array representing 5 traits
#define MAX_STR 25 // maximum name length of a cat
const float POSITION_BONUS[10] = {3.0, 5.0, 4.0, 6.0, 7.0, 2.0, 8.0, 1.0, 9.0, 1.5};


typedef struct Cat{
    char *name; // dynamically allocated cat name
    char *breed; // dynamically allocated breed
    int scores[MAX_SCORES]; // array of size 5 representing trait scores
    int baseScore; // sum of scores array
} Cat;

typedef struct Rivals{
    Cat *cat1; // pointer to cat1
    Cat *cat2; // pointer to cat2
} Rivals;

// bonus percentages that apply to each position
Cat *cats;
Rivals *rivals;
float bestPermScore;
int **tracker;

//score based functions
int countHighPerformersTraits(Cat **team, int rIndex, int catIndex);//to count high performer bonus traits in a team
int synergyBonusApplies(Cat **team, int rIndex, int catIndex);//to check if a team gets a synergy bonus or not
int isDiffBreed(Cat **team, int rIndex, int catIndex);
int positionBonus();
int rivalPenaltyApplies(Cat **team, int catIndex, int numRivals);//to check if a team gets a rival's penalty or not
int areRivals(Cat *cat1, Cat *cat2, int numRivals);//returns 1 if they are rivals, returns 0 if they are not

//creates cats and rivals
void CreateCat(int numCatsper, int totalCatsOut);//creates cats
void createRivals(int r, int totalCats);

//permutation/score handling and freeing memory
void Permutations(int* perm, int* used, int k, int totalCats, int numTeams, int numCatsPer, int numRivals);
float teamComp(int numTeams, int numCatsPer, int totalCats, int k, int *perm, int numRivals);
float scorePermutation(int numTeams, int numCatsPer, int totalCats, int k, int* perm, int numRivals);
void printResults(int numTeams, int numCatsper, int numRivals);//prints out results 
void freeMemory(int totalCatsOut, int numTeams);//frees memory

//main func
int main(){
    int numTeams, numCatsper, numRivals;//varibles
    //gets user input for number teams and cats per team, then calculates the total number of cats
    scanf("%d %d", &numTeams, &numCatsper);
    int totalCatsOut = numTeams * numCatsper;
    CreateCat(numCatsper, totalCatsOut);//creates cats
    
    //creates arrays for permutations, one for the permutations and one used array
    int *PERM = (int*)malloc(totalCatsOut * sizeof(int));
    int *USED = (int*)calloc(totalCatsOut, sizeof(int));
    //if the total number of cats is greater than 1
    if(totalCatsOut > 1){
        //asks user for the amount of rivals then creates the rivals between the cats
        scanf("%d", &numRivals);
        createRivals(numRivals, totalCatsOut);
    }//end of if    

    //setting up the tracker array, setting every element to 0
    tracker = (int**)malloc(numTeams * sizeof(int*));
    for(int i = 0; i < numTeams; i++){
        tracker[i] = (int*)calloc(numCatsper, sizeof(int));
    }
    printf("\n");
    Permutations(PERM, USED, 0, totalCatsOut, numTeams, numCatsper, numRivals);

    //prints the results
    printResults(numTeams, numCatsper, numRivals);
    //frees all memory used
    free(PERM);
    free(USED);
    freeMemory(totalCatsOut, numTeams);
    return 0;
}//end of main

//prints the results
void printResults(int numTeams, int numCatsper, int numRivals){
    //prints out the best permutations score
    printf("Best Teams Grouping score: %.2lf\n", bestPermScore);
    //prints out the teams
    float LargestTeamScore = 0.0;
    int indexI = 0;
    for(int i = 0; i < numTeams; i++){
        float totalTeamScore = 0.0;
        printf("Team %d: ", i);
        //prints the team names
        for(int j = 0; j < numCatsper; j++){
            printf("%s ", cats[tracker[i][j]].name);
            totalTeamScore += cats[tracker[i][j]].baseScore;
        }//end of j loop
        //builds a small array for the team, calculates the final team score
        int tempPerm[10];
        for(int j = 0; j < numCatsper; j++){
            tempPerm[j] = tracker[i][j];
        }//end of j loop
        totalTeamScore = scorePermutation(1, numCatsper, numCatsper, 0, tempPerm, numRivals);

        //gets the team index for the team with the largest score
        if(totalTeamScore > LargestTeamScore){
            LargestTeamScore = totalTeamScore;
            indexI = i;
        }//end of if
        printf("%.2lf\n", totalTeamScore);//prints total team score
    }//end of loop
    printf("Best Candidate: ");
    for(int i = 0; i < numCatsper; i++){
        printf("%s ", cats[tracker[indexI][i]].name);
    }
    printf("\n");
}//end of func

//creates a single cat
void CreateCat(int numCatsper, int totalCatsOut){
    //allocates memory for cats
    cats = (Cat*)malloc(totalCatsOut * sizeof(Cat));
    //temp for both memory and breeds
    char tempName[MAX_STR];
    char tempBreed[MAX_STR];
    for(int i = 0; i < totalCatsOut; i++){
        cats[i].baseScore = 0;//sets basescore to 0
        //gets data from user, allocates memory for name and breeds
        scanf("%s %s", tempName, tempBreed);
        cats[i].name = malloc(strlen(tempName) + 1 * sizeof(char));
        cats[i].breed = malloc(strlen(tempBreed) + 1 * sizeof(char));
        strcpy(cats[i].name, tempName);
        strcpy(cats[i].breed, tempBreed);

        // printf("%s %s temp name and breed\n", tempName, tempBreed);
        // printf("%s %s temp name and breed\n", cats[i].name, cats[i].breed);
        //gets the scores and calculates best score
        for(int j = 0; j < MAX_SCORES; j++){
            //takes in the 5 scores and calculates the base score.
            scanf("%d", &cats[i].scores[j]);
            cats[i].baseScore += cats[i].scores[j];
        }
        //printf("my base score is %d\n", cats[i].baseScore);
    }
}//end of func

//creates rival between two specified cats
//r is the num of rivals
void createRivals(int r, int totalCats){
    //Dynamically allocates memory for rivals array
    rivals = (Rivals*)malloc(r * sizeof(Rivals));
    //creates temp cat varibles
    Cat *cat1 = NULL;
    Cat *cat2 = NULL;
    //creates two cat strings to input as rivals
    char n1[MAX_STR + 1];
    char n2[MAX_STR + 1];
    for(int i = 0; i < r; i++){
        //gets the name of the cats that want to be rivals
        scanf("%s %s", n1, n2);
        //finds the rival cats
        for(int j = 0; j < totalCats; j++){
            if(strcmp(cats[j].name, n1) == 0){
                //printf("I ran\n");
                cat1 = &cats[j];
            }
            if(strcmp(cats[j].name, n2) == 0){
                //printf("I ran\n");
                cat2 = &cats[j];
            }
        }
        //inputs them as rivals
        rivals[i].cat1 = cat1;
        rivals[i].cat2 = cat2;
    }//end of loop
}//end of func


float teamComp(int numTeams, int numCatsPer, int totalCats, int k, int *perm, int numRivals){
    float totalScore = 0.0;
    for(int i = 0; i < numTeams;i++){
        totalScore += scorePermutation(numTeams, numCatsPer, totalCats, k, &perm[i * numCatsPer], numRivals);
    }//end loop
    return totalScore;
}//end func

//generates the permutations of different cat teams
void Permutations(int* perm, int* used, int k, int totalCats, int numTeams, int numCatsPer, int numRivals){
    //if k is greater than the total number of cats
    if (k == totalCats){
        //calculates possible team combinations
        float s = teamComp(numTeams, numCatsPer, totalCats, k, perm, numRivals);
        //when s is greater than the best score
        if(s > bestPermScore){
            bestPermScore = s;//set bestPermScore equal to s
            for(int i = 0; i < numTeams; i++){//loop through num of teams
                for(int j = 0; j < numCatsPer; j++){//loop through num of totalCats
                    tracker[i][j] = perm[i * numCatsPer + j];//add the cat to the tracker
                }//end of j loop
            }//end of loop
        }//end of if
    }else{
        //permutate through all of the cats
        for (int i=0; i< totalCats; i++) {
            if (!used[i]){
                used[i] = 1; //mark that it is used
                perm[k] = i; //transfer i to the the perm array at kth position
                //increase k and grow further
                Permutations(perm, used, k+1, totalCats, numTeams, numCatsPer, numRivals); 
                used[i] = 0; //unmark i for next process.
            }//end of if
        }//end of loop
    }//end of else
}//end of func

float scorePermutation(int numTeams, int numCatsPer, int totalCats, int k, int* perm, int numRivals){
    //creates an array of cats, allocates memory and puts cats into the teams
    float finalScore = 0.0;
    float baseScore = 0.0;
    int startingIndex = 0;
    Cat **team = (Cat**)malloc(numCatsPer * sizeof(Cat*));

    //inputs cats into teams, adds every cats base score to the finalscore
    for(int i = 0; i < numCatsPer; i++){
        team[i] = &cats[perm[i]];
        baseScore += team[i]->baseScore;
    }//end of loop
    finalScore = baseScore / numCatsPer;

    //adds bonus to the final score
    int ScoreIndex = countHighPerformersTraits(team, startingIndex, numCatsPer);
    finalScore += ScoreIndex * 5;

    //gives synergyBonus when applicable
    ScoreIndex = synergyBonusApplies(team, startingIndex, numCatsPer);
    finalScore += ScoreIndex * 30;

    //gives bonus for different breeds, if dup is found subtract 15 from final score
    ScoreIndex = isDiffBreed(team, startingIndex, numCatsPer);
     if(ScoreIndex == 1){
        finalScore += 10;
    }else{
        finalScore -= 15;
    }//end of if else

    //calculates the position scores for each cat in the current team 
    for(int i = 0; i < numCatsPer; i++){
        finalScore += team[i]->baseScore * (POSITION_BONUS[i]/100);
    }//end of loop
    
    //applies the rival penalty if cats are rivals
    ScoreIndex = rivalPenaltyApplies(team, numCatsPer, numRivals);
    if(ScoreIndex > 0){
        finalScore -= 25;
    }//end of if

    //frees memory for the team and returns the final score
    free(team);
    return finalScore;
}//end of func

//For each cat in the team, add +5 points for each trait score that is 90 or higher
int countHighPerformersTraits(Cat **team, int rIndex, int catIndex){
    
    //if the team index is equal to cat index, return 0
    if(rIndex == catIndex){
        //printf("returning 0!\n");
        return 0;
    }//end of if
    //loops through the cat index;
    int count = 0;
    //loops through the scores of each cat within the team, incriments count when score is greater or equal 90
    for(int i = 0; i < MAX_SCORES; i++){
        //printf("%d current scores %d\n", i, team[rIndex]->scores[i]);
        if(team[rIndex]->scores[i] >= 90){
            //printf("Im looping!\n");
            count++;
        }//end of if
    }//end of loop
    //printf("Current count value %d\n", count);
    //adds count to the function and increments index by 1
    return count + countHighPerformersTraits(team, rIndex + 1, catIndex);
    
}//end of func

/*If each cat on the team (e.g., all 3 cats in a 3-cat team) has at least one trait score of 85 or higher,
add +30 bonus points to the team’s score.*/
int synergyBonusApplies(Cat **team, int rIndex, int catIndex){
    //if index is equal to cat index then return 1
    if(rIndex == catIndex){
        return 1;//everything has been checked
    }
    int count = 0;
    //looks through each cats score to see if they are greater than or equal to 85
    for(int i = 0; i < MAX_SCORES; i++){
        if(team[rIndex]->scores[i] >= 85){
            count = 1;
            break;
        }//end of if
    }//end of loop
    //count has failed, return 0
    if(!count){
        return 0;
    }//end of if
    //adds count to the function and increments index by 1
    return synergyBonusApplies(team, rIndex + 1, catIndex);
}//end of func

//If all cats on a team are of different breeds, add +10 points to the team score.
int isDiffBreed(Cat **team, int rIndex, int catIndex){

    //rIndex =0
    //catIndex = numcats per team
    char** array = (char**)malloc(catIndex * sizeof(char*));
    //inputs breeds into the array
    for(int i = 0; i < catIndex; i++){
        array[i] = (char*)malloc(catIndex * sizeof(char));
        strcpy(array[i], team[i]->breed);
    }//end of loop
    //checks for dupes
    for(int i = 0; i < catIndex - 1; i++){
        for(int k = i; k < catIndex - 1; k++){
            if(strcmp(array[i], team[k+1]->breed) == 0){
                for(int j = 0; j < catIndex; j++){
                    free(array[j]);
                }//end of j loop
                free(array);
                return 0;//returns 0 when dupes
            }//end of if
        }
    }//end of loop
    //if no dupes return 1
    for(int j = 0; j < catIndex; j++){
        free(array[j]);
    }
    free(array);
    return 1;
}//end of func

//sees if cats are rivals
int rivalPenaltyApplies(Cat **team, int catIndex, int numRivals){
    //if index is equal to cat index then return 0
    //printf("Rivals is running\n");
    // if(catIndex <= numRivals){
    //     return 0;
    // }
    if(catIndex <= 1){
        return 0;
    }

    if(areRivals(team[catIndex-1], team[catIndex-2], numRivals) == 1){
        //printf("rivalPenaltys return 1\n");
        return 1 + rivalPenaltyApplies(team, catIndex -1, numRivals);
    }

    return rivalPenaltyApplies(team, catIndex -1, numRivals);;
}//end of func

//checks if cats are rivals
int areRivals(Cat *cat1, Cat *cat2, int numRivals){
    // printf("Are rivals is running\n");
    // printf("%s is cat 1 %s is cat 2\n", cat1->name, cat2->name);
    // printf("%s is r 1 %s is r 2\n", rivals[0].cat1->name, rivals[0].cat2->name); //here
    for(int i = 0; i < numRivals; i++){
        //printf("Current index i %d\n", i);
        if((rivals[i].cat1 == cat1 && rivals[i].cat2 == cat2) || 
        (rivals[i].cat1 == cat2 && rivals[i].cat2 == cat1)){
            //printf("Are rivals ran 1\n");
            return 1;//confirms cats are rivals 
        }
    }
    //printf("Are rivals ran 0\n");
    return 0;//cats are not rivals
}

//frees memory for all of the dynamically allocated items
void freeMemory(int totalCatsOut, int numTeams){
    // printf("Testing\n");
    // printf("%d total cats\n", totalCatsOut);
    //frees memory for cats and tracker
    for(int c = 0; c < totalCatsOut; c++){
        //printf("Testing loop %d\n", c);
        free(cats[c].name);
        free(cats[c].breed);
    }//end of c loop

    for(int i = 0; i < numTeams; i++){
        free(tracker[i]);
    }
    //printf("Exited lloop!\n");
    //frees rest of the memory
    free(cats);
    //printf("cats where freed\n");
    free(rivals);
    free(tracker);
    //printf("Func ran!\n");
}//end of func
