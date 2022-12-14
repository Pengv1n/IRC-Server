#ifndef FT_IRC_USER_HPP
#define FT_IRC_USER_HPP

class Server;
class Channel;

#define REGISTERED		0b00000001
#define INVISIBLE		0b00000010
#define RECEIVENOTICE	0b00000100
#define RECEIVEWALLOPS	0b00001000
#define IRCOPERATOR		0b00010000
#define AWAY			0b00100000
#define PINGING			0b01000000
#define BREAKCONNECTION	0b10000000

#include <string>
#include <queue>
#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"

class User
{
private:
    int sockfd;
    std::string password;
    std::string hostname;
    std::string nickname;
    std::string username;
    const std::string   &servername;
    std::string realname;
    time_t  registrationTime;
    time_t  timeOfLastMessage;
    time_t  timeAfterPing;
    std::string awayMessage;
    unsigned char   flags;
    std::queue<std::string> messages;
    std::vector<const Channel *>    channels;
    std::string quitMessage;

    User();
    User(const User& copy);
    User	&operator=(const User& other);
public:
    User(int sockfd, const std::string &host, std::string &servername);
    ~User();

    const std::string   &getUsername() const;
    const std::string   &getHostname() const;
    const std::queue<std::string>   &getMessages() const;
    unsigned char   getFlags() const;
    const std::string   &getServername() const;
    const std::string   &getNickname() const;
    const std::string   &getRealname() const;
    const std::string   &getPassword() const;
    const std::vector<const Channel *>  &getChannels() const;
    const std::string   &getAwayMessage() const;
    int getSockfd() const;
    const time_t    &getRegistrationTime() const;
    const time_t    &getTimeOfLastMessage() const;
    const time_t    &getTimeAfterPing() const;
    const std::string   &getQuitMessage() const;
    std::string getPrefix() const;

    void	setQuitMessage(const std::string &msg);
    void    setFlag(unsigned char flag);
    void    setPassword(const std::string &pass);
    void    setNickname(const std::string &nickname);
    void    setUsername(const std::string &username);
    void    setRealname(const std::string &realname);
    void	setAwayMessage(const std::string &msg);

    bool    isOnChannel(const std::string &name) const;
    int readMessage();
    void	removeChannel(const std::string &name);
    void    popMessage();
    void	addChannel(const Channel &channel);
    void    removeFlag(unsigned char flag);
    void    updateTimeOfLastMessage();
    void    updateTimeAfterPing();
    void    sendMessage(const std::string &msg) const;

};

#endif //FT_IRC_USER_HPP
