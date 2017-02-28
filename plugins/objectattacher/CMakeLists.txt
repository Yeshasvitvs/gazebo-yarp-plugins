cmake_minimum_required(VERSION 2.8.7)

PROJECT(Plugin_ObjectAttacher)

include(AddGazeboYarpPluginTarget)

set(objectattacher_sources src/objectattacherserver.cpp src/objectattacherinterface.cpp)
set(objectattacher_headers include/objectattacherserver.h include/objectattacherinterface.h)

set(LIB_COMMON_NAME gazebo_yarp_lib_common)
if(CMAKE_VERSION VERSION_LESS 3.0.0)
    get_property(GAZEBO_YARP_COMMON_HEADERS GLOBAL PROPERTY GAZEBO_YARP_COMMON_HEADERS)
    unset(LIB_COMMON_NAME)
endif()
                            
add_gazebo_yarp_plugin_target(LIBRARY_NAME objectattacher
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}
                              INCLUDE_DIRS include/gazebo 
                              SYSTEM_INCLUDE_DIRS ${GAZEBO_YARP_COMMON_HEADERS} ${YARP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS}  ${SDFORMAT_INCLUDE_DIRS} ${PROTOBUF_INCLUDE_DIRS}
                              LINKED_LIBRARIES gazebo_yarp_rpc_objectattacher ${LIB_COMMON_NAME} gazebo_yarp_singleton ${YARP_LIBRARIES} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES}
                              HEADERS ${objectattacher_headers}
                              SOURCES ${objectattacher_sources}
                            )
