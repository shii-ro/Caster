cmake_minimum_required(VERSION 3.10)
project(ms_emu C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include the command that downloads libraries
include(FetchContent)

# define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    # setup the declare
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(${libName})
endfunction()

# add SDL3 support
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.0)
else()
    message(STATUS "Using local SDL3")
endif()


# Add source files
add_executable(ms_emu
    WIN32
    main.c
    core/sms.c
    mem/mem.c
    cpu/z80.c
    cpu/z80_dasm.c
    cpu/z80_op.c
    cpu/z80_op_execute.c
    cpu/z80_dd_op_execute.c
    cpu/z80_ed_op_execute.c
    cpu/z80_fd_op_execute.c
    cpu/z80_cb_op_execute.c
    cpu/z80_fd_cb_op_execute.c
    cpu/z80_dd_cb_op_execute.c
    cpu/z80_flags.c
    cpu/z80_test.c
)



# Include directory for headers
target_include_directories(ms_emu PRIVATE cpu core SDL3::SDL3)

# set the include directory
target_include_directories(ms_emu PUBLIC ${SDL3_INCLUDE_DIRS})

# link all libraries to the project
target_link_libraries(ms_emu PRIVATE SDL3::SDL3)
target_compile_definitions(ms_emu PRIVATE SDL_MAIN_USE_CALLBACKS)
target_link_options(ms_emu PRIVATE -static)