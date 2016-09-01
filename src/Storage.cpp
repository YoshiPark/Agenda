// Storage.cpp
//  Agenda
//
//  Created by Yoshi Park 16/08/18.
//  Copyright  2016 Yoshi Park. All rights reserved.
//

#include"Storage.hpp"
#include"Path.hpp"
#include<fstream>
#include<iostream>
#include<cctype>
using std::cin;
using std::endl;
std::shared_ptr<Storage> Storage::m_instance = nullptr;
Storage::Storage():m_dirty(false) {
    readFromFile();
}
bool Storage::readFromFile(void) {
    std::ifstream userin(Path::userPath);
    // std::ifstream userin("User.csv");
    /* in.open(Path::userPath, std::ios::in);*/
    if (userin.fail()) return false;
    if (!userin.good()) return false;
    std::string user_info;
    while (!userin.eof()) {
        getline(userin, user_info);
        auto count = 0;
        std::string user_name, user_password, user_email, user_phone;
        while (!user_info.empty()) {
            auto f_pos = user_info.find("\"");
            if (f_pos == std::string::npos) break;
            auto iter = user_info.begin();
            for (size_t i = 0; i <= f_pos; ++i) ++iter;
            user_info.erase(user_info.begin(), iter);
            auto s_pos = user_info.find("\"");
            std::string temp = user_info.substr(0, s_pos);
            auto it = user_info.begin();
            for (size_t i = 0; i <= s_pos; ++i) ++it;
            user_info.erase(user_info.begin(), it);
            if (count == 0) {
                user_name = temp;
                count++;
            }
            else if (count == 1) {
                user_password = temp;
                count++;
            }
            else if (count == 2) {
                user_email = temp;
                count++;
            }
            else
            {
                user_phone = temp;
            }
        }
        if (user_name == "" || user_password == "" || user_email == "" || user_phone == "") {

        } else {
            m_userList.push_back(User(user_name, user_password, user_email, user_phone));
        }
        
    }
    userin.close();

    std::ifstream meetingin(Path::meetingPath);
    // std::ifstream meetingin("meeting.csv");
    if (meetingin.fail()) return false;
    if (!meetingin.good()) return false;
    std::string meeting_info;
    while (!meetingin.eof()) {
        getline(meetingin, meeting_info);
        if (meeting_info.empty()) break;
        auto count = 0;
        std::string meeting_sponsor, meeting_participator, meeting_sdate, meeting_edate, meeting_title;
        while (!meeting_info.empty()) {
            auto f_pos = meeting_info.find("\"");
            if (f_pos == std::string::npos) break;
            auto iter = meeting_info.begin();
            for (size_t i = 0; i <= f_pos; ++i) ++iter;
            meeting_info.erase(meeting_info.begin(), iter);
            auto s_pos = meeting_info.find("\"");
            std::string temp = meeting_info.substr(0, s_pos);
            auto it = meeting_info.begin();
            for (size_t i = 0; i <= s_pos; ++i) ++it;
            meeting_info.erase(meeting_info.begin(), it);
            if (count == 0) {
                meeting_sponsor = temp;
                count++;
            }
            else if (count == 1) {
                meeting_participator = temp;
                count++;
            }
            else if (count == 2) {
                meeting_sdate = temp;
                count++;
            }
            else if (count == 3)
            {
                meeting_edate = temp;
                count++;
            }
            else
            {
                meeting_title = temp;
            }
        }
        std::vector<std::string> meeting_part;
        while (!meeting_participator.empty()) {
            auto f_pos = meeting_participator.find("&");
            if (f_pos == std::string::npos) {
                auto t = meeting_participator;
                meeting_part.push_back(t);
                break;
            }
            auto list = meeting_participator.substr(0, f_pos);
            meeting_part.push_back(list);
            auto iter = meeting_participator.begin();
            for (size_t i = 0; i <= f_pos; ++i) ++iter;
            meeting_participator.erase(meeting_participator.begin(), iter);
        }
        if (meeting_sponsor == "" || meeting_participator.empty() || meeting_title == "") {

        } else{
            m_meetingList.push_back(Meeting(meeting_sponsor, meeting_part, Date::stringToDate(meeting_sdate), Date::stringToDate(meeting_edate), meeting_title));
        }
        
    }
    meetingin.close();
    // m_dirty = false;
    return true;
}

