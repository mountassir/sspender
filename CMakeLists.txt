CMAKE_MINIMUM_REQUIRED ( VERSION 3.0 )

CMAKE_POLICY ( VERSION 3.0 )

PROJECT ( sspender )

SET (CMAKE_CXX_STANDARD 11)

find_library(LIBCONFIG_PATH config++)
find_package (Threads)

ADD_EXECUTABLE ( sspender
  src/sspender.cpp
  src/ConfigParser.cpp
  src/Cpu.cpp
  src/Disk.cpp
  src/Device.cpp
  src/Manager.cpp
  src/Monitor.cpp
  src/PartitionTable.cpp
  src/WatchDog.cpp
  src/utils.cpp
  )

target_link_libraries(sspender "${LIBCONFIG_PATH}" ${CMAKE_THREAD_LIBS_INIT})

INSTALL (TARGETS sspender DESTINATION /usr/local/bin)
