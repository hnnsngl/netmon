include_directories(.)

add_executable (netmon-client-example     netmon-client-example.cpp)
add_executable (netmon-client             netmon-client.cpp)
target_link_libraries(netmon-client       ${CORELIBS})
add_executable (netgroup-example          netgroup-example.cpp)
add_executable (socket-example            socket-example.cpp)
target_link_libraries(socket-example      ${CORELIBS})
