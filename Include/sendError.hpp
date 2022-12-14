#ifndef FT_IRC_SENDERROR_HPP
#define FT_IRC_SENDERROR_HPP

class User;

#include "User.hpp"
#include <sstream>
#include "answers.hpp"

int		sendError(const User &user, int err, const std::string &arg1 = "", const std::string &arg2 = "");

#endif //FT_IRC_SENDERROR_HPP
