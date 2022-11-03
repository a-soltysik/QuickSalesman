set(${PROJECT_NAME}_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE ${PROJECT_NAME}_SRC_LIST
    ${${PROJECT_NAME}_SRC_DIR}/*.h
    ${${PROJECT_NAME}_SRC_DIR}/*.cpp
    )

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("Cannot compile parsers on debug build!")
    list(REMOVE_ITEM ${PROJECT_NAME}_SRC_LIST ${${PROJECT_NAME}_SRC_DIR}/parsers/ProjectReader.cpp)
endif()

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SRC_LIST}
    )

target_sources(${PROJECT_NAME}
    PRIVATE
    ${${PROJECT_NAME}_SRC_LIST}
    )

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${${PROJECT_NAME}_SRC_DIR}
    ${TSP_READER_INC_DIR}
    )

target_link_libraries(${PROJECT_NAME} PUBLIC TSP_READER)

