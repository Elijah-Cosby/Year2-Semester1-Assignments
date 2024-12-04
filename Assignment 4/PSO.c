// CODE: include library(s)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "OF_lib.h"
#include "utility.h"

// Helper function to generate random numbers in a range
double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// CODE: implement other functions here if necessary
void allocateInitParticles(Particle *Swarm, int NUM_PARTICLES, int NUM_VARIABLES, Bound *bounds, ObjectiveFunction objective_function, double bestGlobalFitness, double bestGlobalPosition[]) {
    for(int i=0; i<NUM_PARTICLES;i++){
        Swarm[i].position = malloc(NUM_VARIABLES*sizeof(double));
        Swarm[i].velocity = malloc(NUM_VARIABLES*sizeof(double));
        Swarm[i].bestPosition = malloc(NUM_VARIABLES*sizeof(double));
        Swarm[i].fitnessVal = malloc(sizeof(double));
        for(int j=0; j<NUM_VARIABLES;j++){
            Swarm[i].position[j] = random_double(bounds->lowerBound, bounds->upperBound);
            Swarm[i].velocity[j] = random_double(bounds->lowerBound, bounds->upperBound);
            Swarm[i].bestPosition[j] = Swarm[i].position[j];
        }
        Swarm[i].fitnessVal = objective_function(NUM_VARIABLES,Swarm[i]->position);
        if (Swarm[i].fitnessVal < bestGlobalFitness){
            bestGlobalFitness = Swarm[i].fitnessVal;
            bestGlobalPosition = Swarm[i].position;
        }
    }
}


double pso(ObjectiveFunction objective_function, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position) {
//Initializes constants and particles
    float COG_COEF = 1.5;
    float GLOB_COEF = 1.5;
    float WEIGHT = 0.7;
    double bestGlobalFitness = INFINITY;
    double bestGlobalPosition[NUM_VARIABLES];
    Particle *Swarm = (Particle *)malloc(NUM_PARTICLES*sizeof(Particle));

    allocateInitParticles(Swarm, NUM_PARTICLES, NUM_VARIABLES, bounds, objective_function, bestGlobalFitness, bestGlobalPosition);

//Now onto the actual PSO
    


    

}