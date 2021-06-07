
cmake_minimum_required(VERSION 3.16)
#SET(CMAKE_INSTALL_PRFIX )

project(wwjlogtest)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(${PROJECT_SOURCE_DIR}/../include)

#find_path(Muduo_LIBRARY_DIR libtinymuduo.so "${MUDUO_PATH}")
#message(STATUS ${Muduo_LIBRARY_DIR})

#find_library(tinymuduo libtinymuduo.so ${Muduo_LIBRARY_DIR})

add_executable(test test.cc)
target_link_libraries(test ${PROJECT_SOURCE_DIR}/../lib/libwwjlog.so)
#target_link_libraries(test1 pthread rt)

#[[
add_executable(test2 test2.cc)
target_link_libraries(test2 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test2 pthread rt)

add_executable(test3 test3.cc)
target_link_libraries(test3 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test3 pthread rt)

add_executable(test4 test4.cc)
target_link_libraries(test4 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test4 pthread rt)

add_executable(test5 test5.cc)
target_link_libraries(test5 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test5 pthread rt)

add_executable(test6 test6.cc)
target_link_libraries(test6 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test6 pthread rt)

add_executable(test7 test7.cc)
target_link_libraries(test7 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test7 pthread rt)

add_executable(test8 test8.cc)
target_link_libraries(test8 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test8 pthread rt)

add_executable(test9 test9.cc)
target_link_libraries(test9 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test9 pthread rt)

add_executable(test10 test10.cc)
target_link_libraries(test10 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test10 pthread rt)

add_executable(test11 test11.cc)
target_link_libraries(test11 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test11 pthread rt)

add_executable(test12 test12.cc)
target_link_libraries(test12 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test12 pthread rt)

add_executable(test13 test13.cc)
target_link_libraries(test13 ${PROJECT_SOURCE_DIR}/../lib/libtinymuduo.so)
target_link_libraries(test13 pthread rt)

]]