bool Storage::writeToFile(void) {
    if (m_dirty == false) return false;
    std::ofstream userout(Path::userPath);
    // std::ofstream userout("User.csv");
    /* out.open(Path::userPath,std::ios::out); */
    if (!userout.good()) return false;
    if (userout.fail()) return false;
    // userout << "\"name\",\"password\",\"email\",\"phone\"" << endl;
    for (auto iter = m_userList.begin(); iter != m_userList.end(); ++iter) {
        // std::cout << m_userList.size(); 
        userout << "\"" << (*iter).getName() << "\",\"" << (*iter).getPassword() << "\",\"" << (*iter).getEmail() << "\",\"" << (*iter).getPhone() << "\"" << endl;
        // std::cout << "\"" << (*iter).getName() << "\",\"" << (*iter).getPassword() << "\",\"" << (*iter).getEmail() << "\",\"" << (*iter).getPhone() << "\"" << endl;
    }
    userout.close();

    std::ofstream meetingout(Path::meetingPath);
    // std::ofstream meetingout("meeting.csv");
    if (!meetingout.good()) return false;
    if (meetingout.fail()) return false;
    // meetingout << "\"sponsor\",\"participator\",\"sdate\",\"edate\",\"title\"" << endl;
    for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); ++iter) {
        meetingout << "\"" << (*iter).getSponsor() << "\",\"";
        // std::cout << "\"" << (*iter).getSponsor() << "\",\"";
        for (auto num = 0; num < (*iter).getParticipator().size(); ++num) {
            meetingout << (*iter).getParticipator()[num];
            // std::cout << (*iter).getParticipator()[num];
            if (num != (*iter).getParticipator().size() - 1) meetingout << "&";
        }
        meetingout << "\",\"" << Date::dateToString((*iter).getStartDate()) << "\",\"" << Date::dateToString((*iter).getEndDate()) << "\",\"" << (*iter).getTitle() << "\"" << endl;
        // std::cout << "\",\"" << Date::dateToString((*iter).getStartDate()) << "\",\"" << Date::dateToString((*iter).getEndDate()) << "\",\"" << (*iter).getTitle() << "\"" << endl;
    }
    meetingout.close();
    m_dirty = false;
    return true;
}
std::shared_ptr<Storage> Storage::getInstance(void) {
    if (!m_instance) {
        m_instance.reset(new Storage());
    }
    return m_instance;
}
Storage::~Storage() { 
    sync(); 
    m_instance.reset();
    
}
void Storage::createUser(const User& user) { 
    m_dirty = true;
    m_userList.push_back(user);
}
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> found_user_list;
    for (auto iter = m_userList.begin(); iter != m_userList.end(); ++iter) {
        if (filter(*iter)) found_user_list.push_back(*iter);
    }
    return found_user_list;
}
// return found users
int Storage::updateUser(std::function<bool(const User&)> filter,
    std::function<void(User&)> switcher) {
    auto num = 0;
    for (auto iter = m_userList.begin(); iter != m_userList.end(); ++iter) {
        if (filter(*iter)) {
            num++;
            m_dirty = true;
            switcher(*iter);
        }
    }
    return num;
}
// return the number of updated users
int Storage::deleteUser(std::function<bool(const User&)> filter) {
    auto num = 0;
    auto iter = m_userList.begin();
    while (iter != m_userList.end()) {
        if (filter(*iter)) {
            num++;
            m_dirty = true;
            m_userList.erase(iter++);
        }
        else
        {
            iter++;
        }
    }
    return num;
}
// return the number of deleted users
void Storage::createMeeting(const Meeting& meeting) {
    m_dirty = true;
    m_meetingList.push_back(meeting);
}
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> found_meeting_list;
    for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); ++iter) {
        if (filter(*iter)) found_meeting_list.push_back(*iter);
    }
    return found_meeting_list;
}
// return found meetings
int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
    std::function<void(Meeting&)> switcher) {
    auto num = 0;
    for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); ++iter) {
        if (filter(*iter)) {
            num++;
            m_dirty = true;
            switcher(*iter);
        }
    }
    return num;
}
// return the number of updated meetings
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    auto num = 0;
    auto iter = m_meetingList.begin();
    while (iter != m_meetingList.end()) {
        if (filter(*iter)) {
            num++;
            m_dirty = true;
            m_meetingList.erase(iter++);
        }
        else
        {
            iter++;
        }
    }
    return num;
}
// return the number of deleted meetings
// File IO
bool Storage::sync(void) {
    return writeToFile();
}
