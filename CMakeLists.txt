cmake_minimum_required(VERSION 3.1)

include_directories(include)

add_executable(test_mysql_connector test_mysql_connector.cpp)

file(GLOB MYSQL_PREBUILT_LIBRARIES "lib/*.so*")
message("LIBRARIES = ${MYSQL_PREBUILT_LIBRARIES}")
target_link_libraries(test_mysql_connector ${MYSQL_PREBUILT_LIBRARIES})
