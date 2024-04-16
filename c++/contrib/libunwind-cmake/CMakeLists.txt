set(LIBUNWIND_SOURCE_DIR "${CxxTestDemo_SOURCE_DIR}/contrib/libunwind")
set(LIBUNWIND_BINARY_DIR "${CxxTestDemo_BINARY_DIR}/contrib/libunwind")

set(CMAKE_C_STANDARD 11)

set(PKG_MAJOR "1")
set(PKG_MINOR "8")
set(PKG_EXTRA "0")
set(PACKAGE_STRING "libunwind")
set(PACKAGE_BUGREPORT "")

set(TARGET_AMD64 1)
set(arch x86_64)
add_definitions(-D__x86_64__)
add_definitions(-D__amd64__)
add_definitions(-D__linux__)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DHAVE_SYS_LINK_H)
# remove_definitions(-DUNW_REMOTE_ONLY)

include(CheckCSourceCompiles)
include(CheckIncludeFiles)
include_directories(${LIBUNWIND_SOURCE_DIR}/include/tdep)
include_directories(${LIBUNWIND_SOURCE_DIR}/src)
include_directories(${LIBUNWIND_SOURCE_DIR}/include)
include_directories(${LIBUNWIND_SOURCE_DIR}/include/remote)
include_directories(${LIBUNWIND_BINARY_DIR}/include/tdep)
include_directories(${LIBUNWIND_BINARY_DIR}/include)

configure_file(${LIBUNWIND_SOURCE_DIR}/include/libunwind-common.h.in ${LIBUNWIND_BINARY_DIR}/include/libunwind-common.h)
configure_file(${LIBUNWIND_SOURCE_DIR}/include/libunwind.h.in ${LIBUNWIND_BINARY_DIR}/include/libunwind.h)
configure_file(${LIBUNWIND_SOURCE_DIR}/include/tdep/libunwind_i.h.in ${LIBUNWIND_BINARY_DIR}/include/tdep/libunwind_i.h)

SET(libunwind_coredump_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_accessors.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_create.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_destroy.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_access_mem.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_elf_map_image.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_find_proc_info.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_get_proc_name.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_get_elf_filename.c

    ${LIBUNWIND_SOURCE_DIR}/src/mi/init.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UPT_elf.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UPT_access_fpreg.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UPT_get_dyn_info_list_addr.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UPT_put_unwind_info.c
    ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UPT_resume.c
)

# List of arch-independent files needed by generic library (libunwind-$ARCH):
SET(libunwind_la_SOURCES_generic
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gdyn-extract.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gdyn-remote.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gfind_dynamic_proc_info.c
    # The Gget_accessors.c implements the same function as Lget_accessors.c, so
    # the source is excluded here to prevent name clash
    #mi/Gget_accessors.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gget_proc_info_by_ip.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gget_proc_name.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gput_dynamic_unwind_info.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gdestroy_addr_space.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gget_reg.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gset_reg.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gget_fpreg.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gset_fpreg.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gset_caching_policy.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gset_cache_size.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gset_iterate_phdr_function.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Gget_elf_filename.c
)

SET(libunwind_la_SOURCES_os_linux
    ${LIBUNWIND_SOURCE_DIR}/src/os-linux.c
)

SET(libunwind_la_SOURCES_os                 ${libunwind_la_SOURCES_os_linux})
SET(libunwind_la_SOURCES_os_local           ${libunwind_la_SOURCES_os_linux_local})
SET(libunwind_la_SOURCES_x86_os             ${LIBUNWIND_SOURCE_DIR}/src/x86/Gos-linux.c)
SET(libunwind_x86_la_SOURCES_os             ${LIBUNWIND_SOURCE_DIR}/src/x86/getcontext-linux.S)
SET(libunwind_la_SOURCES_x86_os_local       ${LIBUNWIND_SOURCE_DIR}/src/x86/Los-linux.c)
SET(libunwind_la_SOURCES_x86_64_os          ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gos-linux.c)
SET(libunwind_la_SOURCES_x86_64_os_local    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Los-linux.c)
list(APPEND libunwind_coredump_la_SOURCES   ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_access_reg_linux.c
                                            ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_get_threadinfo_prstatus.c
                                            ${LIBUNWIND_SOURCE_DIR}/src/coredump/_UCD_get_mapinfo_linux.c)

SET(libunwind_la_SOURCES_common
    ${libunwind_la_SOURCES_os}
    ${LIBUNWIND_SOURCE_DIR}/src/mi/init.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/flush_cache.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/mempool.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/strerror.c
)

