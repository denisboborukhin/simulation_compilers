cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

set(PROJECT_NAME rv32_interpret)
project(${PROJECT_NAME} LANGUAGES CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER clang++)

include_directories(parser/include interpreter/include ELFIO)

set (SOURCE_LIB parser/src/elf_rv32_parser.cpp)
set (SOURCE_INTERPRET interpreter/src/rv32_interpreter.cpp)

add_library (elf_rv32_parser STATIC ${SOURCE_LIB})

add_executable (${PROJECT_NAME} ${SOURCE_INTERPRET})

#sanitizers
option(ADD_SANITIZERS "Build with sanitizers" ON)
if (ADD_SANITIZERS)
    message ("Build with sanitizers")

    add_compile_options (-fsanitize=address -fsanitize=undefined)
    set_target_properties (elf_rv32_parser PROPERTIES LINK_FLAGS "-fsanitize=address -fsanitize=undefined")
    set_target_properties (${PROJECT_NAME} PROPERTIES LINK_FLAGS "-fsanitize=address -fsanitize=undefined")
endif ()

target_link_libraries (${PROJECT_NAME} PUBLIC elf_rv32_parser)

option(TESTING "Include testing" OFF)
#tests
if (TESTING)
    message ("Include testing")

    find_package (GTest REQUIRED)
    include_directories (${GTEST_INCLUDE_DIRS})

    add_executable (${PROJECT_NAME}_tests ${SOURCE_TEST})
    
    if (ADD_SANITIZERS)
        set_target_properties (${PROJECT_NAME}_tests PROPERTIES LINK_FLAGS "-fsanitize=address -fsanitize=undefined")
    endif()
    target_link_libraries (${PROJECT_NAME}_tests PUBLIC GTest::GTest GTest::Main ${PROJECT_NAME})
    
    enable_testing ()

    add_custom_target (run_${PROJECT_NAME}_tests 
        COMMENT "Running ${PROJECT_NAME}_tests"
        COMMAND ./${PROJECT_NAME}_tests
    )
    add_dependencies (run_${PROJECT_NAME}_tests ${PROJECT_NAME}_tests)
   
endif ()

#clang-format
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    message (clang-format " from " ${CMAKE_SOURCE_DIR}/.clang-format)

    file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.hpp)
    add_custom_target(
        clang_format
        COMMAND clang-format
        -i
        -style=file:${CMAKE_SOURCE_DIR}/.clang-format
        ${ALL_SOURCE_FILES}
    )
endif()



