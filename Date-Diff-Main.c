#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>


//This is a date difference calculator/countdown timer
//This program operates in Coordinated Universal Time (UTC), so difference dates are not tuned to
//local time zones of user

//Due to the limitations of C/CS50, only a 32 bit integer can be stored for time difference in seconds
//so calculator is only accurate up to 68 YEAR(S) 35 DAY(S) 3 HOUR(S) 14 MINUTE(S), 8 SECOND(S) from
//current date to past and 17 YEAR(S) 255 DAY(S) 23 HOUR(S) 59 MINUTE(S), 59 SECOND(S) from current date
//to future as of 4/24/20
//Times will slowly even out as time passes


//global variable used to designate turning from a struct tm (string)
//into an int that is used for later math with difftime() in difference() function   
  int tartimemath;

//Declaring ints to be used in convertdiff & printtime
//only diff needs to be a double as it deals with raw amount of seconds of
//target date from Epoch (Jan. 1, 1970)
//which then converted into much nicer numbers for various time intervals which can use ints    

  double diff;  //double can only store up to so many seconds so program
                //does have limits on target dates    
  int seconds;    
  int minutes;    
  int hours;    
  int days;    
  int years;

//create arrays to store date inputs as strings from fgets    
  char inputmonth[4];   
  char inputday[4];   
  char inputyear[5];  
  char inputhour[4];  
  char inputmin[4];    
  char inputsec[4];

//Functions need declaration before use
//but can be defined after
  void systime();
  void targettime();
  void difference();
  void printtime();
  void findtime();
  void initvars();

int main(int argc, char *argv[])
{
//ask user to put in dates with manner program can work with
  printf("\nUsing MM/DD/YYYY and HH:MM:SS\n\nInsert target date: ");

//asks and records input from user into aforementioned arrays,
//then gets rid of null buffer in that string
  printf("\n\nMonth (01-12): ");
  fgets(inputmonth, 4, stdin);    
  inputmonth[strlen(inputmonth) - 1] = '\0';
  //range from 1 to 12
  
  printf("Day (01-31): ");
  fgets(inputday, 4, stdin);    
  inputday[strlen(inputday) - 1] = '\0';
  //range from 1 to 31, depending on month
  
  printf("Year: ");
  fgets(inputyear, 6, stdin);    
  inputyear[strlen(inputyear) - 1] = '\0';
  //range can be anything within diff's bounds
  
  printf("Hour (00-23): ");
  fgets(inputhour, 6, stdin);    
  inputhour[strlen(inputhour) - 1] = '\0';
  //range from 0 to 23
  
  printf("Min (00-59): ");
  fgets(inputmin, 6, stdin);    
  inputmin[strlen(inputmin) - 1] = '\0';
  //range from 0 to 59
  
  printf("Sec (00-59): ");
  fgets(inputsec, 5, stdin);    
  inputsec[strlen(inputsec) - 1] = '\0';
  //range from 0 to 59
  
  targettime(); //converts user input of target date into ints that asctime() function can understand,
                //then prints target date
                //Must be run outside of a larger function otherwise cannot get day of week on its own
  
  initvars();   //initializes variables so findtime knows when to stop and start running (when diff != 0)
  
  findtime();   //calculates difference with updated time, prints, then waits 1 second,
                //only when there is a difference between current and target time
}

//creates datatype of time_t so time system values can be stored
//then finds and prints the current date as a string
void systime(void)
{    
  time_t sysresult;
  
  printf("Current date:  ");
  sysresult = time(NULL); //time(NULL) is command that grabs number of seconds since Epoch (Jan. 1, 1970 00:00:00 UTC)
  printf("%s", asctime(localtime(&sysresult)));
}

