project("lab01_errors")

file(GLOB SRC src/*.cpp src/*.cxx)

include_directories(include)

add_executable(${PROJECT_NAME} ${SRC}
    include/errors_calc.hpp)

target_link_libraries(${PROJECT_NAME} ${COMMON_DEPS})