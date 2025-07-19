
file( GLOB UTIL_SOURCES "util/*.cpp")
file( GLOB COM_SOURCES "algorithm/*.cpp" )
list( REMOVE_ITEM COM_SOURCES   "${CMAKE_CURRENT_SOURCE_DIR}/algorithm/constellation.cpp" 
                                "${CMAKE_CURRENT_SOURCE_DIR}/algorithm/ofdm.cpp")

add_library( utils STATIC ${UTIL_SOURCES} )
# target_link_directories( utils PUBLIC  )

add_library( algorithm STATIC ${COM_SOURCES})
target_link_libraries( algorithm PUBLIC utils )

