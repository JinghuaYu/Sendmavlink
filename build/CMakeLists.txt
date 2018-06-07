cmake_minimum_required(VERSION 2.8)  
PROJECT(send_mavlink)  
SET(SRC_LIST ../main.c)  
SET(SENDMAV_BINARY_DIR ../)
SET(SENDMAV_SOURCE_DIR ${SRC_LIST})
MESSAGE(STATUS "This is BINARY dir "${SENDMAV_BINARY_DIR})  
MESSAGE(STATUS "This is SOURCE dir "${SENDMAV_SOURCE_DIR})  
include_directories(
${SENDMAV_BINARY_DIR}/mavlink/include/mavlink/v2.0/common
)
ADD_EXECUTABLE(send_mavlink ${SRC_LIST})  
target_link_libraries(send_mavlink
	-lpthread
)
