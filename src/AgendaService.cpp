//  Service.cpp
//  Agenda
//
//  Created by Yoshi Park 16/08/18.
//  Copyright  2016 Yoshi Park. All rights reserved.
//

#include <iostream>
#include "AgendaService.hpp"
#include "MyException.cpp"
#include <algorithm>
#include <set>
/*Storage *m_storage;*/
AgendaService::AgendaService(){ startAgenda(); }
AgendaService::~AgendaService() {  }
bool AgendaService::userLogIn(std::string userName, std::string password) {
    std::function<bool(const User&)> filter = [userName](const User& user)->bool {
        return userName == user.getName();
    };
    auto user_list = m_storage->queryUser(filter);
    if (user_list.empty()) {
        throw No_User_Error();
        return false;
    }
    for (auto iter = user_list.begin(); iter != user_list.end(); ++iter) {
        if ((*iter).getPassword() == password) return true;
    }
    throw Password_Error();
    return false;
}
bool AgendaService::userRegister(std::string userName, std::string password,
    std::string email, std::string phone) {
    std::function<bool(const User&)> filter = [userName](const User& user)->bool {
        return userName == user.getName();
    };
    auto user_list = m_storage->queryUser(filter);
    if (!user_list.empty()) {
        throw User_Exist_Error();
        return false;
    }
    m_storage->createUser(User(userName, password, email, phone));
    return true;
}
bool AgendaService::deleteUser(std::string userName, std::string password) {
    std::function<bool(const Meeting&)> meet_filter = [=](const Meeting& meeting)->bool {
        auto participator = meeting.getParticipator();
        auto iter = find(participator.begin(), participator.end(), userName);
        if (iter != participator.end() || userName == meeting.getSponsor()) return true;
        return false;
    };
    std::function<bool(const User&)> filter = [=](const User& user)->bool {
        if (userName == user.getName() && password == user.getPassword()) {
            m_storage->deleteMeeting(meet_filter);
            return true;
        }
        return false;
    };
    auto num = m_storage->deleteUser(filter);
    return num != 0;
}
// a user can only delete itself
std::list<User> AgendaService::listAllUsers(void) const {
    std::function<bool(const User&)> filter = [](const User& user)->bool {
        return true;
    };
    auto user_list = m_storage->queryUser(filter);
    return user_list;
}

