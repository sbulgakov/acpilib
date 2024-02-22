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
    char name[32] = {0};
    int ret;
    
    if(!findlit(name))
    {
      printf("Backlight information not available\n");
      return 1;
    }
    
    /*Backlight information*/
    if((ret=readlit(name)) != 0)
    {
      fprintf(stderr, "Error reading backlight status");
      if(ret == -1) fprintf(stderr, ": information not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
    else
    {
      printf("Backlight:\n"
             "\t%s\n", name);
      printf("\tBrightness %d\n", litact());
      printf("\tMaximum    %d\n", litmax());
    }
  }
  else if(argc == 3)
  {
    int ret;
    int val;
    
    litnam(argv[1]);
    val = atoi(argv[2]);
    
    /*Set backlight*/
    if((ret=litset(val)) != 0)
    {
      fprintf(stderr, "Error writing backlight value");
      if(ret == -1) fprintf(stderr, ": not available");
      else if(ret == -2) fprintf(stderr, ": i/o error");
      fprintf(stderr, "\n");
    }
  }
  
  return 0;
}

