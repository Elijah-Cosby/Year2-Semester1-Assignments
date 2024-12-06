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
void allocateInitParticles(Particle *Swarm, int NUM_PARTICLES, int NUM_VARIABLES, Bound *bounds, ObjectiveFunction objective_function, double *bestGlobalFitness, double *bestGlobalPosition) {
    for(int i=0; i<NUM_PARTICLES;i++){
        Swarm[i].position = malloc(NUM_VARIABLES*sizeof(double));
        Swarm[i].velocity = malloc(NUM_VARIABLES*sizeof(double));
        Swarm[i].bestPosition = malloc(NUM_VARIABLES*sizeof(double));
        //double initPosition = random_double(bounds[i].lowerBound, bounds[i].upperBound);
        for(int j=0; j<NUM_VARIABLES;j++){
            //Swarm[i].position[j] = initPosition;
            Swarm[i].position[j] = random_double(bounds[j].lowerBound, bounds[j].upperBound);
            Swarm[i].velocity[j] = random_double(bounds[j].lowerBound, bounds[j].upperBound);
            Swarm[i].bestPosition[j] = Swarm[i].position[j];
        }
        Swarm[i].fitnessVal = objective_function(NUM_VARIABLES, Swarm[i].position);
        Swarm[i].bestFitnessVal = Swarm[i].fitnessVal;
        if (Swarm[i].bestFitnessVal < *bestGlobalFitness){
            *bestGlobalFitness = Swarm[i].fitnessVal;
            for(int j=0; j<NUM_VARIABLES;j++){
                bestGlobalPosition[j] = Swarm[i].bestPosition[j];
            }
        }
    }
}


double pso(ObjectiveFunction objective_function, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position) {
//Initializes constants and then particles
    const double COG_COEF = 1.5;
    const double GLOB_COEF = 1.5;
    const double WEIGHT = 0.7;
    const int MAX_NO_IMPROV = 1000;
    double bestGlobalFitness = INFINITY;
    double previousBest = bestGlobalFitness;
    int noImprovement = 0;
    double *bestGlobalPosition = malloc(NUM_VARIABLES * sizeof(double));
    Particle *Swarm = (Particle *)malloc(NUM_PARTICLES*sizeof(Particle));

    allocateInitParticles(Swarm, NUM_PARTICLES, NUM_VARIABLES, bounds, objective_function, &bestGlobalFitness, bestGlobalPosition);

//Now onto the actual PSO
    for (int i=0; i<MAX_ITERATIONS; i++){
        for(int j=0; j<NUM_PARTICLES;j++){
            for (int k=0; k<NUM_VARIABLES; k++){
                double r1 = random_double(0, 1);
                double r2 = random_double(0, 1);
                Swarm[j].velocity[k] = WEIGHT*Swarm[j].velocity[k]+COG_COEF*r1*(Swarm[j].bestPosition[k]-Swarm[j].position[k])+GLOB_COEF*r2*(bestGlobalPosition[k]-Swarm[j].position[k]);
                Swarm[j].position[k] = Swarm[j].position[k]+Swarm[j].velocity[k];
                if (Swarm[j].position[k] < bounds[k].lowerBound){
                    Swarm[j].position[k] = bounds[k].lowerBound;
                }
                if (Swarm[j].position[k] > bounds[k].upperBound){
                    Swarm[j].position[k] = bounds[k].upperBound;
                }
            }
            Swarm[j].fitnessVal = objective_function(NUM_VARIABLES,Swarm[j].position);
            if (Swarm[j].fitnessVal < Swarm[j].bestFitnessVal){
                Swarm[j].bestFitnessVal = Swarm[j].fitnessVal;
                for(int k=0; k<NUM_VARIABLES;k++){
                    Swarm[j].bestPosition[k] = Swarm[j].position[k];
                }
            }
            if (Swarm[j].fitnessVal < bestGlobalFitness){
                bestGlobalFitness = Swarm[j].fitnessVal;
                for(int k=0; k<NUM_VARIABLES;k++){
                    bestGlobalPosition[k] = Swarm[j].position[k];
                }
            }
        }   
    printf("Iteration %d, Best Fitness: %f\n", i, bestGlobalFitness);
    if (bestGlobalFitness < previousBest){
        noImprovement=0;
    } else{
        if (noImprovement > MAX_NO_IMPROV){
            printf("No improvement, stopping @ %d iterations.\n", i);
            break;
        }
        noImprovement++;
    }
    previousBest = bestGlobalFitness;
    }

    for (int i=0; i<NUM_VARIABLES; i++){
        best_position[i] = bestGlobalPosition[i];
    }

//Freeing the memory used
    for (int i=0; i<NUM_PARTICLES; i++){
        free(Swarm[i].position);
        free(Swarm[i].velocity);
        free(Swarm[i].bestPosition);
    }
    free(Swarm);
    free(bestGlobalPosition);
    return bestGlobalFitness; 
}