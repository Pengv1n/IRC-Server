#ifndef FT_IRC_HISTORY_HPP
#define FT_IRC_HISTORY_HPP

#include <string>
#include <vector>

class User;

class UserInfo
{
private:
    std::string nickname;
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
    time_t      registrationTime;

    UserInfo();
    UserInfo(const UserInfo &copy);
    UserInfo &operator=(const UserInfo &copy);
public:
    UserInfo(const User &user);
    ~UserInfo();

    const std::string   &getNickname() const;
    const std::string	&getUsername() const;
    const std::string	&getHostname() const;
    const std::string   &getRealname() const;
    const std::string	&getServername() const;
};

class History
{
private:
    History(const History &copy);
    History &operator=(const History &copy);

    std::vector<UserInfo *> historyList;
public:
    History();
    ~History();

    void    addUser(const User &user);
    const std::vector<const UserInfo *> getHistoryByName(const std::string &nickname) const;
};

#include "User.hpp"

#endif //FT_IRC_HISTORY_HPP
