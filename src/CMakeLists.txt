
file( GLOB UTIL_SOURCES "util/*.cpp")
file( GLOB COM_SOURCES "comalgorithm/*.cpp" )

add_library( utils STATIC ${UTIL_SOURCES} )
# target_link_directories( utils PUBLIC  )

add_library( comalgorithm STATIC ${COM_SOURCES})
target_link_directories( utils PUBLIC utils )

