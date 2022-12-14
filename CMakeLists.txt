cmake_minimum_required(VERSION 3.24)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)

include_directories(Include)

add_executable(ft_irc src/main.cpp Include/Server.hpp src/Server.cpp Include/Message.hpp Include/User.hpp src/User.cpp src/utils.cpp Include/utils.hpp src/Message.cpp Include/sendError.hpp src/sendError.cpp Include/answers.hpp Include/History.hpp src/History.cpp Include/Channel.hpp src/Channel.cpp src/commands/registrationCommands.cpp Include/Hash.hpp src/Hash.cpp Include/sendReply.hpp src/sendReply.cpp src/commands/userCommands.cpp src/commands/channelCommands.cpp src/commands/otherCommands.cpp src/commands/ircOperatorCommands.cpp)
