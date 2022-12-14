#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

class Server;
class User;
class Message;
class Channel;

#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <netinet/in.h>
#include "Message.hpp"
#include "User.hpp"
#include <fcntl.h>
#include <poll.h>
#include "utils.hpp"
#include "sendError.hpp"
#include "sendReply.hpp"
#include "History.hpp"
#include "Channel.hpp"
#include <unistd.h>

#define	DISCONNECT	-2

#ifdef __APPLE__
#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif

typedef  int (Server::*Method) (const Message &, User &);

class Server
{
private:
    int port;
    std::string password;
    const id_t    timeout;
    std::map<std::string , Method>  commands;
    std::vector<std::string>    motd;
    int sockfd;
    sockaddr_in sockaddr;
    std::vector<struct pollfd>  userFDs;
    std::vector<User *> connectedUsers;

    std::string name;
    std::string info;
    std::string version;
    std::string debuglvl;
    std::string comments;
    std::string describe;
    std::string adminName;
    std::string adminEmail;
    std::string adminNickname;
    in_addr_t   allowedIP;
    unsigned long   maxInactiveTimeout;
    unsigned long   maxResponseTimeout;
    unsigned long   maxChannels;
    std::map<std::string, std::string>  operators;
    std::map<std::string, Channel *>    channels;
    History nicknamesHistory;

    // Commands

    int passCmd(const Message &msg, User &user);
    int nickCmd(const Message &msg, User &user);
    int userCmd(const Message &msg, User &user);
    int operCmd(const Message &msg, User &user);
    int quitCmd(const Message &msg, User &user);
    int privmsgCmd(const Message &msg, User &user);
    int awayCmd(const Message &msg, User &user);
    int noticeCmd(const Message &msg, User &user);
    int whoCmd(const Message &msg, User &user);
    int whoisCmd(const Message &msg, User &user);
    int whowasCmd(const Message &msg, User &user);
    int modeCmd(const Message &msg, User &user);
    int topicCmd(const Message &msg, User &user);
    int joinCmd(const Message &msg, User &user);
    int	inviteCmd(const Message &msg, User &user);
    int	kickCmd(const Message &msg, User &user);
    int partCmd(const Message &msg, User &user);
    int namesCmd(const Message &msg, User &user);
    int listCmd(const Message &msg, User &user);
    int	wallopsCmd(const Message &msg, User &user);
    int pingCmd(const Message &msg, User &user);
    int pongCmd(const Message &msg, User &user);
    int	isonCmd(const Message &msg, User &user);
    int	userhostCmd(const Message &msg, User &user);
    int	versionCmd(const Message &msg, User &user);
    int	infoCmd(const Message &msg, User &user);
    int	adminCmd(const Message &msg, User &user);
    int	timeCmd(const Message &msg, User &user);
    int	rehashCmd(const Message &msg, User &user);
    int	restartCmd(const Message &msg, User &user);
    int	killCmd(const Message &msg, User &user);

    // Helpful things for commands
    int handleChanFlags(const Message &msg, User &user);
    int handleUserFlags(const Message &msg, User &user);
    void    notifyUsers(User &user, const std::string &notification);
    int     checkConnection(User &user);

    Server();
    Server(const Server& copy);
    Server	&operator=(const Server& other);
public:
    Server(int port, const std::string &password);
    ~Server();

    void    loadConfig();

    // Server setup
    void    createSocket();
    void    bindSocket();
    void    listenSocket();
    void    grabConnection();
    void    processMessages();
    int handleMessages(User &user);
    void    checkConnectionWithUsers();
    void    deleteBrokenConnections();
    void    deleteEmptyChannels();

    // Other mothods

    void	inviteToChannel(const User &user, const std::string &nickname, const std::string &chanName);
    bool    containsNickname(const std::string &nickname) const;
    bool    containsChannel(const std::string &name) const;
    void    sendMOTD(const User &user) const;
    int     connectToChannel(const User &user, const std::string &name, const std::string &key);
    User    *getUserByName(const std::string &name);
};

#endif //FT_IRC_SERVER_HPP
