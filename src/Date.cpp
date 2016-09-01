//  Date.cpp
//  Agenda
//
//  Created by Yoshi Park 16/08/18.
//  Copyright  2016 Yoshi Park. All rights reserved.
//

#include"Date.hpp"
#include<string>
#include<sstream>
#include<cctype>
inline bool is_format(std::string other) {
    if (other.length() == 16) {
        for (auto i = 0; i < other.length(); ++i) {
            if (i == 0 || i == 1 || i == 2 || i == 3 || i == 5 || i == 6 || i == 8 || i == 9 || i == 11 || i == 12 || i == 14 || i == 15) {
                if (!isdigit(other[i])) return false;
            }
            if (i == 4 || i == 7) {
                if (other[i] != '-') return false; 
            }
            if (i == 10) {
                if (other[i] != '/') return false;
            }
            if (i == 13) {
                if (other[i] != ':') return false;
            }
        }
        return true;
    } 
    return false;
}
Date::Date() :m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {}
Date::Date(int y, int m, int d, int h, int mi) : m_year(y), m_month(m), m_day(d), m_hour(h), m_minute(mi) {}
int Date::getYear(void) const { return m_year; }
void Date::setYear(int year) { m_year = year; }
int Date::getMonth(void) const { return m_month; }
void Date::setMonth(int month) { m_month = month; }
int Date::getDay(void) const { return m_day; }
void Date::setDay(int day) { m_day = day; }
int Date::getHour(void) const { return m_hour; }
void Date::setHour(int hour) { m_hour = hour; }
int Date::getMinute(void) const { return m_minute; }
void Date::setMinute(int minute) { m_minute = minute; }
inline bool is_leap_year(int year) {
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) return true;
    return false;
}
inline bool is_big_month(int m) {
    if (m == 1 || m == 3 || m == 5 || m == 7  || m == 8 || m == 10 || m == 12) return true;
    return false;
}
 Date::Date(std::string dateString) {
    if (is_format(dateString)) {
        m_year = Date::stringToDate(dateString).m_year;
        m_month = Date::stringToDate(dateString).m_month;
        m_day = Date::stringToDate(dateString).m_day;
        m_hour = Date::stringToDate(dateString).m_hour;
        m_minute = Date::stringToDate(dateString).m_minute;
    } else {
        m_year = m_month = m_day = m_hour = m_minute = 0;
    }
 }
bool Date::isValid(Date date) {
    if (date.getYear() < 1000 || date.getYear() > 9999 || date.getMonth() < 1 || date.getMonth() > 12 || date.getDay() < 1 || date.getDay() > 31 || date.getHour() < 0 || date.getHour() > 23 || date.getMinute() < 0  || date.getMinute() > 59) return false;
    if (date.getMonth() == 2) {
        if (is_leap_year(date.getYear()) && (date.getDay() < 1 ||date.getDay() > 29)) return false;
        if (!is_leap_year(date.getYear()) && (date.getDay() < 1 ||date.getDay() > 28)) return false;
        if (date.getHour() < 0 || date.getHour() > 23 || date.getMinute() < 0 || date.getMinute() > 59) return false;
        return true;
    }
    if (is_big_month(date.getMonth())) {
        if (date.getDay() < 1 || date.getDay() > 31) return false;
        if (date.getHour() < 0 || date.getHour() > 23 || date.getMinute() < 0 || date.getMinute() > 59) return false;
        return true;
    }
    else
    {
        if (date.getDay() < 1 || date.getDay() > 30) return false;
        if (date.getHour() < 0 || date.getHour() > 23 || date.getMinute() < 0 || date.getMinute() > 59) return false;
        return true;
    }

}
Date Date::stringToDate(std::string dateString) {
    if (!is_format(dateString)) {
        return Date(0,0,0,0,0);
    }
    std::string year = dateString.substr(0, 4);
    std::string month = dateString.substr(5, 2);
    std::string day = dateString.substr(8, 2);
    std::string hour = dateString.substr(11, 2);
    std::string minute = dateString.substr(14, 2);
    Date temp(atoi(year.c_str()), atoi(month.c_str()), atoi(day.c_str()), atoi(hour.c_str()), atoi(minute.c_str()));
    return temp;
}
inline std::string int_2_string(int number) {
    std::stringstream ss;
    std::string str;
    ss << number;
    ss >> str;
    if (number >= 10) return str;
    return "0" + str;
}
std::string Date::dateToString(Date date) {
    if (!isValid(date)) {
        return "0000-00-00/00:00";
    }
    std::string date_str = int_2_string(date.m_year) + "-" + int_2_string(date.m_month) + "-" + int_2_string(date.m_day) + "/" + int_2_string(date.m_hour) + ":" + int_2_string(date.m_minute);
    return date_str;
}
Date & Date::operator=(const Date& date) {
    if (this != &date) {
        m_year = date.m_year;
        m_month = date.m_month;
        m_day = date.m_day;
        m_hour = date.m_hour;
        m_minute = date.m_minute;
    }
    return *this;
}
bool Date::operator==(const Date& date) const {
    if (m_year == date.m_year && m_month == date.m_month && m_day == date.m_day && m_hour == date.m_hour && m_minute == date.m_minute) return true;
    return false;
}
bool Date::operator>(const Date& date) const {
    if (m_year > date.m_year) return true;
    else if (m_year == date.m_year) {
        if (m_month > date.m_month) return true;
        else if (m_month == date.m_month) {
            if (m_day > date.m_day) return true;
            else if (m_day == date.m_day) {
                if (m_hour > date.m_hour) return true;
                else if (m_hour == date.m_hour) {
                    if (m_minute > date.m_minute) return true;
                    return false;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool Date::operator<(const Date& date) const {
    return (!(*this == date) && !(*this > date));
}
bool Date::operator>=(const Date& date) const {
    return ((*this == date) || (*this > date));
}
bool Date::operator<=(const Date& date) const {
    return ((*this == date) || (*this < date));
}