/*
    SPDX-FileCopyrightText: 2024 Stanislav Bulgakov
 
    SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <stdlib.h>

#include "acpi.h"


int main(int argc, char **argv)
{
  if(argc == 1)
  {
    int ret;
    
    /*CPU information*/
    if((ret=cpupres()) < 0)
    {
      fprintf(stderr, "Error reading CPU status");
      if(ret == -1) fprintf(stderr, ": information not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
    else
    {
      char gov[32] = {0};
      int num = ret;
      int i;
      
      for(i=0; i <= num; ++i)
      {
        printf("CPU%d:\n", i);
        ret = govcpu(i, gov);
        if(ret)
        {
          if(ret == -1)
            printf("\tinformation not available\n");
          else if(ret == -2)
            printf("\ti/o error\n");
        }
        else
          printf("\tGovernor %s\n", gov);
        printf("\tFrequency %.1f-%.1f MHz\n", 
          1.0*freqmin(i)/1000000,
          1.0*freqmax(i)/1000000);
      }
    }
  }
  else if(argc == 3)
  {
    int ret;
    int cpu;
    
    cpu = atoi(argv[1]);
    
    /*Set governor*/
    if((ret=govset(cpu, argv[2])) != 0)
    {
      fprintf(stderr, "Error writing cpu governor");
      if(ret == -1) fprintf(stderr, ": not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
  }
  else if(argc == 4)
  {
    int ret;
    int cpu;
    int min;
    int max;
    
    cpu = atoi(argv[1]);
    min = atoi(argv[2]);
    max = atoi(argv[3]);
    
    /*Set frequency*/
    if((ret=freqset(cpu, min, max)) != 0)
    {
      fprintf(stderr, "Error writing cpu frequency");
      if(ret == -1) fprintf(stderr, ": not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
  }
  
  return 0;
}

