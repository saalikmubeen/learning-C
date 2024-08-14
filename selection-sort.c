#include <stdio.h>


// Worst cost -> n^2
// Best cost -> n^2
int main() {

    int numbers[] = {1, 500, 700, 2, 432, 0,  6, 354, 20002, 8, 34,678,  1001, };

    int length = sizeof(numbers) / sizeof(numbers[0]);


    for (int i = 0; i < length; i++) {

      int minIdex = i;

    for (int j = i + 1; j < length; j++) {
      if (numbers[j] < numbers[minIdex]) {
        minIdex = j;
      }
    }

    if (i != minIdex) {
      int temp = numbers[i];
      numbers[i] = numbers[minIdex];
      numbers[minIdex] = temp;
    }
  }



    for (int i = 0; i < length; i++) {
      printf("%i \t", numbers[i]);
    }
    printf("\n");



}
