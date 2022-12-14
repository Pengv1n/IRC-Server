#include "sendReply.hpp"

int		sendReply(const std::string &from, const User &user, int rpl, \
				const std::string &arg1,const std::string &arg2, \
				const std::string &arg3,const std::string &arg4, \
				const std::string &arg5,const std::string &arg6, \
				const std::string &arg7,const std::string &arg8)
{
    std::string msg = ":" + from + " ";
    std::stringstream   ss;
    ss << rpl;
    msg += ss.str() + " " + user.getNickname() + " ";
    switch (rpl)
    {
        case RPL_YOUREOPER:
            msg += ":You are now an IRC operator\n";
            break;
        case RPL_MOTD:
            msg += ":- " + arg1 + "\n";
            break;
        case RPL_MOTDSTART:
            msg += ":- " + arg1 + " Message of the day - \n";
            break;
        case RPL_ENDOFMOTD:
            msg += ":End of /MOTD command\n";
            break;
        case RPL_AWAY:
            msg += arg1 + " :" + arg2 + "\n";
            break;
        case RPL_WHOREPLY:
            msg += arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " ";
            msg += arg5 + " " + arg6 + " :" + arg7 + " " + arg8 + "\n";
            break;
        case RPL_ENDOFWHO:
            msg += arg1 + " :End of /WHO list\n";
            break;
        case RPL_UMODEIS:
            msg += arg1 + "\n";
            break;
        case RPL_ADMINME:
            msg += arg1 + " :Administrative info\n";
            break;
        case RPL_ADMINLOC1:
            msg += ":Name     " + arg1 + "\n";
            break;
        case RPL_ADMINLOC2:
            msg += ":Nickname " + arg1 + "\n";
            break;
        case RPL_ADMINEMAIL:
            msg += ":E-Mail   " + arg1 + "\n";
            break;
        case RPL_USERHOST:
            msg += ":" + arg1 + "\n";
            break;
        case RPL_ISON:
            msg += ":" + arg1 + "\n";
            break;
        case RPL_UNAWAY:
            msg += ":You are no longer marked as being away\n";
            break;
        case RPL_NOWAWAY:
            msg += ":You have been marked as being away\n";
            break;
        case RPL_WHOISUSER:
            msg += arg1 + " " + arg2 + " " + arg3 + " * :" + arg4 + "\n";
            break;
        case RPL_WHOISSERVER:
            msg += arg1 + " " + arg2 + " :" + arg3 + "\n";
            break;
        case RPL_WHOISOPERATOR:
            msg += arg1 + " :is an IRC operator\n";
            break;
        case RPL_WHOWASUSER:
            msg += arg1 + " " + arg2 + " " + arg3 + " * :" + arg4 + "\n";
            break;
        case RPL_WHOISIDLE:
            msg += arg1 + " " + arg2 + " " + arg3 + " :seconds idle\n";
            break;
        case RPL_ENDOFWHOIS:
            msg += arg1 + " :End of /WHOIS list\n";
            break;
        case RPL_WHOISCHANNELS:
            msg += arg1 + " :" + arg2 + "\n";
            break;
        case RPL_LISTSTART:
            msg += "Channel :Users  Name\n";
            break;
        case RPL_LIST:
            msg += arg1 +  " " + arg2 + " :" + arg3 + "\n";
            break;
        case RPL_LISTEND:
            msg += ":End of /LIST\n";
            break;
        case RPL_CHANNELMODEIS:
            msg += arg1 + " +" + arg2 + "\n";
            break;
        case RPL_NOTOPIC:
            msg += arg1 + " :No topic is set\n";
            break;
        case RPL_TOPIC:
            msg += arg1 + " :" + arg2 + "\n";
            break;
        case RPL_INVITING:
            msg += arg1 + " " + arg2 + "\n";
            break;
        case RPL_VERSION:
            msg += arg1 + "." + arg2 + " " + arg3 + " :" + arg4 + "\n";
            break;
        case RPL_NAMREPLY:
            msg += arg1 + " :" + arg2 + "\n";
            break;
        case RPL_ENDOFNAMES:
            msg += arg1 + " :End of /NAMES list\n";
            break;
        case RPL_ENDOFWHOWAS:
            msg += arg1 + " :End of WHOWAS\n";
            break;
        case RPL_INFO:
            msg += ":" + arg1 + "\n";
            break;
        case RPL_ENDOFINFO:
            msg += ":End of /INFO list\n";
            break;
        case RPL_REHASHING:
            msg += arg1 + " :Rehashing\n";
            break;
        case RPL_TIME:
            msg += arg1 + " :" + arg2; // ctime return string following '\n'
            break;
    }
    send(user.getSockfd(), msg.c_str(), msg.size(), IRC_NOSIGNAL);
    return 0;
}