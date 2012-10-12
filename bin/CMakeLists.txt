include_directories(.)

add_executable (netmon-client-example     netmon-client-example.cpp
                                          ../src/netmon-lib.cpp  ../src/socket.cpp)

add_executable (netmon-client             netmon-client.cpp
                                          ../src/netmon-lib.cpp ../src/socket.cpp)
target_link_libraries(netmon-client       ${CORELIBS})

add_executable (netgroup-example          netgroup-example.cpp)

add_executable (socket-example            socket-example.cpp)
target_link_libraries(socket-example      ${CORELIBS})

add_executable (netmon-hosts-test         netmon-hosts-test.cpp
                                          ../src/netmon-hosts.cpp)
target_link_libraries(netmon-hosts-test   ${CORELIBS})

