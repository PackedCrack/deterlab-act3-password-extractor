SET(MAIN_SRCS 
main.cpp
defines.hpp
logger.hpp
profiler.hpp
CPasswordTester.cpp
CPasswordTester.hpp
)

add_executable(${MAIN_PROJECT} ${MAIN_SRCS})