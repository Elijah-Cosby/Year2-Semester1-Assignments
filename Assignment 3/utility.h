// utility.h

#ifndef UTILITY_H
#define UTILITY_H

#define ARRAY_SIZE 10
#define MAX_WORD_LEN 20  // Adjust if needed for longer words

typedef struct {
    char word[MAX_WORD_LEN];
    float value1; // Mean sentiment value
    float value2; // Standard deviation
    int intArray[ARRAY_SIZE]; // Array of sentiment ratings
} WordData;

// Function prototypes
WordData* getData(const char* filename, int* lineCount);
float getWordScore(const char* word, WordData* data, int *lineCount);
int isAllCaps(const char* word);
int isIntensifier(const char* word);
float calcSentiment(const char* sentence, WordData* data, int* lineCount);

#endif 
