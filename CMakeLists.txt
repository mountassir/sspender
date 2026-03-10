CMAKE_MINIMUM_REQUIRED ( VERSION 3.20 )
CMAKE_POLICY ( VERSION 3.20 )
PROJECT ( sspender VERSION 1.1 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)

ADD_EXECUTABLE (sspender
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

target_compile_features(sspender PRIVATE)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(sspender PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(sspender PRIVATE tomlplusplus::tomlplusplus Threads::Threads)

INSTALL (TARGETS sspender DESTINATION /usr/local/bin)
