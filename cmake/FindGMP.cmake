find_path(GMP_INCLUDE_DIR NAMES gmp.h)
find_library(GMP_LIBRARIES NAMES gmp libgmp)
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP DEFAULT_MSG GMP_INCLUDE_DIR GMP_LIBRARIES)

if(GMP_FOUND)
  set(GMP_INCLUDE_DIRS ${GMP_INCLUDE_DIR})
  
  if(NOT TARGET gmp::gmp)
    add_library(gmp::gmp UNKNOWN IMPORTED)
    set_target_properties(gmp::gmp PROPERTIES
      IMPORTED_LOCATION "${GMP_LIBRARIES}"
      INTERFACE_INCLUDE_DIRECTORIES "${GMP_INCLUDE_DIR}")
  endif()

  if(NOT TARGET gmp::libgmp)
    add_library(gmp::libgmp ALIAS gmp::gmp)
  endif()

  if(GMPXX_LIBRARIES)
      if(NOT TARGET gmp::gmpxx)
        add_library(gmp::gmpxx UNKNOWN IMPORTED)
        set_target_properties(gmp::gmpxx PROPERTIES
          IMPORTED_LOCATION "${GMPXX_LIBRARIES}"
          INTERFACE_INCLUDE_DIRECTORIES "${GMP_INCLUDE_DIR}")
        target_link_libraries(gmp::gmpxx INTERFACE gmp::gmp)
      endif()
  endif()
endif()
