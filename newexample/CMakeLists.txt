cmake_minimum_required (VERSION 3.1)
project (many2manypath)

if(UNIX OR APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -std=c++17")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

find_package(Boost 1.57 COMPONENTS regex REQUIRED)
find_package(Protobuf)


file(GLOB MANY2MANY_SRC
        ${PROJECT_SOURCE_DIR}/../io/*.h
        ${PROJECT_SOURCE_DIR}/../processing/*.h
        ${PROJECT_SOURCE_DIR}/../stats/*.h
        ${PROJECT_SOURCE_DIR}/../*.h
        *.h
        *.cpp)

message("These files are included: ${MANY2MANY_SRC}")

set(MANY2MANY_SRC
        ${MANY2MANY_SRC}
        ${INCLUDE_FILES}
        )



add_executable(many2manypath ${MANY2MANY_SRC})
target_link_libraries(many2manypath
    protos-ch
    ${Boost_LIBRARIES}
    ${PROTOBUF_LIBRARIES})
