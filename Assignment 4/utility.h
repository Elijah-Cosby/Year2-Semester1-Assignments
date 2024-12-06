#ifndef UTILITY_H
#define UTILITY_H

// Function pointer type for objective functions
typedef double (*ObjectiveFunction)(int, double *);

typedef struct Bound{
    double lowerBound;
    double upperBound;
}Bound;


// Function prototypes
double random_double(double min, double max);
double pso(ObjectiveFunction objective_function, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double best_position[]);

// CODE: declare other functions and structures if necessary
typedef struct Particle{
    double *position;
    double *velocity;
    double *bestPosition;
    double fitnessVal;
    double bestFitnessVal;
}Particle;

#endif // UTILITY_H
