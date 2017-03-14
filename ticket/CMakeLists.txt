
include_directories(include)
file(GLOB sources src/*.cpp)

add_executable(ticket_service ${sources})
target_link_libraries(ticket_service microhttpd)
