
#include "time.hpp"
#include "tstrmem.hpp"

	bool IsLeapYear(int Year) { return (!(Year & 3) && Year != (Year/100)*100) || Year == (Year/400)*400; }
	int LeapYears(int Year) { return (Year/4) - Year/100 + Year/400 - 477; }

	char *Time2Str(unsigned long TimeStamp, char *szBuffer)
	{
		int DayInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

		unsigned long nHour = TimeStamp / 86400;
		unsigned long Hour = TimeStamp % 86400 / 3600;
		unsigned long nMinute = TimeStamp % 86400 % 3600;
		unsigned long Minute = nMinute / 60;
		unsigned long Year = TimeStamp / 86400 / 365 + 1970;
		unsigned long Second = nMinute % 60;
		unsigned long nDays = TimeStamp / 86400 % 365;
		unsigned long nDaysLeapYear = LeapYears(Year); //(Year/4) - Year/100 + Year/400 - 477;
		if (IsLeapYear(Year))// (!(Year & 3) && Year != (Year/100)*100) || Year == (Year/400)*400)
			--nDaysLeapYear;

		if (nDays < nDaysLeapYear)
			--Year;

		unsigned long v7 = 365*(Year-1970) + LeapYears(Year); //(Year/4) - Year/100 + Year/400 - 477;
		if (IsLeapYear(Year))//  (!(Year & 3) && Year != (Year/100)*100) || Year == (Year/400)*400)
		{
			--v7;
			DayInMonth[1] = 29;
		}

		unsigned long Day = nHour - v7;
		unsigned long Month;
		for (Month = 0; Month < 12; ++Month)
		{
			unsigned long v10 = DayInMonth[Month];
			if (Day < v10) break;
			Day -= v10;
		}
		TSPrintf(szBuffer, "%02d/%02d/%d %02d:%02d:%02d",
			(unsigned short)(Day + 1), (unsigned short)(Month + 1), (unsigned short)Year,
			(unsigned short)Hour, (unsigned short)Minute, (unsigned short)Second);
		return szBuffer;
	}
