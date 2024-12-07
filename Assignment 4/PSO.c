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
        for(int j=0; j<NUM_VARIABLES;j++){
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
    const int MAX_NO_IMPROV = 100;
    double bestGlobalFitness = INFINITY;
    double previousBest = bestGlobalFitness;
    int noImprovement = 0;
    double *bestGlobalPosition = malloc(NUM_VARIABLES * sizeof(double));
    Particle *Swarm = (Particle *)malloc(NUM_PARTICLES*sizeof(Particle));

    allocateInitParticles(Swarm, NUM_PARTICLES, NUM_VARIABLES, bounds, objective_function, &bestGlobalFitness, bestGlobalPosition);

//For neighboorhood method, adding a local best
    double *bestLocalPosition = malloc(NUM_PARTICLES * NUM_VARIABLES * sizeof(double));
    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int k = 0; k < NUM_VARIABLES; k++) {
            bestLocalPosition[i * NUM_VARIABLES + k] = Swarm[i].bestPosition[k];
        }
    }

//Now onto the actual PSO
    for (int i=0; i<MAX_ITERATIONS; i++){
        

        for(int j=0; j<NUM_PARTICLES;j++){
            Swarm[j].fitnessVal = objective_function(NUM_VARIABLES,Swarm[j].position);
            if (Swarm[j].fitnessVal < Swarm[j].bestFitnessVal){
                Swarm[j].bestFitnessVal = Swarm[j].fitnessVal;
                for(int k=0; k<NUM_VARIABLES;k++){
                    Swarm[j].bestPosition[k] = Swarm[j].position[k];
                }
            }
        }

//Gets local best for each particle
        for(int j=0; j<NUM_PARTICLES;j++){
            int right;
            if (j == NUM_PARTICLES - 1){
                right = 0;
            } else {
                right = j + 1;
            }
            int left;
            if (j == 0){
                left = NUM_PARTICLES - 1;
            } else {
                left = j - 1;
            }
            double bestFitness = Swarm[j].bestFitnessVal;
            int bestIndex = j;
            if (Swarm[left].bestFitnessVal < bestFitness) {
                bestFitness = Swarm[left].bestFitnessVal;
                bestIndex = left;
            }
            if (Swarm[right].bestFitnessVal < bestFitness) {
                bestFitness = Swarm[right].bestFitnessVal;
                bestIndex = right;
            }

            for (int k = 0; k < NUM_VARIABLES; k++) {
                bestLocalPosition[j * NUM_VARIABLES + k] = Swarm[bestIndex].bestPosition[k];
            }
        }

        for (int j=0; j<NUM_PARTICLES; j++){
            for (int k=0; k<NUM_VARIABLES; k++){
                double r1 = random_double(0, 1);
                double r2 = random_double(0, 1);
                Swarm[j].velocity[k] = WEIGHT*Swarm[j].velocity[k]+COG_COEF*r1*(Swarm[j].bestPosition[k]-Swarm[j].position[k])+GLOB_COEF*r2*(bestLocalPosition[k+(j*NUM_VARIABLES)]-Swarm[j].position[k]);
                Swarm[j].position[k] = Swarm[j].position[k]+Swarm[j].velocity[k];
                if (Swarm[j].position[k] < bounds[k].lowerBound){
                    Swarm[j].position[k] = bounds[k].lowerBound;
                }
                if (Swarm[j].position[k] > bounds[k].upperBound){
                    Swarm[j].position[k] = bounds[k].upperBound;
                }
            }
            
            if (Swarm[j].fitnessVal < bestGlobalFitness){
                bestGlobalFitness = Swarm[j].fitnessVal;
                for(int k=0; k<NUM_VARIABLES;k++){
                    bestGlobalPosition[k] = Swarm[j].position[k];
                }
            }
        }   
    double currentBest = Swarm[0].bestFitnessVal;
    for(int j = 1; j < NUM_PARTICLES; j++){
        if (Swarm[j].bestFitnessVal < currentBest){
            currentBest = Swarm[j].bestFitnessVal;
        }
    }
    bestGlobalFitness = currentBest;
    printf("Iteration %d, Best Fitness: %f\n", i, bestGlobalFitness);
    if(bestGlobalFitness < previousBest){
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
    free(bestLocalPosition);
    //free(bestLocalFitness);
    return bestGlobalFitness; 
}