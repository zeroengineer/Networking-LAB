#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main()
{
  int nf, N, x;
  int tr = 0;
  srand(time(NULL));
  
  printf("Enter the number of frames: ");
  scanf("%d", &nf);
  
  printf("Enter the Window Size: ");
  scanf("%d", &N);
  
  int i = 1;
  
  while(i <= nf)
  {
    x = 0; // Reset x for each window of frames
    
    // Send frames in the window
    for(int j = i; j < i + N && j <= nf; j++)
    {
      printf("Sent Frame %d \n", j);
      tr++;
    }

    // After sending frames, check for acknowledgment
    for(int j = i; j < i + N && j <= nf; j++)
    {
      int flag = rand() % 2;  // Simulate random frame loss (0 = received, 1 = lost)
      
      if (!flag)  // Frame successfully received
      {
        printf("%d: Acknowledged! \n", j);
        x++;  // Increment x when a frame is successfully acknowledged
      }
      else  // Frame lost, need retransmission
      {
        printf("Frame %d Not Received \n", j);
        printf("Retransmitting Window \n");
        break;  // Break out of the loop to retransmit the window
      }
    }
    
    i += x;  // Update i by the number of successfully acknowledged frames
    printf("\n");
  }
  
  printf("Total number of transmissions: %d \n", tr);
  
  return 0;
}

