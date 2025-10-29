/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
10/03/25
*/

/*
Simple marble game where user competes with the computer
first one to take the last amount of marbles looses
*/

//library
#include <stdio.h>

#define ROUNDS 2

//functions
void greeting();
int playRound(int round);
int humanPick();
int computerPick(int userPrev, int leftover);
int leftOnTable(int marbles, int taken);
void winnerAnnouncement(int userWon);

//main function
int main(){
    //greets the player
    greeting();
    //set the amount of rounds played
    int roundsPlayed = 1;
    int winner = 0;
    while(roundsPlayed <= ROUNDS){
        //the game
        winner = playRound(roundsPlayed);
        //announces the winner
        winnerAnnouncement(winner);
        //prints a new line
        printf("\n");
        //round ticker goes up intil it reaches the same number rounds is
        roundsPlayed += 1;
    }
    printf("------- THANKS FOR PLAYING -------");
    return 0;
}//end of func


void greeting(){
    //simple text greetings
    printf("Welcome to Marble Mountain!\n");
    printf("Start with 25 marbles.On your turn, take 1-4 marbles.\n");
    printf("Who ever takes the LAST marble LOSES. You go first\n");
}
//manages rounds within the game
int playRound(int round){
    int marbles = 25;
    int lastUserPick = 0;
    int lastCompPick = 0;
    int winner = 2;
    printf("=== Round: %d ===\n", round);
    while(marbles != 0){
        //prints marble amount at the end of every turn
        //players turn
        lastUserPick = humanPick();
        //when player makes an invalid input
        if(lastUserPick >= 5 || lastUserPick <= 0){
            printf("Invalid Input, Try Again\n");
            //loops to the beginning of the loop
            continue;
        } else {
            //if input is valid(took 1-4 marbles) the the amound of marbles is subtracted by
            //nested if statment, checks if users turn will result in 0 marbles
            if ((marbles - lastUserPick) == 0 && winner > 1){
                winner = 0;
                marbles = 0;
            } else{
                marbles -= lastUserPick;
            }
        }
        //computers Turn
        lastCompPick = computerPick(lastUserPick, marbles);
        printf("Computer Took %d\n", lastCompPick);
        //if computer is about to take all the remaning marbles
        if((marbles - lastCompPick) == 0 && winner > 1){
            winner = 1;
            marbles = 0;
        //else computer takes marbles and continues
        } else{
            marbles -= lastCompPick;
        }
        //prints the amount of marbles left 
        printf("\nMarbles Left: %d\n", marbles);
    }
    return winner;
}
//users action
int humanPick(){
    //gets user input
    int UserInput;
    printf("How many will you take (1-4)? ");
    scanf("%d", &UserInput);
    return UserInput;
}
//computers action
int computerPick(int userPrev, int leftover){
    //if leftover is greater than 5
    int CompAnswer = 0;
    if(leftover > 5){
        CompAnswer = 5 - userPrev;
    //if leftover is between 2 and 5
    } else if(leftover >= 2 && leftover <= 5){
        CompAnswer = leftover - 1; //leaves one marble
    //if there is only one marble left
    } else if (leftover == 1){
        CompAnswer = 1;
    }
    //returns the answer
    return CompAnswer;
}
//Updates and returns how many marbles are left after a move
int leftOnTable(int marbles, int taken){
    return marbles - taken;
}
//Prints a message announcing the winner of the round
void winnerAnnouncement(int userWon){
   //1 means the user won, 0 means the computer won
   if(userWon == 1){
        printf("!!! YOU WON THE ROUND !!!\n");
   }else if(userWon == 0){
        printf("!!! I won the round, better luck next time !!!\n");
   }
}
//end of code