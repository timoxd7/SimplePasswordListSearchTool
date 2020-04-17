#include <stdio.h>
#include <conio.h>
#include <string.h>


#define LINE_BUFFER_SIZE 102400
#define MAX_PASSWORD_LENGTH 1000
#define MAX_FILEPATH_LENGTH 1000

#define MAX_ACTION_COMMAND_LENGTH 5

int main() {
    // Get passwort to be searched for
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter the Passwort to be searched for: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    
    if (strlen(password) <= 0) {
        printf("Password is empty!\n");
        getchar();
        return 1;
    } else {
        // Remove Newline
        if (password[strlen(password) - 1] == '\n')
            password[strlen(password) - 1] = '\0';
    }

    // Next, get Filepath to the Password file List
    char filePath[MAX_FILEPATH_LENGTH];
    printf("Enter Path to your Password List File: ");
    fgets(filePath, MAX_FILEPATH_LENGTH, stdin);
    
    if (strlen(filePath) <= 0) {
        printf("Filepath is empty!\n");
        getchar();
        return 2;
    } else {
        // Remove Newline
        if (filePath[strlen(filePath) - 1] == '\n')
            filePath[strlen(filePath) - 1] = '\0';
        
        // Remove Leading "
        while (filePath[0] == '"') {
            int filePathLength = strlen(filePath);
            for (int i = 1; i <= filePathLength; i++) {
                filePath[i - 1] = filePath[i];
            }
        }

        // Remove Following "
        while (filePath[strlen(filePath) - 1] == '"')
            filePath[strlen(filePath) - 1] = '\0';
    }

    // Open File
    FILE* passwordList;
    passwordList = fopen(filePath, "rb");

    if (passwordList == NULL) {
        // Throw Error if not possible to open
        printf("Error opening File!\n");
        getchar();
        return 3;
    }

    // Some Vars for the Loop
    int endOfFile = 0;
    int passwordFound = 0;
    long currentLine = 0;
    long lineBufferOverflow = 0;

    // Start and reprint info
    printf("Checking file: ");
    printf(filePath);
    printf("\nFor password: ");
    printf(password);
    printf("\nFor large files, this can take a while!\n");

    // Endless checking loop
    for(;;) {
        char lineBuffer[LINE_BUFFER_SIZE];
        int readPosition = 0;

        // Almost endless line search loop. Ends on EOF or line ending, filling up line buffer
        for(;;) {
            if (readPosition == LINE_BUFFER_SIZE-1) {
                // => Buffer Overflow, take notice
                lineBufferOverflow++;
                break;
            }

            // Get next char
            int currentChar = fgetc(passwordList);
            
            // End of File?
            if (currentChar == EOF) {
                printf("End of File reached!\n");
                endOfFile = 1;
                break;
            }

            lineBuffer[readPosition++] = currentChar;
            lineBuffer[readPosition] = '\0';

            if (currentChar == '\n') {
                currentLine++;
                break;
            }
        }

        if (strstr(lineBuffer, password) != NULL) {
            passwordFound = 1;
            printf("Found at Line %i and Line Buffer Overflow (should be 0) of %i\n", currentLine, lineBufferOverflow);
            printf("Line containing Password: ");
            printf(lineBuffer);
            
            /*
            printf("\nContinue? (Y\\N): ");

            char inputBuffer[MAX_ACTION_COMMAND_LENGTH];
            fgets(inputBuffer, MAX_ACTION_COMMAND_LENGTH, stdin);
    
            if (strlen(inputBuffer) <= 0 || inputBuffer[0] != 'Y') {
                printf("Stopping search.\n");
                break;
            }
            */
        }

        if (endOfFile == 1) break;
    }

    fclose(passwordList);

    if (passwordFound == 0) {
        printf("Great, password is not in the List!\n");
    }

    printf("End of Program\n");
    getchar();
    return 0;
}