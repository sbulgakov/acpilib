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
    
    /*Power adapter information*/
    if((ret=readadp(0)) != 0)
    {
      fprintf(stderr, "Error reading adapter status");
      if(ret == -1) fprintf(stderr, ": information not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
    else
    {
      printf("Power adapter:\n"
               "\tADP0\n");
      if(adponline())
        printf("\tOnline\n");
      else
        printf("\tOffline\n");
    }
    
    /*Battery informaton*/
    if((ret=readbat(0)) != 0)
    {
      fprintf(stderr, "Error reading battery status");
      if(ret == -1) fprintf(stderr, ": information not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
    else
    {
      printf("Battery:\n"
               "\tBAT0\n");
      if(batpres())
      {
        printf("\t%d%%\n", batcap());
        switch(batstat())
        {
          case Discharging:
            printf("\tDischarging\n");
            break;
          case Charging:
            printf("\tCharging\n");
            break;
          case Full:
            printf("\tFull\n");
            break;
        }
      }
      else
        printf("\tAbsent\n");
    }
  }
  
  return 0;
}

