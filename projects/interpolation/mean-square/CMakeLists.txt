project("mean-square-app")

file(GLOB SRC src/*.cpp src/*.cxx)

add_executable(${PROJECT_NAME} ${SRC})

set(DOMAIN_DEPS
    plane2d
    mean-square
    horner
    plot2d
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_DEPS} ${DOMAIN_DEPS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)