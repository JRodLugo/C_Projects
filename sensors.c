/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
10/16/25
*/

/*
scans given files and checks to see if all the file elements are the same
loops through an array and outputs the lowest element, highest element, and the arrays average
loops through an RSVP list, checks to see how many dup Ids there are in the list
outputs the number of og Ids and unique Ids
*/

//library
#include <stdio.h>


//functions
void compareSensors();//pt1
void analyzeTemperatures(double readings[], int count);//pt2
void dedupeRSVPs(int ids[], int count);//pt3

//main function
int main(){
    //problem one
    compareSensors();

    //temperature problem
    double UserArray[1000];
    int RSVP_Array[100];
    int countTemp = 0;
    int RSVP_Count = 0;

    //user input for temperature problem
    printf("Enter temperature readings (enter a negative number to stop): \n");
    for(int i = 0; i < 100; i++){
        scanf("%lf", &UserArray[i]);
        //if user inputed a negative number, break out of loop
        if(UserArray[i] < 0){
            break;
        }
        //keeps track of how big the array is
        countTemp += 1;
    }//end of loop
    analyzeTemperatures(UserArray, countTemp);

    //user input for RSVP problem
    printf("Enter student IDs (enter a negative number to stop): \n");
    for(int i = 0; i < 100; i++){
        scanf("%d", &RSVP_Array[i]);
        //if user inputed a negative number, break out of loop
        if(RSVP_Array[i] < 0){
            break;
        }
        //keeps track of how big the array is
        RSVP_Count += 1;
    }//end of loop

    dedupeRSVPs(RSVP_Array, RSVP_Count);
}//end of main

//Opens and reads all three files line by line (those files are giving to you).
//Compares the contents of the three files
void compareSensors(){
    //inputs for the given files
    FILE *inp1;
    FILE *inp2;
    FILE *inp3;
    //opens the files and reads them
    inp1 = fopen("sensorA.txt" , "r");
    inp2 = fopen("sensorB.txt" , "r");
    inp3 = fopen("sensorC.txt" , "r");
    int scan1, scan2, scan3;

    while(fscanf(inp1, "%d" , &scan1) != EOF){
         fscanf(inp2, "%d" , &scan2);
         fscanf(inp3, "%d" , &scan3);

        //checks for any inconsistencies between the files
        if(scan1 != scan2 || scan1 != scan3){
            printf("Data Inconsistent!\n");
            return;
        } else if(scan2 != scan1 || scan2 != scan3){
            printf("Data Inconsistent!\n");
            return;
        } else if(scan3 != scan1 || scan3 != scan2){
            printf("Data Inconsistent!\n");
            return;
        }
    }//end of loop

    printf("Data Consistent!\n");

    //closes the files
    fclose(inp1);
    fclose(inp2);
    fclose(inp3);
}

//determins the lowest, highest, and average temperatures based on the user’s input
void analyzeTemperatures(double readings[], int count){

    double averageTemp = 0.0;
    double highestTemp = 0.0;
    double highT = 0.0;
    double lowestTemp = 0.0;
    double lowT = 0.0;

    //calculations
    for(int i = 0; i < count; i++){
        //average
        averageTemp += readings[i];

        //checks to see if element i in readings is less than low
        if(readings[i] < lowT){
            //if it is then lowestTemp is set to that element
            lowestTemp = readings[i];
        //if it isn't, then lowT is equal to the element
        } else {
            //inputs current element into the varibles
            lowT = readings[i];
        }

        //checks to see if element i in readings is greater than high
        if(readings[i] > highT){
            //if it is then highestTemp is set to that element
            highestTemp = readings[i];
        } else{
            //inputs current element into the varibles
            highT = readings[i];
        }
    }
    //calculates the average out of all the elements within the array
    averageTemp = averageTemp / (double) count;

    //functions output
    printf("Lowest Temperature: %.2lf\n", lowestTemp);
    printf("Highest Temperature: %.2lf\n", highestTemp);
    printf("Average Temperature: %.2lf\n", averageTemp);
}

//checks to see if there is any dup ids within the array
void dedupeRSVPs(int ids[], int count){
    //varibles
    FILE *rsvpFileOUT;

    int IDchecker = 0;
    int UniqueIds[50];
    int Unique_IDcounter = count;
    //reading and writing the file
    rsvpFileOUT = fopen("unique_rsvps.txt","w");
    //loops through the array
    for(int i = 0; i < count; i++){
        //sets the IDchecker to the current element
        IDchecker = ids[i];
        int j = 0;
        //incriments j based on the size of i only when IDchecker is not equal to current element
        while(IDchecker != UniqueIds[j] && j < i){
            j++;
        }//end of nested while loop

        if(j == i){
            //puts the unique element into the array and into the file
            UniqueIds[i] = IDchecker;
            fprintf(rsvpFileOUT, "%d/n", IDchecker);
        } else{
            //if element isn't unique
            Unique_IDcounter -= 1;
        }

    }//end of loop

    //function output
    printf("Original IDs: %d\n" ,count);
    printf("Unique IDs: %d\n" , Unique_IDcounter);
    //closes the file
    fclose(rsvpFileOUT);
}//end of function

//end of program