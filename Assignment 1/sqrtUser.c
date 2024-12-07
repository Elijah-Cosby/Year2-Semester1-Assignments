// This assignment was to make a function to take the sqrt of a number to a specified precision without using the Math.h library.
// The  function should be take two parameters, the number to find the sqrt of and the precision of the result.
// The function should return the sqrt of the number to the specified precision.
// I chose to use the Binary Search method to accomplish this.
#include <stdio.h>

// This power function is needed to turn the integer given for how many decimal places of precision, into the actual precision.
double powFunc(int base, int exponent){
    double pow = base;
    for(int i = 0; i<(exponent-1); i++){
        pow = pow*base;
    }
    return pow;
}

// This funciton is the actual sqrt function that can be called to find the sqrt of a number to a specified precision.
double sqrtUser(double number, int n) {
    double leftIndex = 0.0;
    double rightIndex = number;
    double precision = 1 / powFunc(10,n);
    double midIndex = rightIndex/2;
    while ((rightIndex-leftIndex)>precision){
        if (midIndex*midIndex<number){
            leftIndex = midIndex;
        }
        else{
            rightIndex = midIndex;
        }
        midIndex = (rightIndex+leftIndex)/2;
    }
    return midIndex;
}
