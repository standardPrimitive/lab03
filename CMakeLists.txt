cmake_minimum_required(VERSION 3.2)

project(user_service C CXX)

SET (USER_SERVICE "user_service")
set (STD_CXX "c++17")
set (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)
# find_package(Poco REQUIRED Foundation Data)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})

add_executable(${USER_SERVICE} main.cpp
                                 config/config.cpp 
                                 database/database.cpp
                                 database/route.cpp
                                 database/trip.cpp
                                 database/user.cpp
                                 database/cache.cpp)

add_executable(load_data load_data.cpp)

target_include_directories(${USER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${USER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
# target_link_libraries(${USER_SERVICE} PRIVATE Poco::Foundation Poco::Data)
target_link_libraries(${USER_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             #"PocoFoundation"                   
                             ZLIB::ZLIB)


target_link_libraries(load_data PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"                      
                             ZLIB::ZLIB)

set_target_properties(${USER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${USER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)