/*
*@brief 计算时间
*@author yanning
*@date 2022-04-20
*/
#ifndef MSTIME_H
#define MSTIME_H
#include <stdint.h>
#include <time.h>

namespace serverco
{
    class Time
    {
    public:  
        Time(int64_t msSinceEpoch) : _timeVal(msSinceEpoch) {}
        Time(const Time& time) { _timeVal = time._timeVal; }
        Time(const Time&& time) { _timeVal = time._timeVal; }

        Time& operator=(const Time& time)
        {
            _timeVal = time._timeVal;
            return *this;
        }
        ~Time() {}
        // 更新1970年1月1日到现在的毫秒数
        static Time now();

        // 1970年1月1日到现在的秒数
        static time_t nowSec();

        // 根据距离1970年1月1日的秒数与秒数所属时区的时差计算当前时区的时间
        static void toLocalTime(time_t second, long timezone, struct tm* tm_time);

        //上次调用到现在的时间间隔
        struct timespec timeIntervalFromNom();

        // 获取到现在的毫秒数
        int64_t getTimeVal() { return _timeVal; }
    private:  
        int64_t _timeVal;
    };

    inline bool operator < (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() < rhs.getTimeVal();
    }

    inline bool operator <= (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() <= rhs.getTimeVal();
    }

    inline bool operator > (Time lhs, Time rhs)
	{
		return lhs.getTimeVal() > rhs.getTimeVal();
	}

	inline bool operator >= (Time lhs, Time rhs)
	{
		return lhs.getTimeVal() >= rhs.getTimeVal();
	}

	inline bool operator == (Time lhs, Time rhs)
	{
		return lhs.getTimeVal() == rhs.getTimeVal();
	}
}

#endif