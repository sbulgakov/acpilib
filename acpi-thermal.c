/*
    SPDX-FileCopyrightText: 2024 Stanislav Bulgakov
 
    SPDX-License-Identifier: MIT
*/

#include <stdio.h>

#include "acpi.h"


int main(int argc, char **argv)
{
  if(argc == 1)
  {
    int ret;
    
    int i;
    for(i = 0; i < 10; ++i)
    {
      if(testmon(i, NULL))
      {
        char name[32] = {0};
        testmon(i, name);
        
        /*Hardware monitor information*/
        if((ret=readmon(i)) == -1)
        {
          fprintf(stderr, "Error reading hardware monitor status");
          fprintf(stderr, ": information not available");
          fprintf(stderr, "\n");
        }
        else
        {
          printf("Hardware monitor:\n"
                 "\thwmon%d\n", i);
          if(name[0])
            printf("\t%s\n", name);
          if(ret == -2)
            printf("\ti/o error\n");
          else
            printf("\t%f C\n", montemp1()/1000);
        }
      }
    }
  }
  
  return 0;
}

