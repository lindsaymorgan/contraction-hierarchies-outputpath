cmake_minimum_required (VERSION 3.1)
project (many2manypath)

if(UNIX OR APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -std=c++11")
endif()

find_package(Boost 1.57 COMPONENTS regex REQUIRED)
find_package(Protobuf)


file(GLOB MANY2MANY_SRC
        io/*.h
        processing/*.h
        *.h
        *.cpp)

set(MANY2MANY_SRC
        ${CHMAIN_FILES}
        ${INCLUDE_FILES}
        )


add_executable(many2manypath ${MANY2MANY_SRC})
target_link_libraries(many2manypath
    protos-ch
    ${Boost_LIBRARIES}
    ${PROTOBUF_LIBRARIES})