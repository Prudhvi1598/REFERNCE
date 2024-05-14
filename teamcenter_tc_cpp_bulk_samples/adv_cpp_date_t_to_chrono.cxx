/*
The following code is a C++14 or greater using the ITK C++ interface. (VS2019)

The code demonstrates how to convert 'date_t' to chrono 'time_point' and 'time_point' to 'date_t'.

The advantage is you can then use chrono duration +,-,*,/ to easily calculate new time_points and use these new date_t values in your Teamcenter queries.

An example is provided on how to calculate 1/4 and 3/4 duration between tp1 and tp2 and then add the durations together to show there is only 1 second difference to the original tp2. 

Author: Patrick Hoonhout
Date: 02/03/2023
*/


#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

#if _HAS_CXX20 < 1
#include "date.h" // Adv. chrono support for c++17 and c++14: https://github.com/HowardHinnant/date
using namespace date;
#endif 

#include <unidefs.h>
#include <fclasses/tc_date.h>
#include <base_utils/Mem.h>
#include <base_utils/ScopedSmPtr.hxx>


std::string DateToString(const date_t& date)
{
    const char* dateFormat = "%Y-%m-%d T%H:%M:%S";
    Teamcenter::scoped_smptr<char> date_str;

    DATE_date_to_string(date, dateFormat, &date_str);

    if (date_str.get() != nullptr)
        return std::string(date_str.get());
  
    return std::string();
}


int main()
{
    const auto date_t_to_time_point = [](const date_t& date) {
        using namespace std::chrono;
        return time_point_cast<seconds>(sys_time<days>(year(date.year) / month(date.month + 1) / day(date.day))) + hours(date.hour) + minutes(date.minute) + seconds(date.second); // date_t 'month' is zero based (0 to 11); chrono 'month' is one based, (1 to 12)
    };

    const auto time_point_to_date_t = [](const auto& tp) {
        using namespace std::chrono;
        const auto date_p{ time_point_cast<days>(tp) };
        const auto time_d{ tp - date_p };
        const auto hour_d{ duration_cast<hours>(time_d) };
        const auto minute_d{ duration_cast<minutes>(time_d - hour_d) };
        const auto second_d{ duration_cast<seconds>(time_d - hour_d - minute_d) };
        const year_month_day ymd{ date_p };

        return date_t{ static_cast<short>(static_cast<int>(ymd.year())),
                        static_cast<byte>(static_cast<unsigned int>(ymd.month()) - 1), // date_t 'month' is zero based (0 to 11); chrono 'month' is one based, (1 to 12)
                        static_cast<byte>(static_cast<unsigned int>(ymd.day())),
                        static_cast<byte>(hour_d.count()),
                        static_cast<byte>(minute_d.count()),
                        static_cast<byte>(second_d.count()) };
    };

    const date_t date1 { 2023, 4, 30, 0, 36, 2 }; 
    const date_t date2 { 2023, 5, 25, 8, 4, 45 };

    auto tp1 = date_t_to_time_point(date1);
    auto tp2 = date_t_to_time_point(date2);

    auto dur = (tp2 - tp1) / 4; // 1/4 duration, use chrono for time calulations.
    auto dur2 = ((tp2 - tp1) * 3) / 4; // 3/4 duration, use chrono for time calulations.

    auto date3 = time_point_to_date_t(tp1 + (tp2 - tp1) / 2); // calculate date half-way between tp1 and tp1 and convert to date_t.

    std::cout << "Date: " << DateToString(time_point_to_date_t(tp1)) << " (tp1)" << std::endl;
    std::cout << "Date: " << DateToString(time_point_to_date_t(tp2)) << " (tp2)" << std::endl;
    std::cout << "Date: " << DateToString(time_point_to_date_t(tp1 + dur + dur2)) << " (add 1/4 and 3/4 duration to tp1, +-1 second difference to tp2)" << std::endl;
    std::cout << "Date: " << DateToString(date3) << " (mid-time between tp1 & tp2)" << std::endl;
}