bool AgendaService::createMeeting(const std::string userName, const std::string title,
    const std::string startDate, const std::string endDate,
    const std::vector<std::string> participator) {
    auto item = find(participator.begin(),participator.end(),userName);
    if (item != participator.end()) {
        throw Participator_Userin_Error();
        return false;
    }
    std::set<std::string> is_one;
    for (auto i = 0; i < participator.size(); ++i) {
        is_one.insert(participator[i]);
    }
    if (is_one.size() != participator.size()) {
        throw Participator_Repeat_Error();
        return false;
    }
    std::function<bool(const User&)> user_filter = [userName](const User& user)->bool {
        return userName == user.getName();
    };
    /*std::function<bool(const User&)> participator_filter = [participator](const User& user)->bool {
        //return participator == user.getName();
        auto iter = find(participator.begin(), participator.end(), user.getName());
        return iter != participator.end();
    }; */
    std::function<bool(const User&)> all_man = [](const User& user)->bool {
        return true;
    };
    auto allman = m_storage->queryUser(all_man);
    for (auto iter = participator.begin(); iter != participator.end(); ++iter) {
        bool is_in = false;
        for (auto it = allman.begin(); it != allman.end(); ++it) {
            if ((*it).getName() == *iter) {
                is_in = true;
                break;
            }
        }
        if (is_in == false) {
            throw Participator_Noin_Error();
            // std::cout << "No participator!\n";
            return false;
        }
    };
    auto user_ = m_storage->queryUser(user_filter);
    //auto partcipator_ = m_storage->queryUser(participator_filter);
    if (user_.empty()) {
        // std::cout << "username and partcipator not exists!\n";
        throw No_User_in_Error();
        return false; // judge if the username and partcipator exists.
    }

    auto prev = Date::stringToDate(startDate), next = Date::stringToDate(endDate);
    if (!(Date::isValid(prev) && Date::isValid(next))) {
        throw Date_Format_Error();
        // std::cout << "the date is not valid!\n";
        return false; // judge if the date valid.
        
    }
    if (prev >= next) {
        throw Date_SE_Error();
        return false;
    }

    std::function<bool(const Meeting&)> title_filter = [title](const Meeting& meeting)->bool {
        return title == meeting.getTitle();
    };
    auto title_ = m_storage->queryMeeting(title_filter);
    if (!title_.empty()) {
        throw Title_Error();
        // std::cout << "the title is exists!\n";
        return false;  // judge if the title exists.
    }


    auto related_people = participator;
    related_people.push_back(userName); 
    std::list<Meeting> related_meeting;
    std::function<bool(const Meeting&)> date1_filter = [=](const Meeting& meeting)->bool {
         auto every_partcipator = meeting.getParticipator();
         every_partcipator.push_back(meeting.getSponsor());
         for (auto iter = related_people.begin(); iter != related_people.end(); ++iter) {
            auto it = find(every_partcipator.begin(),every_partcipator.end(), *iter);
            if (it != every_partcipator.end()) return true;
         }
         return false;
    };

    related_meeting = m_storage->queryMeeting(date1_filter);
    for (auto iter = related_meeting.begin(); iter != related_meeting.end(); ++iter) {
        if (!((*iter).getEndDate() <= prev || next <= (*iter).getStartDate())) {
            throw Time_Conflict_Error();
            return false;
        }
    }
    m_storage->createMeeting(Meeting(userName, participator, prev, next, title));
    return true;
    /* std::function<bool(const Meeting&)> date1_filter = [=](const Meeting& meeting)->bool {
        auto iter = find(meeting.getParticipator().begin(), meeting.getParticipator().end(), userName);
        if (userName == meeting.getSponsor() || iter != meeting.getParticipator().end()) {
            if (meeting.getEndDate() <= prev || next <= meeting.getStartDate()) return false;
            return true;
        }
        return false;
    };
    std::function<bool(const Meeting&)> date2_filter = [=](const Meeting& meeting)->bool {
        for (auto it = participator.begin(); it != participator.end(); ++it) {
            auto iter = find(meeting.getParticipator().begin(), meeting.getParticipator().end(), *it);
            if (iter != meeting.getParticipator().end() || userName == meeting.getSponsor()) {
                if (meeting.getEndDate() <= prev || next <= meeting.getStartDate()) return false;
                return true;
            }
        }
        return false;
    }; */
    /* auto time_check_1 = m_storage->queryMeeting(date1_filter), time_check_2 = m_storage->queryMeeting(date2_filter);
    if (time_check_1.empty() && time_check_2.empty()) {
        m_storage->createMeeting(Meeting(userName, participator, prev, next, title));
        return true;
    }
    // std::cout << "the meeting time is overlap!\n";
    return false;  // judge if the meeting time is overlap.  */
}
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
    const std::string title) const {
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        auto participator = meeting.getParticipator();
        auto iter = find(participator.begin(), participator.end(), userName);
        if ((userName == meeting.getSponsor() || iter != participator.end()) && title == meeting.getTitle()) return true;
        return false;
    };
    auto query = m_storage->queryMeeting(filter);
    return query;
}
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
    const std::string startDate,
    const std::string endDate) const {
    auto prev = Date::stringToDate(startDate), next = Date::stringToDate(endDate);
    if ((!Date::isValid(prev) || !Date::isValid(next)) || prev > next) {
        std::list<Meeting> meet;
        return meet;
    } 
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        auto participator = meeting.getParticipator();
        auto iter = find(participator.begin(), participator.end(), userName);
        if ((userName == meeting.getSponsor() || iter != participator.end()) && !(prev > meeting.getEndDate() || next < meeting.getStartDate())) return true;
        return false;
    };
    auto query = m_storage->queryMeeting(filter);
    return query;
}
std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        auto participator = meeting.getParticipator();
        auto iter = find(participator.begin(), participator.end(), userName);
        if (userName == meeting.getSponsor() || iter != participator.end()) return true;
        return false;
    };
    auto query = m_storage->queryMeeting(filter);
    return query;
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        if (userName == meeting.getSponsor()) return true;
        return false;
    };
    auto query = m_storage->queryMeeting(filter);
    return query;
}
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string userName) const {
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        auto participator = meeting.getParticipator();
        auto iter = find(participator.begin(), participator.end(), userName);
        if (iter != participator.end()) return true;
        return false;
    };
    auto query = m_storage->queryMeeting(filter);
    return query;
}
bool AgendaService::deleteMeeting(std::string userName, std::string title) {
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        if (userName == meeting.getSponsor() && title == meeting.getTitle()) return true;
        return false;
    };
    auto num = m_storage->deleteMeeting(filter);
    return num != 0;
}
bool AgendaService::deleteAllMeetings(std::string userName) {
    std::function<bool(const Meeting&)> filter = [=](const Meeting& meeting)->bool {
        if (userName == meeting.getSponsor()) return true;
        return false;
    };
    auto num = m_storage->deleteMeeting(filter);
    return num != 0;
}

bool AgendaService::updatePassword(std::string username, std::string newpsd) {
    std::function<bool(const User&)> filter = [username](const User& user)->bool {
        if (username == user.getName()) return true;
        return false;
    };
    std::function<bool(User &)> switcher = [newpsd](User & use)->bool {
        use.setPassword(newpsd);
    };
    return (m_storage->updateUser(filter,switcher));
}

bool AgendaService::updateEmail(std::string username, std::string email) {
    std::function<bool(const User&)> filter = [username](const User& user)->bool {
        if (username == user.getName()) return true;
        return false;
    };
    std::function<bool(User &)> switcher = [email](User & use)->bool {
        use.setEmail(email);
    };
    return (m_storage->updateUser(filter,switcher));
}

bool AgendaService::updatePhone(std::string username, std::string phone) {
    std::function<bool(const User&)> filter = [username](const User& user)->bool {
        if (username == user.getName()) return true;
        return false;
    };
    std::function<bool(User &)> switcher = [phone](User & use)->bool {
        use.setPhone(phone);
    };
    return (m_storage->updateUser(filter,switcher));
}


void AgendaService::startAgenda(void) { m_storage = m_storage->getInstance(); }
void AgendaService::quitAgenda(void) { m_storage->sync();}
