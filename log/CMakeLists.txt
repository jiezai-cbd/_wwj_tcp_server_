
cmake_minimum_required(VERSION 3.16)
#SET(CMAKE_INSTALL_PRFIX )

project(wwjmuduo)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

SET(LIB_SRC  
    Acceptor.cc
    Buffer.cc		        
    Channel.cc		        
		    
  	        
    EventLoop.cc		    
    EventLoopThread.cc  	
	EventLoopThreadPool.cc		    
    InetAddress.cc		    
    Poller.cc		        
    Socket.cc		        
    SocketsOps.cc		    
  		    
    TcpConnection.cc	    
    TcpServer.cc	    
    Timer.cc		        
    TimerQueue.cc           
    Timestamp.cc            
    Logging.cc
    LogStream.cc              
    Thread.cc
)
#SET(LIB_SRC Acceptor.cc Buffer.cc)
#message(STATUS ${LIB_SRC})

add_library(wwjmuduo STATIC ${LIB_SRC})
install(TARGETS wwjmuduo ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)
#install(TARGETS wwjmuduo LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib)
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

