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
    
    // Find number of words
    int numLines = 0;
    char line[512];
    while (fgets(line, sizeof(line), dataPtr) != NULL){
        numLines++;
    }
    // Bring ptr back to start of file
    rewind(dataPtr);

    WordData* data = malloc(numLines * sizeof(WordData));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    *lineCount = 0;
    while (fgets(line, sizeof(line), dataPtr) != NULL){
        char word[MAX_WORD_LEN];
        float value1, value2;
        int intArray[ARRAY_SIZE];

        // Parses line
        char* ptr = line;
        // Reads word
        if (sscanf(ptr, "%s", word) != 1) {
            printf("Error parsing word in line: %s\n", line);
            continue;
        }
        strcpy(data[*lineCount].word, word);
        ptr += strlen(word);

        // Reads values
        if (sscanf(ptr, "%f %f", &value1, &value2) != 2) {
            continue;
        }
        data[*lineCount].value1 = value1;
        data[*lineCount].value2 = value2;

        // Find the opening bracket
        char* bracketStart = strchr(ptr, '[');
        if (!bracketStart) {
            printf("Error: Expected '[' in line: %s\n", line);
            continue;
        }
        bracketStart++; // Move past '['

        // Read the integers
        for (int i = 0; i < ARRAY_SIZE; i++) {
            int val;
            if (sscanf(bracketStart, "%d", &val) != 1) {
                printf("Error parsing integer in line: %s\n", line);
                break;
            }
            data[*lineCount].intArray[i] = val;

            // Move to next number
            bracketStart = strchr(bracketStart, ',');
            if (bracketStart == NULL && i < ARRAY_SIZE - 1) {
                printf("Error: Not enough numbers in array in line: %s\n", line);
                break;
            }
            if (bracketStart) bracketStart++;
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
    int hasLetters = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        if (isalpha((unsigned char)word[i])) {
            hasLetters = 1;
            if (!isupper((unsigned char)word[i])) return 0;
        }
    }
    return hasLetters;
}

int isIntensifier(const char* word) {
    const char* intensifiers[] = {"FRIGGIN","absolutely", "completely", "extremely", "really", "so", "totally","very", "particularly", "exceptionally", "friggin", "incredibly", "remarkably", "uber"};
    for (int i = 0; i < (sizeof(intensifiers) / sizeof(intensifiers[0])); i++) {
        if (strcmp(word, intensifiers[i]) == 0) return 1;
    }
    return 0;
}

int isNegation(const char* word) {
    const char* negations[] = {"barely", "hardly", "scarcely", "somewhat", "mildly", "slightly","partially", "fairly","not", "isn't","isnt", "nor"};
    int numNegations = sizeof(negations) / sizeof(negations[0]);
    for (int i = 0; i < numNegations; i++) {
        if (strcmp(word, negations[i]) == 0) return 1;
    }
    return 0;
}


float calcSentiment(const char* sentence, WordData* data, int* lineCount) {
    float totalScore = 0;
    int sentWordCount = 0;
    int negNext = 0;
    int negScope = 0;
    int intNext = 0;

    char sentenceCopy[250];
    strcpy(sentenceCopy, sentence);
    char* word = strtok(sentenceCopy, " ");

    while (word != NULL) {
        //To ensure word is all lowercase
        char cleanWord[50];
        int j = 0;
        for (int i = 0; word[i]; i++) {
            if (isalpha(word[i])) {
                cleanWord[j++] = tolower(word[i]);
            }
        }
        cleanWord[j] = '\0';

        //Check if word is a negaative word that would make the next word the opposite or an intensifier
        if (isNegation(cleanWord)) {
            negScope = 3;
            intNext = 0;
        }
        if (isIntensifier(cleanWord)) {
            intNext = 1;
        }
        float wordScore = getWordScore(cleanWord, data, lineCount);
        if (wordScore != 0) {
            if (negScope>0) {
                wordScore *= -0.5;
                negScope--;
            }
            if (intNext){
                wordScore *= (1+INTENSIFIER_BOOST);
                intNext = 0;
            }
            if (isAllCaps(word)){
                wordScore *= ALLCAPS_BOOST;
            }
            totalScore += wordScore;
            sentWordCount++;
        }
        word = strtok(NULL, " ");
    }

    int exclamations = 0;
    for (int i = 0; sentence[i]; i++) {
        if (sentence[i] == '!') {
            exclamations++;
        }
    }
    if (exclamations > 0) {
        // Cap the emphasis at 3 exclamation marks
        int emphasis = exclamations > 3 ? 3 : exclamations;
        float boost = emphasis * 0.292;
        totalScore += boost;
    }

    float compound = totalScore / sqrt(totalScore * totalScore + NORMALIZATION_ALPHA);
    return compound;
}