//creates struct of tm for use of time vars shown here: http://www.cplusplus.com/reference/ctime/tm/ 
//then converts user input for target date to integers
void targettime(void)
{    
  struct tm t;    
  
  t.tm_mon = atoi(inputmonth) - 1;    
  t.tm_mday = atoi(inputday);   
  t.tm_year = atoi(inputyear) - 1900;   
  t.tm_hour = atoi(inputhour);  
  t.tm_min = atoi(inputmin);   
  t.tm_sec = atoi(inputsec);
  
  printf("\nTarget date:   ");
  printf("%s", asctime(&t));
  
//converts &t to a int, seconds from Epoch (Jan 1, 1970), so it can be used with difftime to
//find difference in difference()    
  tartimemath = mktime(&t);
}

//finds the difference in seconds between current date and target date
//then converts the time in seconds to time in minutes, hours, days, and years for use in printtime()
void difference(void)
{    
  diff = difftime((time(NULL)), tartimemath);   
  minutes = (int)diff / 60;   
  hours = (int)diff / 3600;  
  days = (int)diff / 86400;  
  years = (int)diff / 31536000;
}

//breaks down time intervals into years, days, hours, minutes, and seconds depending on size of diff
//then prints difference
void printtime(void)
{
  //creates local variables that are then used to break down time into smaller intervals    
  int hoursRemainder;   
  int daysRemainder;   
  int yearsRemainder;
  
  printf("\nDifference: ");
 
  //if target date is in the future, diff is
  //assigned a negative number; this changes diff
  //to a positive double usable by the program
  if (diff < 0)
  {        
    diff *= -1;
  }
  
  //if difference is less than 1 min
  if(diff < 60)
  {        
    int diffint = (int) diff;
    printf("%d SECOND(S)", diffint);
  }
  
  //if difference is less than 1 hour
  else if (diff < 3600)
  {
    //finds leftover seconds from minutes
    //need to convert diff to int for use with modulo        
    seconds = (int)diff % 60;
    printf("%d MINUTE(S), %d SECOND(S)", minutes, seconds);
  }
  
  //if difference is less than 1 day
  else if (diff < 86400)
  {
    //finds leftover seconds from hours and makes it minutes       
    hoursRemainder = (int)diff % 3600;     
    minutes = hoursRemainder / 60;   
    seconds = (int)diff % 60;
    printf("%d HOUR(S) %d MINUTE(S), %d SECOND(S)", hours, minutes, seconds);
  }
    
  //if difference is less than 1 year
  else if (diff < 31536000)
  {
    //finds leftover seconds from days and breaks it down      
    daysRemainder = (int)diff % 86400;    
    hours = daysRemainder / 3600;    
    hoursRemainder = (int)(diff - 86400) % 3600;  
    minutes = hoursRemainder / 60;    
    seconds = (int)diff % 60;
    printf("%d DAY(S) %d HOUR(S) %d MINUTE(S), %d SECOND(S)", days, hours, minutes, seconds);
  }
      
  //if difference is more than 1 year
  else
  {
    //finds leftover seconds from years and breaks it down    
    yearsRemainder = (int)diff % 31536000;     
    days = yearsRemainder / 86400;     
    daysRemainder = (int)diff % 86400;  
    hours = daysRemainder / 3600;    
    hoursRemainder = (int)(diff - 86400) % 3600;    
    minutes = hoursRemainder / 60;       
    seconds = (int)diff % 60;
    printf("%d YEAR(S) %d DAY(S) %d HOUR(S) %d MINUTE(S), %d SECOND(S)", years, days, hours, minutes, seconds);
  }
}

//As long as diff != 0 (when there is a difference betweeen current and target date)
//calculates difference with updated time, prints, then waits 1 second and repeats
void findtime(void)
{
  while(diff != 0)
  {
    difference();
    printtime();
    printf("\n");
    sleep(1);  //program waits here for 1 second
  }
  
  //when target date is equal to current date
  //prints a message and then exits the program
  printf("\n\nTarget date has been hit");
  exit(0);
}
  
//initializes variables (mainly diff) so findtime knows when to stop and start running (when diff != 0)
//runs only once when starting program first starts
void initvars(void)
{
  printf("\n");
  systime();  //finds system time
  difference();  //calculates difference between current and target time
}
