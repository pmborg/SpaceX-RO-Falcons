// --------------------------------------------------------------------------------------------
// Filename: lib_date.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from:  - https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              Used to get into KOS the current real date that is beeing simulated.
// 20/Nov/2021
// --------------------------------------------------------------------------------------------
PARAMETER DATE_PRINT_AT IS 0.
PARAMETER DATE_PRINT_LINE IS 0.

function getFevdays
{
	Parameter year.
    
    // Apply leap year rules:
    if (MOD(year, 4) = 0) or (MOD(year, 100) = 0) or (MOD(year, 400) = 0) 
		set fevdays to 29.
	else
		set fevdays to 28.
	
	return fevdays.
}

set monthlist to list(	31, 28, 31, 30,
						31, 30, 31, 31,
						30, 31, 30, 31).

function getMonthDays
{
	Parameter month.	//0-11
	Parameter year.		//
	
	set daysPerMonth to monthlist[month].
	if month = 1
		set daysPerMonth to getFevdays(year).
		
	return daysPerMonth.
}

function getYearDays
{
	Parameter year.
	Parameter until_month is 12.
	
	set sum to 0.
	FROM { local counter is 0. } UNTIL counter = until_month STEP {SET counter to counter + 1.} DO 
	{
		set daysPerMonth to getMonthDays(counter, year).
		set sum to sum+daysPerMonth.
		//print monthlist[counter].
	}
	return sum.
}

function getNumberSecondsPerDay
{
	return KUniverse:HOURSPERDAY*60*60. //24*60*60
}

set monthSTR to list( "Jan", "Feb", "Mar", "Apr",
					  "May", "Jun", "Jul", "Aug",
					  "Sep", "Oct", "Nov", "Dec" ).

function PRINT_REALTIME
{
	PARAMETER START_YEAR is 1970.
	PARAMETER curret_epoch is KUniverse:REALTIME. //number of seconds since 1/1/1970

	// DEBUG:
	// clearscreen. print " ". print " ".
	
	//GET REAL YEAR FROM REALTIME:
	set present_year to 0.
	set epoch to curret_epoch.

	FROM { local y is START_YEAR. } UNTIL epoch <= 0 STEP {SET y to y + 1.} DO 
	{
		//print "Y: "+y.
		set yearDays to getYearDays(y).
		set yearSeconds to yearDays*getNumberSecondsPerDay().
		set epoch to epoch - yearSeconds.
		
		//print y+" getYearDays: "+yearDays+ " getYearSeconds: " +yearSeconds+ " epoch: "+(epoch).
		//LOG y+" getYearDays: "+yearDays+ " getYearSeconds: " +yearSeconds+ " epoch: "+(epoch) to TIME.TXT.
		
		set present_year to y.
	}
	//DEBUG
	// print "curret_epoch: "+curret_epoch.				
	// print "epoch: "+epoch.							// Number of seconds until the end of present date

	set total_seconds_this_year to getYearDays(present_year) * getNumberSecondsPerDay().
	set today_epoch to total_seconds_this_year+epoch.	// Number of seconds since beg. this present year

	set this_year to 0.
	//GET REAL MONTH FROM REALTIME:
	set present_month to 0.
	FROM { local m is 0. } UNTIL m > 11 STEP {SET m to m + 1.} DO 
	{
		set daysPerMonth to getMonthDays(m, present_year).				

		set secondsPerMonth to daysPerMonth*getNumberSecondsPerDay().
		set this_year to this_year+secondsPerMonth.

		// DEBUG:
		// print "m: "+m+" secondsPerMonth: "+secondsPerMonth + " this_year: "+this_year.
		
		if this_year >= today_epoch
		{
			set present_month to m+1.
			break.
		}
	}

	set NumberSecondsUntilTheEndofCurrentMonth to (this_year-today_epoch)/getNumberSecondsPerDay(). //Number of seconds until the end of current month
	set precise_day to (getMonthDays(present_month-1, present_year)-FLOOR(NumberSecondsUntilTheEndofCurrentMonth)).
	set precise_hours to 24*(FLOOR(NumberSecondsUntilTheEndofCurrentMonth)+1-NumberSecondsUntilTheEndofCurrentMonth).
	set precise_minutes to (60*(precise_hours-FLOOR(precise_hours))).
	
	// DEBUG:
	// print getYearDays (present_year, present_month)*getNumberSecondsPerDay().
	// print "DAYS: "+getMonthDays(present_month-1, present_year).
	// print NumberSecondsUntilTheEndofCurrentMonth.
	// print "YEAR: "+present_year.
	// print "MONTH: "+present_month.
	//DEBUG:
	// print "DAY: " + precise_day.
	// print "HOURS: "+ FLOOR(precise_hours).
	// print "MINUTES: "+ FLOOR(precise_minutes).

	print present_year + "/"+monthSTR[present_month-1] + "/" + precise_day + " "+ FLOOR(precise_hours) + ":"+ FLOOR(precise_minutes) at (DATE_PRINT_AT, DATE_PRINT_LINE).
}

//PRINT_REALTIME().						// PRINT REAL TIME
PRINT_REALTIME(1951, TIME:SECONDS).		// PRINT GAME TIME