#pragma once
using namespace std;
using namespace this_thread;
namespace SomeTimeStuff {
	class time_
	{
	public:
		tm* track_time;
		bool pm = false, am = false;
		string hour, min, sec, wday, mday, month_num, month, year, century = "20", suffix, time, dropsec;
	}; time_ time;
	inline thread update_time()
	{
		while (true)
		{
			time.track_time = dpp::utility::mtm(std::time(0));
			if (time.track_time->tm_hour == 13) time.hour = "1", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 14) time.hour = "2", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 15) time.hour = "3", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 16) time.hour = "4", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 17) time.hour = "5", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 18) time.hour = "6", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 19) time.hour = "7", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 20) time.hour = "8", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 21) time.hour = "9", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 22) time.hour = "10", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 23) time.hour = "11", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 0) time.hour = "12", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 1) time.hour = "1", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 2) time.hour = "2", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 3) time.hour = "3", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 4) time.hour = "4", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 5) time.hour = "5", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 6) time.hour = "6", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 7) time.hour = "7", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 8) time.hour = "8", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 9) time.hour = "9", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 10) time.hour = "10", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 11) time.hour = "11", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 12) time.hour = "12", time.am = true, time.pm = false;
			if (time.track_time->tm_min < 10) time.min = "0" + to_string(time.track_time->tm_min);
			else time.min = to_string(time.track_time->tm_min);
			if (time.track_time->tm_sec < 10) time.sec = "0" + to_string(time.track_time->tm_sec);
			else time.sec = to_string(time.track_time->tm_sec);
			time.mday = to_string(time.track_time->tm_mday);
			if (time.track_time->tm_mday == 1 or time.track_time->tm_mday == 21 or time.track_time->tm_mday == 31) time.suffix = "st";
			else if (time.track_time->tm_mday == 2 or time.track_time->tm_mday == 22) time.suffix = "nd";
			else if (time.track_time->tm_mday == 3 or time.track_time->tm_mday == 23) time.suffix = "rd";
			else time.suffix = "th";
			time.year = to_string(time.track_time->tm_year);
			time.year.erase(time.year.begin());
			if (time.track_time->tm_wday == 0) time.wday = "Sunday";
			if (time.track_time->tm_wday == 1) time.wday = "Monday";
			if (time.track_time->tm_wday == 2) time.wday = "Tuesday";
			if (time.track_time->tm_wday == 3) time.wday = "Wednesday";
			if (time.track_time->tm_wday == 4) time.wday = "Thursday";
			if (time.track_time->tm_wday == 5) time.wday = "Friday";
			if (time.track_time->tm_wday == 6) time.wday = "Saturday";
			time.month_num = to_string(time.track_time->tm_mon + 1);
			if (time.track_time->tm_mon == 0) time.month = "January";
			if (time.track_time->tm_mon == 1) time.month = "February";
			if (time.track_time->tm_mon == 2) time.month = "March";
			if (time.track_time->tm_mon == 3) time.month = "April";
			if (time.track_time->tm_mon == 4) time.month = "May";
			if (time.track_time->tm_mon == 5) time.month = "June";
			if (time.track_time->tm_mon == 6) time.month = "July";
			if (time.track_time->tm_mon == 7) time.month = "August";
			if (time.track_time->tm_mon == 8) time.month = "September";
			if (time.track_time->tm_mon == 9) time.month = "October";
			if (time.track_time->tm_mon == 10) time.month = "November";
			if (time.track_time->tm_mon == 11) time.month = "December";
			time.time = time.hour + ":" + time.min + ":" + time.sec + " " + (time.pm ? "PM" : "") + (time.am ? "AM" : "");
			time.dropsec = time.hour + ":" + time.min + " " + (time.pm ? "PM" : "") + (time.am ? "AM" : "");
			sleep_for(200ms);
		}
		return thread();
	}
}