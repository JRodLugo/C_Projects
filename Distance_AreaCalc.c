/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
9/??/25
*/

/*
Program that gets two points from the user and calculates
the distance and the area based off those two points
*/

//library
#include <stdio.h>
#include<math.h> //for math functions

//define to help calculate the area
#define PI 3.14159

//functions for calculating area and distance
double distanceCalc(double posX1, double posY1, double posX2, double posY2);
double areaCalc(double distance);

//main function
int main(){
    //quardinates
    double x1, y1, x2, y2;

    //getting user input
    printf("please input first set of quardinates: ");
    scanf("%lf %lf" , &x1, &y1);

    printf("please input second set of quardinates: ");
    scanf("%lf %lf" , &x2, &y2);

    double totalDistance = distanceCalc(x1,y1,x2,y2);
    double totalArea = areaCalc(totalDistance);
    
    //results
    printf("Point #1 entered: x1 = %lf; y1 = %lf\n", x1, y1);
    printf("Point #2 entered: x2 = %lf; y2 = %lf\n", x2, y2);
    printf("The distance between the two points is %lf\n", totalDistance);
    printf("The area of the city encompassed by your request is %lf\n", totalArea);

}//end of main

// formula for distance is sqr[(x2-x1)^2+(y2-y1)^2]
double distanceCalc(double posX1, double posY1, double posX2, double posY2){
    //inside of the formula
    double x_Total = pow((posX2-posX1),2);
    double y_Total = pow((posY2-posY1),2);
    //prints out the result
    return sqrt(x_Total+y_Total);

}//end of func

//formula for area is (Pi)(distance/2)^2
double areaCalc(double distance){
    return PI * pow((distance/2),2);
}//end of func

//end of program