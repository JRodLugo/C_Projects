/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
Small Program 2 - Functions Practise
9/??/25
*/

/*
Program that calculates the Cylanders volume and surface area
while also calculating the cost of tickets for an amusment park
*/

//library
#include <stdio.h>
#include<math.h> //for math functions

//define for the function PI
#define PI 3.14159

//functions
double cylinderCalc(double radius , double height);
void printSurfaceArea(double radius , double height);
double ticketCost(int adults, int children);

int main(){
    //varibles
    double userRadius, userHeight, cylinderVolumeTotal;

    //user input for radius and height
    printf("Enter radius: ");
    scanf("%lf", &userRadius);

    printf("Enter height: ");
    scanf("%lf", &userHeight);
    //calculates volume of the cylinder
    cylinderVolumeTotal = cylinderCalc(userRadius,userHeight);

    printf("\nCylinder Volume: %.3f\n", cylinderVolumeTotal);
    printSurfaceArea(userRadius, userHeight);


    //ticket calculations
    int numAdults, numChildern;

    printf("\nEnter Number of adults: ");
    scanf("%d", &numAdults);

    printf("Enter Number of childern: ");
    scanf("%d", &numChildern);

    //calculates ticket prices
    double ticketCalc = ticketCost(numAdults,numChildern);
    printf("Total ticket cost: %.3f\n", ticketCalc);

    return 0;
}//end of main

//function for calculating volume of the cylinder
//using calculation PIr^2 * h
double cylinderCalc( double radius , double height){
    return PI * pow(radius,2) * height;
}//end of function

//computes and prints the surface area
//using calculation 2PIr(h+r)
void printSurfaceArea(double radius , double height){
    double total = 2*PI*radius*(height + radius);
    printf("Cylinder Surface Area:%.3f\n", total);

}//end of function

//calculates total cost of tickets
double ticketCost(int adults, int children){
    double adultCost = 39.99;
    double childCost = 24.50;
    return (adultCost * adults)+(childCost * children);

}//end of function