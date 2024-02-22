/*
    SPDX-FileCopyrightText: 2024 Stanislav Bulgakov
 
    SPDX-License-Identifier: MIT
*/

#ifndef  ACPI_H
#define  ACPI_H


typedef enum
{
  Discharging,
  Charging,
  Full,
  Unknown
} BatStatus;

/* int readbat(int)
 * attempts to read battery information
 *   in:      battery number [0-9]
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 *
 * int batcap()
 * returns battery 'capacity' information read by previous readbat() call
 *   return:  [0-100] percentage
 *           -1 readbat() failed to get information
 *
 * int batpres()
 * returns battery 'present' information read by previous readbat() call
 *   return:  0 absent
 *            1 present
 *
 * int batstat()
 * returns battery 'status' information read by previous readbat() call
 *   return: BatStatus
 *           -1 readbat() failed to get information
 */
int readbat(int batnum);
int batcap();
int batpres();
int batstat();


/* int readadp(int)
 * attempts to read power adapter information
 *   in:      adapter number [0-9]
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 * 
 * int adponline()
 * returns adapter 'online' information read by previous readadp() call
 *   return:  0 offline
 *            1 online fixed
 *            2 online programmable
 *           -1 readadp() failed to get information
 */
int readadp(int adpnum);
int adponline();


/* int testmon(int, char*)
 * tests hardware monitor for presence of temperature information
 *   in:      monitor number [0-9]
 *   out:     monitor name
 *   return:  0 absent
 *            1 present
 *            2 name not available
 * 
 * int readmon(int)
 * attempts to read hardware monitor temperature information
 *   in:      monitor number [0-9]
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 * 
 * double montemp1()
 * returns monitor 'temp1_input' information read by previous readmon() call
 *   return:  millidegree Celsius
 *           -1.0 readmon() failed to get information
 */
int    testmon(int monnum, char *outname);
int    readmon(int monnum);
double montemp1();


/* int findlit(char*)
 * attempts to find backlight device
 *   out:     backlight device name
 *   return:  0 found
 *            1 not found
 *           -1 information not available
 * 
 * int readlit(const char*)
 * attempts to read backlight device brightness information
 *   in:      backlight device name
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 * stores backlight device name for use with litset(), see also litnam()
 * 
 * int litact()
 * returns 'actual_brightness' information read by previous readlit() call
 *   return:  [0, max_brightness]
 *           -1 readlit() failed to get information
 *
 * int litmax()
 * returns 'max_brightness' information read by previous readlit() call
 *   return: >0
 *           -1 readlit() failed to get information
 *
 * int litnam(const char*)
 * stores backlight device name for use with litset(), see also readlit()
 *   in:      backlight device name
 *   return:  0 on success
 * 
 * int litset(int)
 * writes 'brightness' information to backlight device which name was stored 
 * by previous readlit() or litnam() call
 *   in:      brightness
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 */
int findlit(char *outname);
int readlit(const char *name);
int litact();
int litmax();
int litnam(const char *name);
int litset(int brightness);


/* int cpupres()
 * attempts to read CPU presence information
 *   return: [0, kernel_max] on success
 *           -1 information not available
 *           -2 i/o error
 * 
 * int freqmax(int cpu)
 * int freqmin(int cpu)
 * attempts to read CPU max/min available frequency information
 *   return: >0 Hz on success
 *           -1 information not available
 *           -2 i/o error
 * 
 * int freqset(int cpu, int min, int max);
 * writes min/max frequency information to CPU 
 *   in:      CPU number [0-9]
 *   in:      frequency min, max Hz
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 * 
 * int govcpu(int, char*)
 * attempts to read CPU governor information
 *   in:      CPU number [0-9]
 *   out:     governor name
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 *
 * int govset(int, const char*)
 * writes governor information to CPU 
 *   in:      CPU number [0-9]
 *   in:      governor name
 *   return:  0 on success
 *           -1 information not available
 *           -2 i/o error
 */
int cpupres();
int freqmax(int cpu);
int freqmin(int cpu);
int freqset(int cpu, int min, int max);
int govcpu(int cpu, char *outgov);
int govset(int cpu, const char *gov);

#endif /*ACPI_H*/

