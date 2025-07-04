cmake_minimum_required(VERSION 3.10)
project(ms_emu C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add source files
add_executable(ms_emu
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
target_include_directories(ms_emu PRIVATE cpu core)