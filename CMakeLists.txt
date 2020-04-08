cmake_minimum_required(VERSION 3.16)
project(fixparser)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)
add_executable(fixparser main.cpp FixApp.cpp FixApp.h FixMessage.cpp FixMessage.h FixTag.h FixTag.cpp FixField.cpp FixField.h FixGroup.cpp FixGroup.h tag_list.h OrderBook.cpp OrderBook.h Order.cpp Order.h)