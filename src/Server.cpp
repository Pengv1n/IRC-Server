#include <fstream>
#include <arpa/inet.h>
#include <iostream>
#include "Server.hpp"

Server::Server(int port, const std::string &password) :
port(port), password(password), timeout(1) {
    commands["PASS"] = &Server::passCmd;
    commands["NICK"] = &Server::nickCmd;
    commands["USER"] = &Server::userCmd;
    commands["OPER"] = &Server::operCmd;
    commands["QUIT"] = &Server::quitCmd;
    commands["PRIVMSG"] = &Server::privmsgCmd;
    commands["AWAY"] = &Server::awayCmd;
    commands["NOTICE"] = &Server::noticeCmd;
    commands["WHO"] = &Server::whoCmd;
    commands["WHOIS"] = &Server::whoisCmd;
    commands["WHOWAS"] = &Server::whowasCmd;
    commands["MODE"] = &Server::modeCmd;
    commands["TOPIC"] = &Server::topicCmd;
    commands["JOIN"] = &Server::joinCmd;
    commands["INVITE"] = &Server::inviteCmd;
    commands["KICK"] = &Server::kickCmd;
    commands["PART"] = &Server::partCmd;
    commands["NAMES"] = &Server::namesCmd;
    commands["LIST"] = &Server::listCmd;
    commands["WALLOPS"] = &Server::wallopsCmd;
    commands["PING"] = &Server::pingCmd;
    commands["PONG"] = &Server::pongCmd;
    commands["ISON"] = &Server::isonCmd;
    commands["USERHOST"] = &Server::userhostCmd;
    commands["VERSION"] = &Server::versionCmd;
    commands["INFO"] = &Server::infoCmd;
    commands["ADMIN"] = &Server::adminCmd;
    commands["TIME"] = &Server::timeCmd;
    commands["REHASH"] = &Server::rehashCmd;
    commands["RESTART"] = &Server::restartCmd;
    commands["KILL"] = &Server::killCmd;

    // Read MOTD
    std::string line;
    std::ifstream   motdFile("../conf/IRCat.motd");
    if (motdFile.is_open())
    {
        while (getline(motdFile, line))
            motd.push_back(line);
        motdFile.close();
    }

    loadConfig();
}

void Server::loadConfig() {
    name = "IRChat";
    info = "IRC server based on TCP/IP protocol to rfc1459 standard";
    version = "v1.0";
    debuglvl = "1";
    comments = "release";
    describe = "describe";
    adminName = "Georgiy";
    adminNickname = "aregenia";
    adminEmail = "aregenia@student.21-school.ru";
    allowedIP = inet_addr("0.0.0.0");
    maxChannels = 10;
    maxInactiveTimeout = 1200;
    maxResponseTimeout = 600;
    operators.insert(std::pair<std::string, std::string>(
            "aregenia", "46332f2c09c368c8f63178c0e1af4ebd5ed077a442e12f76e6bf76d0e7a4dace"
            ));
    operators.insert(std::pair<std::string, std::string>(
            "ntitan", "16c167c315efcdd03dbe1b21d58173e57415fc66ef0a7682b319843150eb52ff"
    ));

    std::cout << "CONFIG" << std::endl;
    std::cout << "servername: " << name << std::endl;
    std::cout << "info: " << info << std::endl;
    std::cout << "version: " << version << std::endl;
    std::cout << "debuglvl: " << debuglvl << std::endl;
    std::cout << "comments: " << comments << std::endl;
    std::cout << "describe: " << describe << std::endl;
    std::cout << "adminName: " << adminName << std::endl;
    std::cout << "adminEmail: " << adminEmail << std::endl;
    std::cout << "adminNickname: " << adminNickname << std::endl;
    std::cout << "maxChannels: " << maxChannels << std::endl;
    std::cout << "maxInactiveTimeout: " << maxInactiveTimeout << std::endl;
    std::cout << "maxResponseTimeout: " << maxResponseTimeout << std::endl;

    struct in_addr paddr;
    paddr.s_addr = allowedIP;
    std::cout << "allowedIP(int): " << allowedIP << std::endl;
    std::cout << "allowedIP(str): " << inet_ntoa(paddr) << std::endl;

    std::map<std::string, std::string>::iterator beg = operators.begin();
    std::map<std::string, std::string>::iterator end = operators.end();
    std::map<std::string, std::string>::iterator it;

    for (it = beg; it != end; it++)	{
        std::cout << "Login: " << it->first << " " << "Hash: " << it->second << std::endl;
    }
}

Server::~Server() {
    for (size_t i = 0; i < connectedUsers.size(); ++i)
    {
        close(connectedUsers[i]->getSockfd());
        delete connectedUsers[i];
    }
    std::map<std::string, Channel *>::const_iterator    beg = channels.begin();
    std::map<std::string, Channel *>::const_iterator    end = channels.end();
    for (; beg != end; ++beg)
        delete (*beg).second;
    close(sockfd);
}

