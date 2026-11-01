project("system-equations-app")

file(GLOB SRC src/*.cpp src/*.cxx)

add_executable(${PROJECT_NAME} ${SRC})

set(DOMAIN_DEPS
    non-linear-core
    non-linear-newton
    non-linear-iterative
    gauss
    matrix
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DOMAIN_DEPS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)