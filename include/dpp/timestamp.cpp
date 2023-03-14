/*
     everything you need for timestamps. This file is not part of the Official D++ library.
*/
#pragma once
#include <ctime>
using namespace std;

namespace dpp {
	/** make a tm from a time_t, then modified into a different time_t
		 * @param use std::time(0) for current time
		 */
	inline tm* mtm(time_t t)
	{
		long long _time = t;
		tm* types = localtime(&_time);
		return types;
	}
	/** make a modified time_t from a tm
		 * @param add(+) to go forward in time, subtract(-) to go back in time
		 */
	time_t mt_t(tm* old_time, int second, int minute, int hour, int wday, int mday, int month_num) {
		old_time->tm_sec = second;
		old_time->tm_min = minute;
		old_time->tm_hour = hour;
		old_time->tm_wday = wday;
		old_time->tm_mday = mday;
		old_time->tm_mon = month_num;
		return mktime(old_time);
	}
	class timestamp {
	public:
		static string short_t(time_t t) { return "<t:" + to_string(t) + ":t>"; }
		static string long_t(time_t t) { return "<t:" + to_string(t) + ":T>"; }
		static string short_d(time_t t) { return "<t:" + to_string(t) + ":d>"; }
		static string long_d(time_t t) { return "<t:" + to_string(t) + ":D>"; }
		static string long_d_short_t(time_t t) { return "<t:" + to_string(t) + ":f>"; }
		static string full(time_t t) { return "<t:" + to_string(t) + ":F>"; }
		static string relevant(time_t t) { return "<t:" + to_string(t) + ":R>"; }
	};
}