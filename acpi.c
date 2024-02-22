/*
    SPDX-FileCopyrightText: 2024 Stanislav Bulgakov
 
    SPDX-License-Identifier: MIT
*/

#include "acpi.h"


/* kernel.org/doc/Documentation/ABI/testing/sysfs-class-power */
static char BAT[64] = "/sys/class/power_supply/BAT0/";
/*                     0123456789012345678901234567*/
#define BATNUM 27

static const char BATalarm[]              = "alarm";
static const char BATcapacity[]           = "capacity";
int                  capacity             = 0;
static const char BATcapacity_level[]     = "capacity_level";
static const char BATcycle_count[]        = "cycle_count";
/*device*/
static const char BATenergy_full[]        = "energy_full";
static const char BATenergy_full_design[] = "energy_full_design";
static const char BATenergy_now[]         = "energy_now";
/*hwmonX*/
static const char BATmanufacturer[]       = "manufacturer";
static const char BATmodel_name[]         = "model_name";
/*power*/
static const char BATpower_now[]          = "power_now";
static const char BATpresent[]            = "present";
int                  present              = 0;
static const char BATserial_number[]      = "serial_number";
static const char BATstatus[]             = "status";
int                  status               = 0;
/*subsystem*/
static const char BATtechnology[]         = "technology";
static const char BATtype[]               = "type";
static const char BATuevent[]             = "uevent";
static const char BATvoltage_min_design[] = "voltage_min_design";
static const char BATvoltage_now[]        = "voltage_now";


static char ADP[64] = "/sys/class/power_supply/ADP0/";
/*                     0123456789012345678901234567*/
#define ADPNUM 27

/*device*/
/*hwmonX*/
static const char ADPonline[] = "online";
int                  online   = 0;
/*power*/
/*subsystem*/
static const char ADPtype[]   = "type";
static const char ADPuevent[] = "uevent";
/*wakeupX*/


/* kernel.org/doc/Documentation/hwmon/sysfs-interface */
/* kernel.org/doc/Documentation/ABI/testing/sysfs-class-hwmon */
static char MON[64] = "/sys/class/hwmon/hwmon0/";
/*                     01234567890123456789012*/
#define MONNUM 22

/*device*/
/*power*/
/*subsystem*/
static const char MONname[]            = "name";
static const char MONtemp1_crit[]      = "temp1_crit";
static const char MONtemp1_crit_hyst[] = "temp1_crit_hyst";
static const char MONtemp1_input[]     = "temp1_input";
double               temp1_input       = 0.0;
static const char MONtemp1_max[]       = "temp1_max";
/*uevent*/


/* kernel.org/doc/Documentation/ABI/stable/sysfs-class-backlight */
static char LIT[64] = "/sys/class/backlight/";
/*                     012345678901234567890*/
#define LITNUM 20

static const char LITactual_brightness[] = "actual_brightness";
int                  actual_brightness   = 0;
static const char LITbl_power[]          = "bl_power";
static const char LITbrightness[]        = "brightness";
/*device*/
static const char LITmax_brightness[]    = "max_brightness";
int                  max_brightness      = 0;
/*power*/
static const char LITscale[]             = "scale";
/*subsystem*/
static const char LITtype[]              = "type";
/*uevent*/


/* kernel.org/doc/Documentation/ABI/testing/sysfs-devices-system-cpu */
static char CPU[80] = "/sys/devices/system/cpu/";
/*                     012345678901234567890123*/
#define CPUNUM 23

/*cpuX*/
/*cpufreq*/
/*cpuidle*/
/*hotplug*/
/*isolated*/
/*kernel_max*/
/*microcode*/
/*modalias*/
static const char CPUoffline[]  = "offline";
static const char CPUonline[]   = "online";
static const char CPUpossible[] = "possible";
/*power*/
static const char CPUpresent[]  = "present";
/*smt*/
/*uevent*/
/*vulnerabilities*/


#include <stdio.h>
#include <string.h>
#include <dirent.h>

