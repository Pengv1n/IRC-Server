#include "User.hpp"

User::User(int sockfd, const std::string &host, std::string &servername) :
sockfd(sockfd), hostname(host), servername(servername), registrationTime(time(0)), flags(RECEIVENOTICE)
{}

int User::readMessage() {
    std::string text;
    if (messages.size() > 0)
        text = messages.front();
    char    buffer[100];
    int bytesRead;
    while ((bytesRead = recv(sockfd, buffer, 99, 0)) > 0)
    {
        buffer[bytesRead] = 0;
        text += buffer;
        buffer[0] = 0;
        if (text.find('\n') != std::string::npos)
            break;
    }
    if (text.length() > 512)
        text = text.substr(0, 510) + "\r\n";
    if (bytesRead == 0)
        return (DISCONNECT);
    while (text.find("\r\n") != std::string::npos)
        text.replace(text.find("\r\n"), 2, "\n");
    if (text.size() > 1)
        messages = split(text, '\n', true);
    return (0);
}

void User::setFlag(unsigned char flag) {
    flags |= flag;
    if (flag == BREAKCONNECTION && quitMessage.size() == 0)
        quitMessage = "Client exited";
}

const std::queue<std::string> &User::getMessages() const {
    return messages;
}

void User::popMessage() {
    if (messages.size() > 0)
        messages.pop();
}

unsigned char User::getFlags() const {
    return flags;
}

const std::string &User::getServername() const {
    return servername;
}

const std::string &User::getNickname() const {
    return nickname;
}

int User::getSockfd() const {
    return sockfd;
}

void User::updateTimeOfLastMessage() {
    this->timeOfLastMessage = time(0);
}

const time_t &User::getTimeOfLastMessage() const {
    return timeOfLastMessage;
}

void User::sendMessage(const std::string &msg) const {
    if (msg.size() > 0)
        send(sockfd, msg.c_str(), msg.size(), IRC_NOSIGNAL);
}

void User::updateTimeAfterPing() {
    this->timeAfterPing = time(0);
}

const time_t &User::getTimeAfterPing() const {
    return timeAfterPing;
}

const std::string &User::getUsername() const {
    return username;
}

const std::string &User::getHostname() const {
    return hostname;
}

const std::string &User::getRealname() const {
    return realname;
}

const std::vector<const Channel *> &User::getChannels() const {
    return channels;
}

std::string	User::getPrefix() const
{
    return std::string(nickname + "!" + username + "@" + hostname);
}

const std::string &User::getQuitMessage() const {
    return quitMessage;
}

void	User::setPassword(const std::string &pass)
{
    password = pass;
}

void User::setNickname(const std::string &nickname) {
    this->nickname = nickname;
}

void User::setUsername(const std::string &username) {
    this->username = username;
}

void User::setRealname(const std::string &realname) {
    this->realname = realname;
}

void User::setQuitMessage(const std::string &msg) {
    quitMessage = msg;
}

const std::string	&User::getPassword() const
{
    return password;
}

const std::string &User::getAwayMessage() const {
    return awayMessage;
}

void User::removeFlag(unsigned char flag) {
    this->flags &= ~flag;
}

void User::setAwayMessage(const std::string &msg) {
    this->awayMessage = msg;
}

const time_t &User::getRegistrationTime() const {
    return registrationTime;
}

void	User::addChannel(const Channel &channel)
{
    channels.push_back(&channel);
}

bool	User::isOnChannel(const std::string &name) const
{
    for (size_t i = 0; i < channels.size(); i++)
        if (channels[i]->getName() == name)
            return true;
    return false;
}

User::~User() {}