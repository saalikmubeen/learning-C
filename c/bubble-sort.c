#include <stdio.h>



// Worst cost -> n^2
// Best cost -> n
int main() {

    int numbers[] = {1, 500, 700, 2, 432, 0,  6, 354, 20002, 8, 34,678,  1001, };

    int length = sizeof(numbers) / sizeof(numbers[0]);



    int swap_counter = 1;
    for (int i = 0; i < length; i++) {

      if (swap_counter == 0){
          printf("Returning early \n");
          break;
      }

    swap_counter = 0;
    for (int j = 0; j < length - 1; j++) {

        if (numbers[j] > numbers[j + 1]) {
          int temp = numbers[j];
          numbers[j] = numbers[j + 1];
          numbers[j + 1] = temp;

          swap_counter++;
      }
    }
  }


  // int swap_counter = 1;

  // while (swap_counter != 0) {
  //   swap_counter = 0;
  //   for (int j = 0; j < length - 1; j++) {
  //       if (numbers[j] > numbers[j + 1]) {
  //         int temp = numbers[j];
  //         numbers[j] = numbers[j + 1];
  //         numbers[j + 1] = temp;

  //         swap_counter++;
  //     }
  //   }
  // }

    for (int i = 0; i < length; i++) {
      printf("%i \t", numbers[i]);
    }
    printf("\n");



}
