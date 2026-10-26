/***********************************************************************************
FILE NAME 		timer_rtc.h
DESCRIPTION 	Real Time Clock functionality header file

Copyright : 2015 Renesas Electronics Europe Ltd.
Copyright : 2015 Renesas Electronics Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
***********************************************************************************/

#ifndef TIMER32K_H
#define TIMER32K_H

/***********************************************************************************/
//Function Prototypes
/***********************************************************************************/
void Timer32k_init(void);
void TimeUpdate(EVENT_MSG const *pMsg);

/**********************************************************************************/
//End of function prototypes section
/**********************************************************************************/
extern char time_str[35];
typedef struct {
	int tm_sec;       /*  seconds after the minute    */
	int tm_min;       /*  minutes after the hour      */
	int tm_hour;      /*  hours since midnight        */
	int tm_mday;      /*  day of the month            */
	int tm_mon;       /*  months since January        */
	int tm_year;      /*  years since 1900            */
	int tm_wday;      /*  days since Sunday           */
	int tm_yday;      /*  days since January 1        */
	int tm_isdst;     /*  daylight savings time flag  */
	int tm_am_pm;	  //	am or pm
}tm;
extern tm time;

#define T_AM		0
#define T_PM		1

#define JAN		0
#define FEB		1
#define MAR		2
#define APR		3
#define MAY		4
#define JUN		5
#define JUL		6
#define	AUG		7
#define SEP		8
#define OCT		9
#define NOV		10
#define DEC		11

#define SUN		0
#define MON		1
#define TUE		2
#define WED		3
#define THU		4
#define FRI		5
#define SAT		6

extern const char day_of_week[7][4];//= {"SUN","MON","TUE","WED","THU","FRI","SAT"};
extern const char month[12][4];//={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
extern const char am_pm[2][3];//={"AM","PM"};	
// MAIN_TMR32K_H 
#endif