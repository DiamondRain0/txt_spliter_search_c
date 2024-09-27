#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
void search(char A[500][25], int b, char *S, int count);

int main() {
    // File pointers
    FILE *ignore;
    FILE *text;
    FILE *word;
    
    // Variable declarations
    int i = 0, j = 0, k = 0, a = 0, b;
    char temp[20], line[100], str[20];
    char IGNORE[213][20];
    char TEXT[500][25];
    char alp[500][25];
    char delim[] = " ";
    char *fileDelim;
    int count1 = 0, count2 = 0;

    // Open ignore list file
    ignore = fopen("ignoreList.txt", "r");
    if (ignore == NULL) {
        printf("Ignore list file can't be opened\n");
        return 1;
    }

    // Read ignore list into IGNORE array
    a = 0;
    while (fscanf(ignore, "%s", temp) != EOF) {
        strcpy(IGNORE[a], temp);
        a++;
    }
    fclose(ignore);

    // Get file name from user
    printf("Enter a file name: ");
    scanf("%s", temp);

    // Open the text file for reading
    text = fopen(temp, "r");
    if (text == NULL) {
        printf("File can't be opened\n");
        return 1;
    }

    // Read and tokenize the text file by spaces, ignoring listed words
    while (fgets(line, sizeof(line), text)) {
        fileDelim = strlwr(strtok(line, delim));
        while (fileDelim != NULL) {
            int isIgnored = 0;
            for (i = 0; i < 213; i++) {
                if (strcmp(fileDelim, IGNORE[i]) == 0 || strcmp(fileDelim, ".") == 0 || strcmp(fileDelim, ",") == 0 || strcmp(fileDelim, " ") == 0) {
                    isIgnored = 1;
                    break;
                }
            }

            if (!isIgnored) {
                strcpy(TEXT[j], fileDelim);
                j++;
                count1++;
            }
            fileDelim = strtok(NULL, delim);
        }
    }
    fclose(text);

    // Write unique words and their frequencies to word.txt
    word = fopen("word.txt", "w");
    if (word == NULL) {
        printf("Unable to open word file for writing\n");
        return 1;
    }

    for (i = 0; i < count1; i++) {
        int counter = 1;

        // Count occurrences of each word
        for (j = i + 1; j < count1; j++) {
            if (strcmp(TEXT[i], TEXT[j]) == 0) {
                counter++;
            }
        }

        // Check if the word has already been counted
        int isDuplicate = 0;
        for (k = 0; k < i; k++) {
            if (strcmp(TEXT[i], TEXT[k]) == 0 || strcmp(TEXT[k], "\0") == 0 || strcmp(TEXT[k], " ") == 0) {
                isDuplicate = 1;
                break;
            }
        }

        if (!isDuplicate && strcmp(TEXT[i], "") != 0) {
            fprintf(word, "%s,%d\n", TEXT[i], counter);
            printf("%s,%d\n", TEXT[i], counter);
        }
    }
    fclose(word);

    // Alphabetically sort unique words
    for (i = 0; i < count1; i++) {
        int isUnique = 1;
        for (j = 0; j < i; j++) {
            if (strcmp(TEXT[j], TEXT[i]) == 0 || strcmp(TEXT[i], "") == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique && TEXT[i] != NULL) {
            strcpy(alp[i], TEXT[i]);
            count2++;
        }
    }

    for (i = 0; i < count2; i++) {
        for (j = i + 1; j < count2; j++) {
            if (strcmp(alp[i], alp[j]) > 0) {
                char tempp[20];
                strcpy(tempp, alp[i]);
                strcpy(alp[i], alp[j]);
                strcpy(alp[j], tempp);
            }
        }
    }

    // Search functionality
    while (1) {
        printf("(Enter 0 to end) Enter a search string: ");
        scanf("%s", str);
        if (strcmp(str, "0") == 0) break;
        search(alp, 0, str, count2);
    }

    printf("---------End----------\n");
    return 0;
}

// Recursive search function
void search(char A[500][25], int b, char *S, int count) {
    if (strcmp(A[b], S) == 0) {
        printf("%s is in the list\n", S);
        return;
    }
    if (b == count) {
        printf("%s is not in the list\n", S);
        return;
    }
    search(A, b + 1, S, count);
}