int readbat(int batnum)
{
  char buf[32];
  FILE *f;
  
  BAT[BATNUM] = '0' + batnum;
  
  capacity = -1;
  BAT[BATNUM+2] = 0;
  strcat(BAT, BATcapacity);
  f = fopen(BAT, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%d", &capacity) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  present = 1;
  BAT[BATNUM+2] = 0;
  strcat(BAT, BATpresent);
  f = fopen(BAT, "r");
  if(f != NULL)
  {
    if(fscanf(f, "%d", &present) != 1) 
    {
      fclose(f);
      return -2;
    }
    fclose(f);
  }
  
  status = -1;
  BAT[BATNUM+2] = 0;
  strcat(BAT, BATstatus);
  f = fopen(BAT, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%s", buf) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  if(strcmp(buf, "Discharging") == 0)   status = Discharging;
  else if(strcmp(buf, "Charging") == 0) status = Charging;
  else if(strcmp(buf, "Full") == 0)     status = Full;
  else if(strcmp(buf, "Unknown") == 0)  status = Unknown;
  
  return 0;
}

int batcap()
{
  return capacity;
}

int batpres()
{
  return present;
}

int batstat()
{
  return status;
}


int readadp(int adpnum)
{
  FILE *f;
  
  ADP[ADPNUM] = '0' + adpnum;
  
  online = -1;
  ADP[ADPNUM+2] = 0;
  strcat(ADP, ADPonline);
  f = fopen(ADP, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%d", &online) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return 0;
}

int adponline()
{
  return online;
}


int testmon(int monnum, char *outname)
{
  FILE *f;
  
  MON[MONNUM] = '0' + monnum;
  
  MON[MONNUM+2] = 0;
  strcat(MON, MONtemp1_input);
  f = fopen(MON, "r");
  if(f == NULL) return 0;
  fclose(f);
  
  if(outname)
  {
    MON[MONNUM+2] = 0;
    strcat(MON, MONname);
    f = fopen(MON, "r");
    if(f == NULL) return 2;
    if(fscanf(f, "%s", outname) != 1) 
    {
      fclose(f);
      return 2;
    }
    fclose(f);
  }
  
  return 1;
}

int readmon(int monnum)
{
  FILE *f;
  
  MON[MONNUM] = '0' + monnum;
  
  temp1_input = -1.0;
  MON[MONNUM+2] = 0;
  strcat(MON, MONtemp1_input);
  f = fopen(MON, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%lf", &temp1_input) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return 0;
}

double montemp1()
{
  return temp1_input;
}


int findlit(char *outname)
{
  int lit;
  DIR* d = opendir(LIT);
  struct dirent *e;
  FILE* f;
  int len;
  
  if(d == NULL) return -1;
  while((e=readdir(d)) != NULL)
  {
    lit = 0;
    
    LIT[LITNUM+1] = 0;
    strcat(LIT, e->d_name);
    strcat(LIT, "/");
    len = strlen(LIT);
    
    strcat(LIT, LITactual_brightness);
    f = fopen(LIT, "r");
    if(f) lit = 1;
    else continue;
    
    LIT[len] = 0;
    strcat(LIT, LITbrightness);
    f = fopen(LIT, "r");
    if(!f) continue;
    
    LIT[len] = 0;
    strcat(LIT, LITmax_brightness);
    f = fopen(LIT, "r");
    if(!f) continue;
    else   break;
  }
  if(lit && e)
  {
    if(outname)
    {
      strcpy(outname, e->d_name);
    }
    
    closedir(d);
    
    return 1;
  }
  
  return 0;
}

int readlit(const char *name)
{
  int len;
  FILE* f;
  
  LIT[LITNUM+1] = 0;
  strcat(LIT, name);
  strcat(LIT, "/");
  len = strlen(LIT);
  
  actual_brightness = -1;
  strcat(LIT, LITactual_brightness);
  f = fopen(LIT, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%d", &actual_brightness) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  max_brightness = -1;
  LIT[len] = 0;
  strcat(LIT, LITmax_brightness);
  f = fopen(LIT, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%d", &max_brightness) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  LIT[len] = 0;
  
  return 0;
}

int litact()
{
  return actual_brightness;
}

int litmax()
{
  return max_brightness;
}

int litnam(const char *name)
{
  LIT[LITNUM+1] = 0;
  strcat(LIT, name);
  strcat(LIT, "/");
  
  return 0;
}

int litset(int brightness)
{
  int out = brightness;
  int len = 0;
  FILE* f;
  
  while(out)
  {
    ++len;
    out /= 10;
  }
  out = len;
  len = strlen(LIT);
  strcat(LIT, LITbrightness);
  f = fopen(LIT, "w");
  if(f == NULL) return -1;
  if(fprintf(f, "%d", brightness) != out) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  LIT[len] = 0;
  
  return 0;
}


int cpupres()
{
  int max = 0;
  FILE* f;
  
  CPU[CPUNUM+1] = 0;
  
  strcat(CPU, CPUpresent);
  f = fopen(CPU, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%*d-%d", &max) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return max;
}

int freqmax(int cpu)
{
  int max = 0;
  FILE* f;
  
  CPU[CPUNUM+1] = 0;
  strcat(CPU,   "cpufreq/policy0/scaling_available_frequencies");
  /*     CPUNUM+0123456789012345*/
  CPU[CPUNUM+15] = '0' + cpu;
  
  f = fopen(CPU, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%d", &max) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return max;
}

int freqmin(int cpu)
{
  int min;
  FILE* f;
  
  CPU[CPUNUM+1] = 0;
  strcat(CPU,   "cpufreq/policy0/scaling_available_frequencies");
  /*     CPUNUM+0123456789012345*/
  CPU[CPUNUM+15] = '0' + cpu;
  
  f = fopen(CPU, "r");
  if(f == NULL) return -1;
  while(fscanf(f, "%d", &min) == 1) ;
  fclose(f);
  
  return min;
}

int freqset(int cpu, int min, int max)
{
  int out;
  int tmp;
  FILE* f;
  
  CPU[CPUNUM+1] = 0;
  strcat(CPU,   "cpufreq/policy0/scaling_min_freq");
  /*     CPUNUM+0123456789012345*/
  CPU[CPUNUM+15] = '0' + cpu;
  
  out = 0;
  tmp = min;
  while(tmp)
  {
    ++out;
    tmp /= 10;
  }
  f = fopen(CPU, "w");
  if(f == NULL) return -1;
  if(fprintf(f, "%d", min) != out) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  CPU[CPUNUM+1] = 0;
  strcat(CPU,   "cpufreq/policy0/scaling_max_freq");
  /*     CPUNUM+0123456789012345*/
  CPU[CPUNUM+15] = '0' + cpu;
  
  out = 0;
  tmp = max;
  while(tmp)
  {
    ++out;
    tmp /= 10;
  }
  f = fopen(CPU, "w");
  if(f == NULL) return -1;
  if(fprintf(f, "%d", max) != out) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return 0;
}

int govcpu(int cpu, char *outgov)
{
  FILE* f;
  
  CPU[CPUNUM+1] = 0;
  strcat(CPU,   "cpufreq/policy0/scaling_governor");
  /*     CPUNUM+0123456789012345*/
  CPU[CPUNUM+15] = '0' + cpu;
  
  f = fopen(CPU, "r");
  if(f == NULL) return -1;
  if(fscanf(f, "%s", outgov) != 1) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return 0;
}

int govset(int cpu, const char *gov)
{
  int out = strlen(gov);
  FILE* f;
  
  CPU[CPUNUM+1] = 0;
  strcat(CPU,   "cpufreq/policy0/scaling_governor");
  /*     CPUNUM+0123456789012345*/
  CPU[CPUNUM+15] = '0' + cpu;
  
  f = fopen(CPU, "w");
  if(f == NULL) return -1;
  if(fprintf(f, "%s", gov) != out) 
  {
    fclose(f);
    return -2;
  }
  fclose(f);
  
  return 0;
}

