# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.0)

# 检测路径
if (CMAKE_BUILD_RPATH)
	message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
	if(NOT EXISTS ${CMAKE_BUILD_RPATH})
	    message(FATAL_ERROR "CMAKE_BUILD_RPATH:${CMAKE_BUILD_RPATH} file/directory not found.")
	endif()
endif()

# 检测代码版本
if(CMAKE_BUILD_TYPE)
    message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

# 项目信息
project (shark)
set (SHARK_MAJOR_VERSION 1)
set (SHARK_MINOR_VERSION 0)
set (SHARK_PATCH_VERSION 0)

# 设置编译选项及链接库路径
if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES DEBUG)) 
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g") 
	set (LIB_PCAP_PATH "${CMAKE_BUILD_RPATH}/Utils/libpcap")
endif()	

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES RELEASE))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")
	set (LIB_PCAP_PATH "${CMAKE_BUILD_RPATH}/Utils/libpcap")
endif()

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES GTEST))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -fprofile-arcs -ftest-coverage") 
	set (LIB_PCAP_PATH "${CMAKE_BUILD_RPATH}/Utils/libpcap")
	set (LIB_GTEST_PATH "${CMAKE_BUILD_RPATH}/Env/gtest")
endif()

# 目录下的所有源文件
aux_source_directory (src SHARK_SRC_FILES)
aux_source_directory (common/src SHARK_COMMON_SRC_FILES)
if (CMAKE_BUILD_TYPE MATCHES GTEST)
    aux_source_directory (gtest CAPTURE_GTEST_FILES)
endif()

# 头文件目录
include_directories (${PROJECT_SOURCE_DIR}/hdr)
include_directories (${PROJECT_SOURCE_DIR}/common/hdr)

# 链接库头文件目录
include_directories (${LIB_PCAP_PATH}/include)
include_directories (${LIB_PCAP_PATH}/include/pcap)
if (CMAKE_BUILD_TYPE MATCHES GTEST)
    include_directories (${LIB_GTEST_PATH}/include)
endif()

# 链接库库文件目录
link_directories (${LIB_PCAP_PATH}/lib)
link_directories(${LIB_GTEST_PATH}/lib)

# 指定生成目标
if (CMAKE_BUILD_TYPE MATCHES GTEST)
    add_executable (gtest_shark ${SHARK_SRC_FILES} ${SHARK_COMMON_SRC_FILES} ${CAPTURE_GTEST_FILES})
endif()

if (NOT (CMAKE_BUILD_TYPE MATCHES GTEST))
    add_executable (shark ${SHARK_SRC_FILES} ${SHARK_COMMON_SRC_FILES})
endif()

# 链接库
if (NOT (CMAKE_BUILD_TYPE MATCHES GTEST))
	target_link_libraries (shark PRIVATE pthread)
	target_link_libraries (shark PRIVATE pcap)
endif()

# debug版本链接gtest库
if (CMAKE_BUILD_TYPE MATCHES GTEST)
	target_link_libraries (gtest_shark PRIVATE pthread)
    target_link_libraries(gtest_shark PRIVATE gtest)
	target_link_libraries (gtest_shark PRIVATE pcap)
endif()
