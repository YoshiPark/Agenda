//  AgendaUI.cpp
//  Agenda
//
//  Created by Yoshi Park 16/08/18.
//  Copyright  2016 Yoshi Park. All rights reserved.
//
#include "AgendaUI.hpp"
#include "MyException.cpp"
#include<iostream>
#include<iomanip>
#include<time.h>
#include <cctype>
// include"Random.h"
using std::cout;
using std::cin;
using std::endl;
static auto interface = 0;
inline std::string participatorList(const std::vector<std::string> other) {
    std::string list;
    for (auto i = 0; i < other.size(); ++i) {
        list += other[i];
        if (i != other.size() - 1) list += ",";
    }
    return list;
}
AgendaUI::AgendaUI() :m_userName(), m_userPassword() {
    /* Random data; */
    startAgenda();
    m_agendaService.startAgenda();
}
void AgendaUI::OperationLoop(void) {
    executeOperation(getOperation());
}
void AgendaUI::startAgenda(void) {
    if (interface == 0) {
        cout << "-------------------- Agenda -----------------------" << endl;
        cout << "Action :" << endl;
        cout << "l    - log in Agenda by user name and password" << endl;
        cout << "r    - register an Agenda account" << endl;
        cout << "q    - quit Agenda" << endl;
        cout << "---------------------------------------------------" << endl << endl;
        cout << "Agenda :  ~#";
    }
    else
    {
        cout << "-------------------- Agenda --------------------" << endl;
        cout << "Action :" << endl;
        cout << "o   - log out Agenda" << endl;
        cout << "dc  - delete Agenda account" << endl;
        cout << "lu  - list all Agenda users" << endl;
        cout << "cm  - create meeting" << endl;
        cout << "la  - list all meetings" << endl;
        cout << "las - list all Sponsor meetings" << endl;
        cout << "lap - list all Participate meetings" << endl;
        cout << "qm  - query meeting by title" << endl;
        cout << "qt  - query meeting by timeInterval" << endl;
        cout << "dm  - delete meeting by title" << endl;
        cout << "da  - delete all meetings" << endl;
        cout << "upa - update password" << endl;
        cout << "upe - update email" << endl;
        cout << "uph - update phone" << endl;
        cout << "------------------------------------------------" << "\n\n";
        cout << "Agenda@" << m_userName << " :  ";
    }
}
std::string AgendaUI::getOperation() {
    std::string selection;
    cin >> selection;
    // getline(cin, selection);
    return selection;
}
bool AgendaUI::executeOperation(std::string op) {
    if (interface == 0) {
        if (op == "l") { userLogIn(); }
        else if (op == "r") { userRegister(); }
        else if (op == "q") { quitAgenda(); }
        else
        {
            cout << "Wrong Selection!\n";
            startAgenda();
            OperationLoop();
        }
    }
    else
    {
        if (op == "cm") { createMeeting(); }
        else if (op == "lu") { listAllUsers(); }
        else if (op == "la") { listAllMeetings(); }
        else if (op == "las") { listAllSponsorMeetings(); }
        else if (op == "lap") { listAllParticipateMeetings(); }
        else if (op == "qm") { queryMeetingByTitle(); }
        else if (op == "qt") { queryMeetingByTimeInterval(); }
        else if (op == "dc") { deleteUser(); }
        else if (op == "da") { deleteAllMeetings(); }
        else if (op == "dm") { deleteMeetingByTitle(); }
        else if (op == "o") { userLogOut(); }
        else if (op == "upa") { updatePassword();}
        else if (op == "upe") { updateEmail();}
        else if (op == "uph") { updatePhone();}
        else
        {
            cout << "Wrong Selection!\n";
            startAgenda();
            OperationLoop();
        }
    }
    return true;
}
void AgendaUI::userLogIn(void) {
    cout << endl;
    cout << "[log in] [user name] [password]" << endl;
    cout << "[log in] ";
    cin >> m_userName;  /* getline(cin, m_userName); */
    cin >> m_userPassword;  /* getline(cin, m_userPassword); */
    time_t start = clock();
    try {
         if (m_agendaService.userLogIn(m_userName, m_userPassword)) {
        cout << "[log in] succeed!" << endl;
        time_t end = clock();
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Log-in time is: " << timer << "s " << endl;
        interface = 1;

        }
        else
        {
        cout << "[error] log in fail!" << endl;
        interface = 0;
        }
        startAgenda();
        OperationLoop();
    }
    catch (No_User_Error & e){
        cout << "[error] log in fail!" << endl;
        e.what();
        startAgenda();
        OperationLoop();
    }
    catch (Password_Error & e) {
        cout << "[error] log in fail!" << endl;
        e.what();
        startAgenda();
        OperationLoop();
    }
    
}
void AgendaUI::userRegister(void) {
    std::string username, userpassword, useremail, userphone;
    cout << endl;
    cout << "[register] [user name] [password] [email] [phone]" << endl;
    cout << "[register] ";
    cin >> username;  //getline(cin, username);
    cin >> userpassword; // getline(cin, userpassword);
    cin >> useremail; //getline(cin, useremail);
    cin >> userphone;  //getline(cin, userphone);
    time_t start = clock();
    try {
        if (m_agendaService.userRegister(username, userpassword, useremail, userphone)) {
        cout << "[register] succeed!\n";
        time_t end = clock();
        interface = 0;
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Register time is: " << timer << "s " << endl;
        startAgenda();
        OperationLoop();
    }
    else
    {
        cout << "[register] register fail!\n";
        interface = 0;
        startAgenda();
        OperationLoop();
    }
    }
    catch (User_Exist_Error & e){
        cout << "[register] register fail!\n";
        e.what();
        startAgenda();
        OperationLoop();
    }
    // startAgenda();
    // OperationLoop();
}
void AgendaUI::quitAgenda(void) {
    m_agendaService.quitAgenda();
}
void AgendaUI::userLogOut(void) {
    cout << endl;
    cout << "[Log Out]:succeed!\n";
    interface = 0;
    startAgenda();
    OperationLoop();
}
void AgendaUI::deleteUser(void) {
    cout << endl;
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
        cout << "[delete agenda account] succeed!" << endl;
    }
    else
    {
        cout << "[delete agenda account] delete agenda account fail!\n";
    }
    interface = 0;
    startAgenda();
    OperationLoop();
}
void AgendaUI::listAllUsers(void) {
    time_t start = clock();
    cout << endl;
    cout << "[list all users]:\n";
    auto user_list = m_agendaService.listAllUsers();
    if (user_list.empty()) {
        cout << "[Result]:No users!" << endl;
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "name" << std::setw(20) << "email" << std::setw(20) << "phone" << endl;

        for (auto iter = user_list.begin(); iter != user_list.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getName() << std::setw(20) << (*iter).getEmail() << std::setw(20) << (*iter).getPhone() << endl;
        }
        time_t end = clock();
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Search time is: " << timer << "s " << endl;
    }

    interface = 1;
    startAgenda();
    OperationLoop();
}
inline bool is_format(std::string s) {
    for (auto i = 0; i < s.length(); ++i) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}
