cmake_minimum_required (VERSION 2.8)
project (serial)
include_directories("include")


add_library(serial text/owen_phy.c text/owen_datalink.c text/owen_present.c text/owen_utils.c)

add_executable(phy_121 test/121_phy/main.c)
add_executable(datalink_118 test/118_datalink/main.c)
add_executable(present_119 test/119_presentation/main.c)
add_executable(user_120 test/120_user/main.c)
target_link_libraries(phy_121 serial)
target_link_libraries(datalink_118 serial)
target_link_libraries(present_119 serial)
target_link_libraries(user_120 serial)
