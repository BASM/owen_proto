cmake_minimum_required (VERSION 2.8)
project (serial)
include_directories("include")


ADD_LIBRARY(serial text/owen_phy.c)

add_executable(phy_121 test/121_phy/main.c libserial.a)
target_link_libraries(phy_121 serial)
