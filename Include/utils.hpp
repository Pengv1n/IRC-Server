#ifndef FT_IRC_UTILS_HPP
#define FT_IRC_UTILS_HPP

#include <queue>
#include <string>
#include <algorithm>
#include <iostream>
#include "Message.hpp"

class Message;

bool						isEqualToRegex(std::string mask, std::string subString);
std::queue<std::string>		split(const std::string &s, char sep, bool include);
void						logMessage(const Message &msg);
bool						isValidNick(const std::string &nick);
bool						isValidChannelName(const std::string &name);

#endif //FT_IRC_UTILS_HPP