void AgendaUI::createMeeting(void) {
    cout << endl;
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int number;
    //std::string num_str;
    //cin >> num_str;
    /* while(!is_format(num_str)) {
        cout << "[create meeting] [the number of participators]" << endl;
        cout << "[create meeting] ";
        cin >> num_str;
    } */
    // int number = atoi(num_str.c_str());
    cin >> number;
    std::vector<std::string> partcipator_list;
    for (auto i = 0; i < number; ++i) {
        std::string participator;
        cout << "[create meeting] [please enter the participator " << i+1 << " ]" << endl;
        cout << "[create meeting] ";
        cin >> participator;//getline(cin, participator);
        partcipator_list.push_back(participator);
    }
    cout << "[title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    std::string title, start, end;
    cin >> title; //getline(cin, title);
    cin >> start; // getline(cin, start);
    /* cin >> title >> participator >> start >> end; */
    cin >> end;//getline(cin, end);
    time_t start_ = clock();
    try {
        if (m_agendaService.createMeeting(m_userName, title, start, end, partcipator_list)) {
        cout << "[create meeting] succeed!" << endl;
        time_t end_ = clock();
        double timer = double(end_ - start_) / CLOCKS_PER_SEC;
        cout << "The Creating meeting time is: " << timer << "s " << endl;
    }
    else
    {
        cout << "[create meeting] error!" << endl;
    }
       interface = 1;
    startAgenda();
    OperationLoop();
    }
    catch (Title_Error & e) {
      cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (Participator_Userin_Error & e) {
        cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (Participator_Repeat_Error & e) {
        cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (Participator_Noin_Error & e) {
        cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (No_User_in_Error & e) {
      cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (Date_Format_Error & e) {
      cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (Date_SE_Error & e) {
        cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
    catch (Time_Conflict_Error & e) {
        cout << "[create meeting] error!" << endl;
      e.what();
      interface = 1;
      startAgenda();
      OperationLoop();
    }
}
void AgendaUI::listAllMeetings(void) {
    time_t start = clock();
    cout << endl;
    cout << "[list all meetings]:\n";
    auto meet_list = m_agendaService.listAllMeetings(m_userName);
    if (meet_list.empty()) {
        cout << "[Result]:No meeting!\n";
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "title" << std::setw(20) << "sponsor" << std::setw(20) << "start time" << std::setw(20) << "end time" << std::setw(50) << "participator" << endl;
        for (auto iter = meet_list.begin(); iter != meet_list.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getTitle() << std::setw(20) << (*iter).getSponsor() << std::setw(20) << Date::dateToString((*iter).getStartDate()) << std::setw(20) << Date::dateToString((*iter).getEndDate()) << std::setw(50) << participatorList((*iter).getParticipator()) << endl;
        }
        time_t end = clock();
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Search time is: " << timer << "s " << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::listAllSponsorMeetings(void) {
    time_t start = clock();
    cout << endl;
    cout << "[list all sponsor meetings]:\n";
    auto meet_list = m_agendaService.listAllSponsorMeetings(m_userName);
    if (meet_list.empty()) {
        cout << "[Result]:No meetings!" << endl;
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "title" << std::setw(20) << "sponsor" << std::setw(20) << "start time" << std::setw(20) << "end time" << std::setw(50) << "participator" << endl;
        for (auto iter = meet_list.begin(); iter != meet_list.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getTitle() << std::setw(20) << (*iter).getSponsor()  << std::setw(20) << Date::dateToString((*iter).getStartDate()) << std::setw(20) << Date::dateToString((*iter).getEndDate()) << std::setw(50) << participatorList((*iter).getParticipator()) << endl;
        }
        time_t end = clock();
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Search time is: " << timer << "s " << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::listAllParticipateMeetings(void) {
    time_t start = clock();
    cout << endl;
    cout << "[list all participator meetings]:\n";
    auto meet_list = m_agendaService.listAllParticipateMeetings(m_userName);
    if (meet_list.empty()) {
        cout << "[Result]:No meetings!\n";
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "title" << std::setw(20) << "sponsor" << std::setw(20) << "start time" << std::setw(20) << "end time" << std::setw(50) << "participator" << endl;
        for (auto iter = meet_list.begin(); iter != meet_list.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getTitle() << std::setw(20) << (*iter).getSponsor()  << std::setw(20) << Date::dateToString((*iter).getStartDate()) << std::setw(20) << Date::dateToString((*iter).getEndDate()) << std::setw(50) << participatorList((*iter).getParticipator()) << endl;
        }
        time_t end = clock();
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Search time is: " << timer << "s " << endl;
    }

    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::queryMeetingByTitle(void) {
    cout << endl;
    cout << "[query meeting] [Title]:" << endl;
    std::string title;
    cout << "[query meeting] ";
    cin >> title;
    // getline(cin, title);
    time_t start = clock();
    auto meet_list = m_agendaService.meetingQuery(m_userName, title);
    if (meet_list.empty()) {
        cout << "[Result]:No Title!\n";
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "title" << std::setw(20) << "sponsor" << std::setw(20) << "start time" << std::setw(20) << "end time" << std::setw(50) << "participator" << endl;
        for (auto iter = meet_list.begin(); iter != meet_list.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getTitle() << std::setw(20) << (*iter).getSponsor() << std::setw(20) << Date::dateToString((*iter).getStartDate()) << std::setw(20) << Date::dateToString((*iter).getEndDate()) << std::setw(50) << participatorList((*iter).getParticipator()) << endl;
        }
        time_t end = clock();
        double timer = double(end - start) / CLOCKS_PER_SEC;
        cout << "The Search time is: " << timer << "s " << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << endl;
    cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meetings] ";
    std::string start, end;
    cin >> start;
    cin >> end;
    // getline(cin, start);
    // getline(cin, end);
    cout << endl;
    while (true) {
        if (Date::isValid(Date::stringToDate(start)) && Date::isValid(Date::stringToDate(end)) && Date::stringToDate(start) < Date::stringToDate(end)) break;
        cout << "[Result]:Invalid Input format of The date!\n";
        cout << "[Query Meeting By TimeInterval]:";
        cin >> start >> end;
    }
    time_t start_ = clock();
    auto meet_list = m_agendaService.meetingQuery(m_userName, start, end);
    if (meet_list.empty()) {
        cout << "[Result]:No Meeting!\n";
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "title" << std::setw(20) << "sponsor" << std::setw(20) << "start time" << std::setw(20) << "end time" << std::setw(50) << "participator" << endl;
        for (auto iter = meet_list.begin(); iter != meet_list.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getTitle() << std::setw(20) << (*iter).getSponsor() << std::setw(20) << Date::dateToString((*iter).getStartDate()) << std::setw(20) << Date::dateToString((*iter).getEndDate()) << std::setw(50) << participatorList((*iter).getParticipator()) << endl;
        }
        time_t end_ = clock();
        double timer = double(end_ - start_) / CLOCKS_PER_SEC;
        cout << "The Search time is: " << timer << "s " << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::deleteMeetingByTitle(void) {
    cout << endl;
    cout << "[delete meeting] [title]" << endl;
    std::string title;
    cout << "[delete meeting] ";
    cin >> title;
    // getline(cin, title);
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << "[delete meeting by title] succeed!\n";
    }
    else
    {
        cout << "[error] delete meeting fail!" << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::deleteAllMeetings(void) {
    cout << endl;
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << "[delete all meetings] succeed!" << endl;
    }
    else
    {
        cout << "[error]delete all meetings fail!" << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::printMeetings(std::list<Meeting> meetings) {
    cout << endl;
    if (meetings.empty()) {
        cout << "No meetings!\n";
    }
    else
    {
        cout << std::setiosflags(std::ios::left) << std::setw(20) << "title" << std::setw(20) << "sponsor" << std::setw(20) << "start time" << std::setw(20) << "end time" << std::setw(50) << "participator" << endl;
        for (auto iter = meetings.begin(); iter != meetings.end(); ++iter) {
            cout << std::setiosflags(std::ios::left) << std::setw(20) << (*iter).getTitle() << std::setw(20) << (*iter).getSponsor() << std::setw(20) << participatorList((*iter).getParticipator()) << std::setw(20) << Date::dateToString((*iter).getStartDate()) << std::setw(20) << Date::dateToString((*iter).getEndDate()) << endl;
        }
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}
void AgendaUI::updatePassword() {
    cout << endl;
    cout << "[Update Pawword]: [New Password]" << endl;
    string newpsd;
    cout << "[Update Pawword]: ";
    cin >> newpsd;
    if (m_agendaService.updatePassword(m_userName, newpsd)) {
        cout << "[Update Pawword]: succeed!" << endl;
    } else{
        cout << "[Update Pawword]: failed!" << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}

void AgendaUI::updateEmail() {
    cout << endl;
    cout << "[Update Email]: [New Email]" << endl;
    string email;
    cout << "[Update Email]: ";
    cin >> email;
    if (m_agendaService.updateEmail(m_userName, email)) {
        cout << "[Update Email]: succeed!" << endl;
    } else{
        cout << "[Update Email]: failed!" << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}

void AgendaUI::updatePhone() {
    cout << endl;
    cout << "[Update Phone]: [New Phone]" << endl;
    string phone;
    cout << "[Update Phone]: ";
    cin >> phone;
    if (m_agendaService.updatePhone(m_userName, phone)) {
        cout << "[Update Phone]: succeed!" << endl;
    } else{
        cout << "[Update Phone]: failed!" << endl;
    }
    interface = 1;
    startAgenda();
    OperationLoop();
}



