/*
 *  Part 1 - Changing Stack Based Variables with a Buffer Overflow
 *  Task - Get authenticated
 *  */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define DEBUG 1

unsigned int BUFFER_SIZE = 0x16;

void getPassword(char* password) {
   FILE *fp;

   fp = fopen("password.txt", "r");
   fscanf(fp, "%s", password);
   // printf("The password is: %s\n", password );
}

void printStack(int **stack, int* loggedIn, char* buffer) {
    int **stackAddress;

    printf("Our stack looks like\n");
    for (stackAddress = stack; stackAddress < stack + 0x10; stackAddress++) {
        printf("%p : %p", stackAddress, *stackAddress);
        if ((int *) stackAddress == loggedIn)
            printf(" <-- loggedIn");
        if ((char *) stackAddress == buffer)
            printf(" <-- enteredPassword");
        printf("\n");
    }
    printf("\n");
}

int authenticateUser() {
    // Notice the position of loggedIn in relation 
    // to the buffers were we can input data
    // Note: integers on a 32bit system are 4 bytes
    // while individual characters are only 1 byte
    struct variables {
        char password[16];
        char enteredPassword[16];
        int loggedIn;
    } vars;
    int **stack;

    vars.loggedIn = 0;
    memset(vars.enteredPassword, 0, sizeof(vars.enteredPassword));
    memset(vars.password, 0, sizeof(vars.password));

    // Basically, our stack from for this function will look
    // like:
    //   TOP OF STACK (lower addresses)      Data goes into buffers going down:
    // -------------------------------                      ||
    // |             ...             |                      ||
    // -------------------------------                    \ || /
    // |                             |                     \||/
    // |  password (64 bytes)        |                      \/
    // |                             |               
    // -------------------------------       When you read data to the stack,
    // |                             |       the data goes into the buffer downwards.
    // |  enteredPassword (64 bytes) |       That is, data gets filled into the buffer
    // |                             |       moving away from the top of the stack
    // -------------------------------       to the bottom of the stack.
    // |  loggedIn (4 bytes)         |
    // -------------------------------
    // |            ...              |
    // -------------------------------
    //   BOTTOM OF STACK (higher addresses)

    // Ask the user for their username and password
    //
    // How much data are we reading into each 
    // stack based buffer?
    // Note: The c read function is defined as:
    //      read(int fileDescriptor, char* destination, unsinged int amountToRead)
    puts("\nPassword: ");
    fgets(vars.enteredPassword, BUFFER_SIZE, stdin);

    // Get rid of the trailing newline character
    size_t len = strlen(vars.enteredPassword) - 1;
    if (vars.enteredPassword[len] == '\n')
        vars.enteredPassword[len] =  '\0';

    if (DEBUG) {
        printf("loggedIn = (decimal) %d = (hex) %x\n", vars.loggedIn, vars.loggedIn);
    }

    puts("");

    // Load the password from a file into memory
    getPassword(vars.password);

    if (DEBUG) {
        stack = (int **) (&stack);
        printStack(stack, &vars.loggedIn, vars.enteredPassword);
    }

    puts("Checking to see if the user's account is legit...");
    if (strcmp(vars.password, vars.enteredPassword) == 0) {
        vars.loggedIn = 1;
    } else {
        // What is the difference between having this line
        // and not having this line? What can we do since
        // this line is not actually a part of the program?
        //
        // loggedIn = 0;
    }

    // If loggedIn has anything but 0, then the user is logged in
    if (vars.loggedIn) return 1;
    // ...else we say that they are not logged in
    else return 0;
}

void printFlag() {
   FILE *fp;
   char flag[64];

   fp = fopen("flag", "r");
   fscanf(fp, "%s", flag);
   printf("Your flag is: %s\n", flag);
}


int main() {
    puts("Welcome to the Login Portal -_-");

    int authenticated = authenticateUser();

    if (authenticated) {
        puts("Hello, would you like to play a game?\n");
        puts("Oh, I guess you already won it lol\n");
        printFlag();
    } else {
        puts("Sorry, I don't know who you are.\n");
    }
}
