#include "common.h"
// // Format a int like 8 into a string like "08", and 13 into "13".
void string_formater(int source, char *dest)
{
	if (source < 10)
	{
		char  temp[3];
		strcpy(dest, "0");
		sprintf(temp, "%d", source);
		strcat(dest, temp);
	}
	else
		sprintf(dest, "%d", source);
}

// Build a time string like "2017-12-11T12:00:00.000Z".
void time_now_to_string(char *timestamp_now)
{
	time_t rawtime;
	struct tm *ptminfo;

	time(&rawtime);
	ptminfo = localtime(&rawtime);

	char stringyear[5], stringmonth[3], stringday[3], stringhour[3], stringmin[3], stringsec[3];

	//Format year, month, day, hour, min, sec into string.
	sprintf(stringyear, "%d", (ptminfo->tm_year + 1900));
	string_formater(ptminfo->tm_mon + 1, stringmonth);
	string_formater(ptminfo->tm_mday, stringday);
	string_formater(ptminfo->tm_hour, stringhour);
	string_formater(ptminfo->tm_min, stringmin);
	string_formater(ptminfo->tm_sec, stringsec);

	strcpy(timestamp_now, stringyear);
	strcat(timestamp_now, "-");
	strcat(timestamp_now, stringmonth);
	strcat(timestamp_now, "-");
	strcat(timestamp_now, stringday);
	strcat(timestamp_now, "T");
	strcat(timestamp_now, stringhour);
	strcat(timestamp_now, ":");
	strcat(timestamp_now, stringmin);
	strcat(timestamp_now, ":");
	strcat(timestamp_now, stringsec);
	strcat(timestamp_now, ".000Z");
}