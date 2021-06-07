
cmake_minimum_required(VERSION 3.16)
#SET(CMAKE_INSTALL_PRFIX )

project(wwjlog)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(${PROJECT_SOURCE_DIR})

SET(LIB_SRC                            
    Logging.cc)
#SET(LIB_SRC Acceptor.cc Buffer.cc)
#message(STATUS ${LIB_SRC})

add_library(wwjlog SHARED ${LIB_SRC})

install(TARGETS wwjlog LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib)
#install(FILES ${PROJECT_SOURCE_DIR}/)

#FILE(GLOB TINNYMUDUO_SRC RELATIVE ${PROJECT_SOURCE_DIR}/src/*.cc)
#FILE(GLOB TINNYMUDUO_SRC ./src/*.cc)
#message(STATUS ${TINNYMUDUO_SRC})

#FILE(GLOB TINNYMUDUO_HEAD RELATIVE ${PROJECT_SOURCE_DIR}/src/include/*.h)
#FILE(GLOB TINNYMUDUO_HEAD ./src/include/*.h)
#message(STATUS ${TINNYMUDUO_HEAD})

#SET(TINYMUDUO_SRC ./src/*.cc)

#add_library(tinymuduo SHARED ${TINYMUDUO_SRC}) 

#INSTALL(TARGETS tinymuduo LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib)
#INSTALL(FILES TINNYMUDUO_HEAD DESTINATION ${PROJECT_SOURCE_DIR}/include)

