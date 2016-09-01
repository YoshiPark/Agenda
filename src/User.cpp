//  User.cpp
//  Agenda
//
//  Created by Yoshi Park 16/08/18.
//  Copyright  2016 Yoshi Park. All rights reserved.
//

#include "User.hpp"
/* User::User() : m_name(), m_password(), m_email(), m_phone() {} */
User::User(std::string userName, std::string userPassword,
    std::string userEmail, std::string userPhone)
    : m_name(userName), m_password(userPassword), m_email(userEmail),
    m_phone(userPhone) {}
User::User(const User &t_user) {
    m_name = t_user.m_name;
    m_password = t_user.m_password;
    m_email = t_user.m_email;
    m_phone = t_user.m_phone;
}
std::string User::getName() const { return m_name; }
void User::setName(std::string name) { m_name = name; }
std::string User::getPassword() const { return m_password; }
void User::setPassword(std::string password) { m_password = password; }
std::string User::getEmail() const { return m_email; }
void User::setEmail(std::string email) { m_email = email; }
std::string User::getPhone() const { return m_phone; }
void User::setPhone(std::string phone) { m_phone = phone; }