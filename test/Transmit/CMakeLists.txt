
add_executable(Transmit main.cpp)
target_link_libraries(Transmit PRIVATE dev)
add_test(
    NAME Transmit
    COMMAND $<TARGET_FILE::Transmit>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)