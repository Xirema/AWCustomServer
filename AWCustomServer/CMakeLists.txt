set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.89.0 REQUIRED COMPONENTS charconv json)
find_package(OpenSSL REQUIRED)
find_package(libzip REQUIRED)
add_compile_options(-Wall -Werror -pedantic-errors -std=c++26 -fconcepts-diagnostics-depth=2)
include_directories(include/)
include_directories(${Boost_INCLUDE_DIRS})
add_library(Core OBJECT 
  src/Archive.cpp src/Properties.cpp src/ResourcePacks.cpp
  src/RestFunctions.cpp src/RestServer.cpp src/SQLUtil.cpp
  src/StateLookup.cpp src/db/Data.cpp src/GameManager.cpp
  src/db/State.cpp src/Calculator.cpp src/ZipFunctions.cpp
  src/db/Resource.cpp src/db/General.cpp src/db/DBTablesV0.0.1.cpp
  src/Cli.cpp
)
add_executable(Server $<TARGET_OBJECTS:Core> src/Main.cpp)
target_link_libraries(Server ${Boost_LIBRARIES} ssl crypto dl zip)
add_executable(CalculatorTest $<TARGET_OBJECTS:Core> src/test/CalculatorTest.cpp)
target_link_libraries(CalculatorTest ${Boost_LIBRARIES} ssl crypto dl zip)
add_executable(ZipTest $<TARGET_OBJECTS:Core> src/test/ZipTest.cpp)
target_link_libraries(ZipTest ${Boost_LIBRARIES} ssl crypto dl zip)
add_executable(JSSTest $<TARGET_OBJECTS:Core> src/test/JSSTest.cpp)
target_link_libraries(JSSTest ${Boost_LIBRARIES} ssl crypto dl zip)
add_executable(ChangeTestGame $<TARGET_OBJECTS:Core> src/test/ChangeTestGame.cpp)
target_link_libraries(ChangeTestGame ${Boost_LIBRARIES} ssl crypto dl zip)
add_executable(ExtractResourcesFromJson $<TARGET_OBJECTS:Core> src/test/ExtractResourcesFromJson.cpp)
target_link_libraries(ExtractResourcesFromJson ${Boost_LIBRARIES} ssl crypto dl zip)
