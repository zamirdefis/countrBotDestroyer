cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "countrBotDestroyer")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
	ZLIB::ZLIB
	CURL::libcurl
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)