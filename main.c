//209089960 chagit stupel

#include <stdio.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1

#define BIG_MIN 65
#define BIG_MAX 90
#define SMALL_MIN 97
#define SMALL_MAX 122

/**
 * Check if the char is kind of space .
 * @param c . char to check if it soace
 * @return 1 if space , 0 if not
 */
int isSpaceChar(char c) {
    //check if it an space
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Check if the char is big or small letter .
 * @param c . char to chekc
 * @return 1 if big letter 2 if small letter and 0 if not letter
 */
int isBigSmallLetter(char c) {
    //check if big letter
    if ((c >= BIG_MIN) && (c <= BIG_MAX)) {
        return 1;
    } else if ((c >= SMALL_MIN) && (c <= SMALL_MAX)) {
        return 2;
    } else {
        //not a letter
        return 0;
    }
}

int main(int argc, char *argv[]) {
    char bufFirst[SIZE + 1]; /* input (output) buffer */
    char bufSecond[SIZE + 1]; /* input (output) buffer */

    int charFdFirst, charFdSecond;          /* how many chars were actually red */

    int fdFirst = open(argv[1], O_RDONLY);
    int fdSecond = open(argv[2], O_RDONLY);

    int retVal = 1;

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
            retVal = 3;
            //check if space
            if (isSpaceChar(bufFirst[0]) || isSpaceChar(bufSecond[0])) {
                if (isSpaceChar(bufFirst[0])) {
                    charFdFirst = read(fdFirst, bufFirst, SIZE);
                }
                if (isSpaceChar(bufSecond[0])) {
                    charFdSecond = read(fdSecond, bufSecond, SIZE);
                }
            } else {
                //check if same letter
                if (isBigSmallLetter(bufFirst[0]) == 1 && isBigSmallLetter(bufSecond[0]) == 2) {
                    //check if both same
                    if ((bufFirst[0] + (SMALL_MAX - BIG_MAX)) == bufSecond[0]) {
                        charFdFirst = read(fdFirst, bufFirst, SIZE);
                        charFdSecond = read(fdSecond, bufSecond, SIZE);
                    } else {
                        //2 letters not same - break
                        retVal = 2;
                        break;
                    }
                } else if (isBigSmallLetter(bufFirst[0]) == 2 && isBigSmallLetter(bufSecond[0]) == 1) {
                    if ((bufFirst[0]) == (bufSecond[0] + (SMALL_MAX - BIG_MAX))) {
                        charFdFirst = read(fdFirst, bufFirst, SIZE);
                        charFdSecond = read(fdSecond, bufSecond, SIZE);
                    } else {
                        //2 letters not same - break
                        retVal = 2;
                        break;
                    }
                } else {
                    retVal = 2;
                    break;
                }
            }

        }
    }
    //check if put because not same
    if (retVal != 2 && (charFdFirst != charFdSecond)) {
        //check which is still size
        if (charFdFirst == SIZE) {
            //if the other have just spaces - still retVal = 3
            while (charFdFirst == SIZE) {
                //check if its
                if (isSpaceChar(bufFirst[0])) {
                    charFdFirst = read(fdFirst, bufFirst, SIZE);
                } else {
                    //not space - not same
                    retVal = 2;
                    break;
                }
            }
        } else if (charFdSecond == SIZE) {
            //if the other have just spaces - still retVal = 3
            while (charFdSecond == SIZE) {
                //check if its a space in the end
                if (isSpaceChar(bufSecond[0]) == 1) {
                    charFdSecond = read(fdSecond, bufSecond, SIZE);
                } else {
                    //not space - not same
                    retVal = 2;
                    break;
                }
            }
        }
        //out because one is less
    }

    close(fdFirst);
    close(fdSecond); /* free allocated structures */

    return retVal;
}