SET(libunwind_la_SOURCES_local_nounwind
    ${libunwind_la_SOURCES_os_local}
    ${LIBUNWIND_SOURCE_DIR}/src/mi/backtrace.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/dyn-cancel.c 
    ${LIBUNWIND_SOURCE_DIR}/src/mi/dyn-info-list.c 
    ${LIBUNWIND_SOURCE_DIR}/src/mi/dyn-register.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Ldyn-extract.c 
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lfind_dynamic_proc_info.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lget_accessors.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lget_proc_info_by_ip.c 
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lget_proc_name.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lput_dynamic_unwind_info.c 
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Ldestroy_addr_space.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lget_reg.c   
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lset_reg.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lget_fpreg.c 
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lset_fpreg.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lset_caching_policy.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lset_cache_size.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lset_iterate_phdr_function.c
    ${LIBUNWIND_SOURCE_DIR}/src/mi/Lget_elf_filename.c
)

SET(libunwind_la_SOURCES_local
    ${libunwind_la_SOURCES_local_nounwind}
    ${libunwind_la_SOURCES_local_unwind}
)

SET(libunwind_dwarf_common_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/global.c
)

SET(libunwind_dwarf_local_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lexpr.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lfde.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lparser.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lpe.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lfind_proc_info-lsb.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lfind_unwind_table.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Lget_proc_info_in_range.c
)

SET(libunwind_dwarf_generic_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gexpr.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gfde.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gparser.c 
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gpe.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gfind_proc_info-lsb.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gfind_unwind_table.c
    ${LIBUNWIND_SOURCE_DIR}/src/dwarf/Gget_proc_info_in_range.c
)

SET(libunwind_elf32_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/elf32.c
)

SET(libunwind_elf64_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/elf64.c
)
SET(libunwind_elfxx_la_SOURCES
    ${LIBUNWIND_SOURCE_DIR}/src/elfxx.c
)

SET(libunwind_la_SOURCES_x86_64_common
    ${libunwind_la_SOURCES_common}
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/is_fpreg.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/regname.c
)

# The list of files that go into libunwind:
SET(libunwind_la_SOURCES_x86_64
    ${libunwind_la_SOURCES_x86_64_common}
    ${libunwind_la_SOURCES_x86_64_os_local}
    ${libunwind_la_SOURCES_local}
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/setcontext.S
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lapply_reg_state.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lreg_states_iterate.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lcreate_addr_space.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lget_save_loc.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lglobal.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Linit.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Linit_local.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Linit_remote.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lget_proc_info.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lregs.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lresume.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lstash_frame.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Lstep.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Ltrace.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/getcontext.S
)

# The list of files that go into libunwind-x86_64:
SET(libunwind_x86_64_la_SOURCES_x86_64
    ${libunwind_la_SOURCES_x86_64_common}
    ${libunwind_la_SOURCES_x86_64_os}
    ${libunwind_la_SOURCES_generic}
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gapply_reg_state.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Greg_states_iterate.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gcreate_addr_space.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gget_save_loc.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gglobal.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Ginit.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Ginit_local.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Ginit_remote.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gget_proc_info.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gregs.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gresume.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gstash_frame.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gstep.c
    ${LIBUNWIND_SOURCE_DIR}/src/x86_64/Gtrace.c
)

SET(libunwind_la_SOURCES                    ${libunwind_la_SOURCES_x86_64})
SET(libunwind_remote_la_SOURCES             ${libunwind_x86_64_la_SOURCES_x86_64})
SET(libunwind_elf_la_SOURCES                ${libunwind_elf64_la_SOURCES})
list(APPEND libunwind_setjmp_la_SOURCES     ${LIBUNWIND_SOURCE_DIR}/src/x86_64/longjmp.S ${LIBUNWIND_SOURCE_DIR}/src/x86_64/siglongjmp.SA)

add_library(libunwind
  OBJECT
  ${LIBUNWIND_SOURCE_DIR}/src/remote/win/missing-functions.c
  # ${libunwind_la_SOURCES}  Local...
  ${libunwind_remote_la_SOURCES}
    # Commented out above for LOCAL + REMOTE runtime build
  ${LIBUNWIND_SOURCE_DIR}/src/mi/Gget_accessors.c
  # ${libunwind_dwarf_local_la_SOURCES}
  ${libunwind_dwarf_common_la_SOURCES}
  ${libunwind_dwarf_generic_la_SOURCES}
  ${libunwind_elf_la_SOURCES}
)