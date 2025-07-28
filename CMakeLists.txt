cmake_minimum_required(VERSION 3.7)
project(dos-app)

set (CMAKE_RANLIB ${CMAKE_FIND_ROOT_PATH}/djgpp/i586-pc-msdosdjgpp/bin/ranlib)
set (CMAKE_CXX_COMPILER_RANLIB ${CMAKE_FIND_ROOT_PATH}/djgpp/i586-pc-msdosdjgpp/bin/ranlib)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_DEBUG_POSTFIX d)

add_compile_options(-Wall -std=c++23 -save-temps -U __STRICT_ANSI__ -Wno-array-bounds -fext-numeric-literals)
# -march=pentium3 -msse

link_directories(../has/build)
link_directories(../hag/build)

add_compile_options(
    "$<$<CONFIG:RELEASE>:-DRELEASE>"
    "$<$<CONFIG:DEBUG>:-DDEBUG>"
)

add_compile_options(
    "$<$<CONFIG:RELEASE>:-O3>"
)

#dxe's will need an export list
add_link_options(
    "$<$<CONFIG:RELEASE>:-s>"
)

#add_compile_options(-D MOCK)

set (ENV{DJDIR} "${CMAKE_FIND_ROOT_PATH}/djgpp/")
set (ENV{DXE_LD_LIBRARY_PATH} "${CMAKE_FIND_ROOT_PATH}/djgpp/lib/gcc/i586-pc-msdosdjgpp/12.1.0/")
message("Environment variable DJDIR: $ENV{DJDIR}")
message("Environment variable DXE_LD_LIBRARY_PATH: $ENV{DXE_LD_LIBRARY_PATH}")

include_directories(
    ../has/include
    ../hag/include
    include
    )

add_library(ham
            src/drivers/creative/shared/system.cpp
            src/drivers/gravis/shared/system.cpp
            src/drivers/gravis/gus/driver.cpp
            src/file/mod.cpp
            src/file/s3m.cpp
            src/file/song.cpp
            src/player/player.cpp
            src/driver.cpp
           )

add_executable(test
               tests/test.cpp
              )

#libraries are linked in reverse order
target_link_libraries(test
                      ham
                     )

target_link_libraries(test debug hassupd)
target_link_libraries(test debug hasd)
target_link_libraries(test optimized hassup)
target_link_libraries(test optimized has)
target_link_libraries(test debug vgad)
target_link_libraries(test debug hagsupd)
target_link_libraries(test debug hagd)
target_link_libraries(test optimized vga)
target_link_libraries(test optimized hagsup)
target_link_libraries(test optimized hag)
