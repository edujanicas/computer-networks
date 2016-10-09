#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* trsCore(char* buffer, char* language) {

    char *instruction, *word, reply[2048] = "ERR\n\0";
    char *line = NULL;
    char *brkt, *brkb;
    char numberOfWordsStr[2];
    int numberOfWords, n;

    FILE *translation;
    size_t len = 0;
    ssize_t read;

    instruction = strtok_r(buffer, " \n", &brkt);

    if (!strcmp(instruction, "TRQ")) {
      // The user requests the TRS to translate the provided text words (t) or image file (f).
      instruction = strtok_r(NULL, " \n", &brkt);

      if (!strcmp(instruction, "t")) {
        // TRQ t N W1 W2 ... WN
        // In the first case (t), N specifies the number of words that are being sent.
        // W1 W2 ... WN are strings specifying each of the N words, separated by spaces.
        // It can be assumed that each word (Wn) contains no more than 30 characters.
        // No more than 10 words are sent in each request instruction.
        strcpy(reply, "TRR t ");

        translation = fopen("text_translation.txt", "r");
        if (translation == NULL) {
            perror("Error opening text_translation.txt");
            exit(EXIT_FAILURE);
        }

        strcpy(numberOfWordsStr, strtok_r(NULL, " \n", &brkt));
        numberOfWords = atoi(numberOfWordsStr);
        strcat(reply, numberOfWordsStr);

        for(n = 0; n < numberOfWords; n++) {

            instruction = strtok_r(NULL, " \n", &brkt); // instruction retains the word to be translated

            while ((read = getline(&line, &len, translation)) != -1) {
                word = strtok_r(line, " \n", &brkb); // Then get the word
                if (!strcmp(instruction, word)) { // If it's the required one
                    word = strtok_r(NULL, " \n", &brkb);
                    strcat(reply, " "); // Append a whitespace between words
                    strcat(reply, word);
                    break;
                }
            }
            rewind(translation); // Position the stream at the beggining of the file
        }
        strcat(reply, "\n");

      } else if (!strcmp(instruction, "f")) {

        translation = fopen("file_translation.txt", "r");
        if (translation == NULL) {
            perror("Error opening file_translation.txt");
            exit(EXIT_FAILURE);
        }
      }
    }
    printf("Exit Core with reply: %s\n", reply);
    fclose(translation);
    if (line) free(line);

    return reply;
}
