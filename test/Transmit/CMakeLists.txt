
add_executable(Transmit main.cpp)
target_link_libraries(Transmit PRIVATE algorithm)

target_compile_definitions(Transmit PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
add_test(
    NAME Transmit
    COMMAND $<TARGET_FILE::Transmit>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)