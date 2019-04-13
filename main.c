//209089960 chagit stupel

#include <stdio.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define SIZE 1
#define IDENTICAL 1
#define DIFFERENT 2
#define SIMILAR 3

int main(int argc, char *argv[]) {
    char bufFirst[SIZE + 1];
    char bufSecond[SIZE + 1];

    // how many chars were actually red
    int charFdFirst, charFdSecond;

    int fdFirst = open(argv[1], O_RDONLY);
    int fdSecond = open(argv[2], O_RDONLY);

    //restart the ret val to 1
    int retVal = IDENTICAL;

    if (fdFirst == -1 || fdSecond == -1) {
        // print which type of error have in a code
        printf("Error Number % d\n", errno);
    }

    charFdFirst = read(fdFirst, bufFirst, SIZE);
    charFdSecond = read(fdSecond, bufSecond, SIZE);

    //make sure enough readed
    while (charFdFirst == SIZE && charFdSecond == SIZE) {
        //compare the chars
        if (bufFirst[0] == bufSecond[0]) {
            //continue - read another char
            charFdFirst = read(fdFirst, bufFirst, SIZE);
            charFdSecond = read(fdSecond, bufSecond, SIZE);
        } else {
            retVal = SIMILAR;
            //check if space
            if (isspace(bufFirst[0]) || isspace(bufSecond[0])) {
                if (isspace(bufFirst[0])) {
                    charFdFirst = read(fdFirst, bufFirst, SIZE);
                }
                if (isspace(bufSecond[0])) {
                    charFdSecond = read(fdSecond, bufSecond, SIZE);
                }
            } else {
                //check if same letter
                if ((tolower(bufFirst[0])) == (tolower(bufFirst[0]))) {
                    //check if both same
                    charFdFirst = read(fdFirst, bufFirst, SIZE);
                    charFdSecond = read(fdSecond, bufSecond, SIZE);
                } else {
                    //2 letters not same - break
                    retVal = DIFFERENT;
                    break;
                }
            }
        }
    }
    //check if put because not same
    if (retVal != DIFFERENT && (charFdFirst != charFdSecond)) {
        //check which is still size
        if (charFdFirst == SIZE) {
            //if the other have just spaces - still retVal = 3
            while (charFdFirst == SIZE) {
                //check if its
                if (isspace(bufFirst[0])) {
                    charFdFirst = read(fdFirst, bufFirst, SIZE);
                } else {
                    //not space - not same
                    retVal = DIFFERENT;
                    break;
                }
            }
        } else if (charFdSecond == SIZE) {
            //if the other have just spaces - still retVal = 3
            while (charFdSecond == SIZE) {
                //check if its a space in the end
                if (isspace(bufSecond[0])) {
                    charFdSecond = read(fdSecond, bufSecond, SIZE);
                } else {
                    //not space - not same
                    retVal = DIFFERENT;
                    break;
                }
            }
        }
    }

    // free allocated structures
    close(fdFirst);
    close(fdSecond);

    return retVal;
}