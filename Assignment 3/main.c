#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utility.h"

int main() {
    int lineCount;
    WordData* data = getData("vader_lexicon.txt", &lineCount);  // Pass pointer to lexiconSize
    if (!data) {
        return 1;
    }

    const char* sentences[] = {
        "VADER is smart, handsome, and funny.",
        "VADER is very smart, handsome, and funny.",
        "VADER is VERY SMART, handsome, and FUNNY!!!",
        "VADER is not smart, handsome, nor funny."
    };
    int numSentences = sizeof(sentences) / sizeof(sentences[0]);

    for (int i = 0; i < numSentences; i++) {
        float score = calcSentiment(sentences[i], data, &lineCount);
        printf("Sentence: \"%s\"\nCompound Score: %.4f\n\n", sentences[i], score);
    }

    free(data);
    return 0;
}