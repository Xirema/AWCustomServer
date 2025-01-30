set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.84.0 REQUIRED COMPONENTS system json)
find_package(OpenSSL REQUIRED)
add_compile_options(-Wall -Werror -pedantic-errors -std=c++26 -fconcepts-diagnostics-depth=2)
include_directories(include/)
include_directories(${Boost_INCLUDE_DIRS})
add_library(Core OBJECT 
  src/Archive.cpp src/Properties.cpp src/ResourcePacks.cpp
  src/RestFunctions.cpp src/RestServer.cpp src/SQLUtil.cpp
  src/StateLookup.cpp src/db/Data.cpp src/jss/Types.cpp src/GameManager.cpp
  src/db/State.cpp src/Calculator.cpp
)
add_executable(Server $<TARGET_OBJECTS:Core> src/Main.cpp)
target_link_libraries(Server ${Boost_LIBRARIES} ssl crypto dl)
add_executable(CalculatorTest $<TARGET_OBJECTS:Core> src/test/CalculatorTest.cpp)
target_link_libraries(CalculatorTest ${Boost_LIBRARIES} ssl crypto dl)
