/**
 *  Before using this test and writing your own test,
 *  you should first know how to write gtest to test your own code.
 *  We strongly recommend you going through the intro on the wiki.
 */

#include <gtest/gtest.h>
#include <string>
#include "Date.hpp"
// #include "User.hpp"
// #include "Meeting.hpp"

using std::string;

TEST(DateTest, TestSample) {
    Date blankDate;
    EXPECT_EQ(blankDate, Date::stringToDate("16-07-06/12:43"));
    Date date1(2016, 7, 6, 17, 19);
    EXPECT_EQ("2016-07-06/17:19", Date::dateToString(date1));
    EXPECT_FALSE(Date::isValid(Date("1800-02-29/00:01")));
    //  Go on to write your own test
}

TEST(UserTest, TestSample) {
    //  Implement your own test here
}


TEST(MeetingTest, TestSample) {
    //  Implement your own test here
}