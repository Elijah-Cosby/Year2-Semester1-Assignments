#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h> 

#include "utility.h"

#define INTENSIFIER_BOOST 0.293
#define ALLCAPS_BOOST 1.5
#define NORMALIZATION_ALPHA 15.0

WordData* getData(const char* fileName, int* lineCount){
    FILE *dataPtr;
    dataPtr = fopen(fileName, "r");
    if (dataPtr == NULL){
        printf("Error opening file\n");
        return NULL;
    }
    
    //Find number of words
    int numLines = 0;
    char line[MAX_WORD_LEN];
    while (fgets(line, sizeof(line), dataPtr) != NULL){
        numLines++;
    }
    WordData* data = malloc(numLines * sizeof(WordData));
    
    *lineCount = 0;
    //Used fscanf here over fgets because then we won't have to do the parsing through the entire line afterwards
    while (fscanf(dataPtr, "%s %f %f", data[*lineCount].word, &data[*lineCount].value1, &data[*lineCount].value2) == 3){
        for(int i=0; i<ARRAY_SIZE;i++){
            fscanf(dataPtr, "%d", &data[*lineCount].intArray[i]);
            if (fscanf(dataPtr, "%d", &data[*lineCount].intArray[i]) != 1) {
                printf("Error reading data\n");
                break;  // Handle the error appropriately
            }
        }
        (*lineCount)++;
    }
    fclose(dataPtr);
    return data;
}

float getWordScore(const char* word, WordData* data, int *lineCount) {
    for (int i = 0; i < *lineCount; i++) {
        if (strcmp(word, data[i].word) == 0) {
            return data[i].value1;
        }
    }
    return 0; // Word not found in lexicon
}

int isAllCaps(const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (!isupper(word[i])) return 0;
    }
    return 1;
}

int isIntensifier(const char* word) {
    const char* intensifiers[] = {"very", "extremely", "absolutely", "completely"};
    for (int i = 0; i < sizeof(intensifiers) / sizeof(intensifiers[0]); i++) {
        if (strcmp(word, intensifiers[i]) == 0) return 1;
    }
    return 0;
}

float calcSentiment(const char* sentence, WordData* data, int* lineCount) {
    float totalScore = 0;
    int sentWordCount = 0;

    char sentenceCopy[250];
    strcpy(sentenceCopy, sentence);
    char* word = strtok(sentenceCopy, " ");

    while (word != NULL) {
        float wordScore = getWordScore(word, data, lineCount);
        if (wordScore != 0) {
            if (isIntensifier(word)){
                wordScore *= (1+INTENSIFIER_BOOST);
            }
            if (isAllCaps(word)){
                wordScore *= (1+ALLCAPS_BOOST);
            }
            totalScore += wordScore;
            sentWordCount++;
        }
        word = strtok(NULL, " ");
    }
    float compound = totalScore / sqrt(totalScore * totalScore + NORMALIZATION_ALPHA);
    return compound;
}
