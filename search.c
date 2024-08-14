#include <stdio.h>


// Worst cost -> log(n)
// Best cost -> 1
int main() {

    int numbers[] = {1, 2, 6, 8, 34, 354, 432, 500, 678, 700, 1001, 20002};
    int secret = 20002;

    // variable to store size of Arr
    int length = sizeof(numbers) / sizeof(numbers[0]);
    int start = 0;
    int end = length - 1;

    while (start <= end) {
        int  middle = start + ((end - start) / 2);
        // printf("Looping");
        printf("Middle: %i \n", middle);

        if (secret == numbers[middle]) {
            printf("Found: %i \n", numbers[middle]);
            return 0;
        } else if ((secret < numbers[middle]) == 1) {
            // printf("Bye");
            end = middle - 1;
        } else if ((secret > numbers[middle]) == 1) {
            // printf("Hi");
            start = middle + 1;
        }

    }

     printf("Not Found: %i \n", secret);
}