void Server::createSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocket() {
    const int trueFlag = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
    {
        std::cout << "setsockopt failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = allowedIP; // INADDR_ANY; Was 0.0.0.0, now 127.0.0.1
    sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cout << "Failed to bind to port " << port << ". errno " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::listenSocket() {
    if (listen(sockfd, 128) < 0)
    {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
}

void Server::grabConnection() {
    size_t addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection >= 0)
    {
        char    host[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(sockaddr.sin_port), host, INET_ADDRSTRLEN);
        struct pollfd   pfd;
        pfd.fd = connection;
        pfd.events = POLLIN;
        pfd.revents = 0;
        userFDs.push_back(pfd);
        connectedUsers.push_back(new User(connection, host, name));
    }
}

void Server::processMessages() {
    int pret = poll(userFDs.data(), userFDs.size(), timeout);
    std::vector<int>    toErase;
    if (pret != 0)
    {
        //Read from the connection
        for (size_t i = 0; i < userFDs.size(); i++)
        {
            if (userFDs[i].revents & POLLIN)
            {
                if (connectedUsers[i]->readMessage() == DISCONNECT)
                    connectedUsers[i]->setFlag(BREAKCONNECTION);
                else if (handleMessages(*(connectedUsers[i])) == DISCONNECT)
                    connectedUsers[i]->setFlag(BREAKCONNECTION);
            }
            userFDs[i].revents = 0;
        }
    }
}

int Server::handleMessages(User &user) {
    while (user.getMessages().size() > 0
        && user.getMessages().front()[user.getMessages().front().size() - 1] == '\n')
    {
        Message msg(user.getMessages().front());
        user.popMessage();
        // log message to server console
        logMessage(msg);
        // handle
        if (!(user.getFlags() & REGISTERED) && msg.getCommand() != "QUIT" && msg.getCommand() != "PASS"
            && msg.getCommand() != "USER" && msg.getCommand() != "NICK")
            sendError(user, ERR_NOTREGISTERED);
        else
        {
            try
            {
                int ret = (this->*(commands.at(msg.getCommand())))(msg, user);
                if (ret == DISCONNECT)
                    return (DISCONNECT);
            }
            catch (const std::exception& e)
            {
                sendError(user, ERR_UNKNOWNCOMMAND, msg.getCommand());
            }
        }
    }
    user.updateTimeOfLastMessage();
    return (0);
}

void Server::checkConnectionWithUsers() {
    for (size_t i = 0; i < connectedUsers.size(); i++)
    {
        if (this->connectedUsers[i]->getFlags() & REGISTERED)
        {
            if (time(0) - this->connectedUsers[i]->getTimeOfLastMessage() > static_cast<time_t>(maxInactiveTimeout))
            {
                this->connectedUsers[i]->sendMessage(":" + this->name + " PING :" + this->name + "\n");
                this->connectedUsers[i]->updateTimeAfterPing();
                this->connectedUsers[i]->updateTimeOfLastMessage();
                this->connectedUsers[i]->setFlag(PINGING);
            }
            if ((connectedUsers[i]->getFlags() & PINGING) && time(0) - connectedUsers[i]->getTimeAfterPing() > static_cast<time_t>(maxResponseTimeout))
                connectedUsers[i]->setFlag(BREAKCONNECTION);
        }
    }
}

void Server::deleteBrokenConnections() {
    for (size_t i = 0; i < connectedUsers.size(); ++i)
    {
        if (connectedUsers[i]->getFlags() & BREAKCONNECTION)
        {
            this->nicknamesHistory.addUser(*(connectedUsers[i]));
            notifyUsers(*(connectedUsers[i]), ":" + connectedUsers[i]->getPrefix() + " QUIT :" + connectedUsers[i]->getQuitMessage() = "\n");
            close(connectedUsers[i]->getSockfd());
            std::map<std::string, Channel *>::iterator  beg = channels.begin();
            std::map<std::string, Channel *>::iterator  end = channels.end();
            for (; beg != end; ++beg)
                if ((*beg).second->containsNickname(connectedUsers[i]->getNickname()))
                    (*beg).second->disconnect(*(connectedUsers[i]));
            delete connectedUsers[i];
            connectedUsers.erase(connectedUsers.begin() + i);
            userFDs.erase(userFDs.begin() + i);
            --i;
        }
    }
}

void Server::notifyUsers(User &user, const std::string &notification) {
    const std::vector<const Channel *> chans = user.getChannels();
    for (size_t i = 0; i < connectedUsers.size(); i++)
    {
        for (size_t j = 0; j < chans.size(); j++)
        {
            if (chans[j]->containsNickname(connectedUsers[i]->getNickname()))
            {
                connectedUsers[i]->sendMessage(notification);
                break;
            }
        }
    }
}

void Server::deleteEmptyChannels() {
    std::map<std::string, Channel *>::const_iterator    beg = channels.begin();
    std::map<std::string, Channel *>::const_iterator    end = channels.end();
    for (; beg != end;)
    {
        if ((*beg).second->isEmpty())
        {
            delete (*beg).second;
            channels.erase((*beg).first);
            beg = channels.begin();
        }
        else
            ++beg;
    }
}

bool Server::containsNickname(const std::string &nickname) const {
    size_t  usersCount = connectedUsers.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (connectedUsers[i]->getNickname() == nickname)
            return (true);
    }
    return (false);
}

bool	Server::containsChannel(const std::string &name) const
{
    try
    {
        channels.at(name);
        return true;
    }
    catch(const std::exception& e)
    {}
    return false;
}

User *Server::getUserByName(const std::string &name) {
    User    *ret;
    size_t  usersCount = connectedUsers.size();
    for (size_t i = 0; i < usersCount; i++)
        if (connectedUsers[i]->getNickname() == name)
            ret = connectedUsers[i];
    return ret;
}