CMAKE_MINIMUM_REQUIRED ( VERSION 3.20 )
CMAKE_POLICY ( VERSION 3.20 )
PROJECT ( sspender VERSION 1.1 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  tomlplusplus
  GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
  GIT_TAG        v3.4.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(tomlplusplus)

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

include(GNUInstallDirs)
INSTALL (TARGETS sspender RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
