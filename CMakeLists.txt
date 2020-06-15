
cmake_minimum_required(VERSION 2.8)

project(cclib)

find_package(PkgConfig)

set(EXT_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/ext_libs")
set(TESTS_DIR "${CMAKE_CURRENT_BINARY_DIR}/tests")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_test("libbase_ccl_test_test" libbase_ccl_test list basic_type_array)

# - update compilation flags -
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wfatal-errors -Wno-class-memaccess")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MACOSX_RPATH ON)
endif()

# - set global variables -
if(("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") OR ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin"))
  set(OS_LINUX TRUE)
  set(OS_WINDOWS FALSE)
  add_definitions("-DLINUX")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -fprofile-arcs -ftest-coverage")
else()
  set(OS_LINUX FALSE)
  set(OS_WINDOWS TRUE)
  add_definitions("-DWINDOWS -D_CRT_SECURE_NO_WARNINGS")
endif()

define_property(GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS
    BRIEF_DOCS "Global include components relative directories"
    FULL_DOCS "Global include components relative directories")

define_property(GLOBAL PROPERTY GP_INCLUDE_COMP_HEADER_DIRS
    BRIEF_DOCS "Global include components header directories"
    FULL_DOCS "Global include components header directories")

set_property(GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS)
set_property(GLOBAL PROPERTY GP_INCLUDE_COMP_HEADER_DIRS)

set(CREATE_SHARED_LIBRARIES TRUE)

# - process component -
function(process_component)
# {{{
  set(PROC_SOURCE_FILES)
  foreach(SOURCE_DIR ${COMP_SOURCE_DIRS})
    set(SOURCE_FILES)
    aux_source_directory(${SOURCE_DIR} SOURCE_FILES)

    foreach(SOURCE_FILE ${SOURCE_FILES})
      list(APPEND PROC_SOURCE_FILES "${SOURCE_FILE}")
    endforeach()
  endforeach()

  if(${COMP_NAME} MATCHES "_[a-z]*l$")
      if (CREATE_SHARED_LIBRARIES)
          add_library(${COMP_NAME} SHARED ${PROC_SOURCE_FILES})
          set_target_properties(${COMP_NAME} PROPERTIES PREFIX "")
          install(TARGETS ${COMP_NAME} LIBRARY DESTINATION lib)
      else()
          add_library(${COMP_NAME} STATIC ${PROC_SOURCE_FILES})
          set_target_properties(${COMP_NAME} PROPERTIES PREFIX "")
      endif()
  else()
    add_executable(${COMP_NAME} ${PROC_SOURCE_FILES})

    if(${COMP_NAME} MATCHES "_test$")
      set(TESTS_DIR "${TESTS_DIR}/${COMP_NAME}")
      file(MAKE_DIRECTORY "${TESTS_DIR}")

      if(EXISTS "${COMP_DIR}/resources")
        file(COPY "${COMP_DIR}/resources" DESTINATION "${TESTS_DIR}")
      endif()

      add_test("${COMP_NAME}_all" ${COMP_NAME} all)
    endif()
  endif()

  target_include_directories(${COMP_NAME} PUBLIC ${COMP_INCLUDE_DIRS})
  target_compile_definitions(${COMP_NAME} PUBLIC ${COMP_COMP_DEFS})
  target_link_libraries(${COMP_NAME} ${COMP_LINK_LIBS})

  if(EXISTS "${COMP_DIR}/subcomponents")
    set(PARENT_COMP_NAME "${COMP_NAME}_")
    string(REGEX REPLACE "^${CMAKE_CURRENT_SOURCE_DIR}/" "" SUBCOMPONENTS_DIR "${COMP_DIR}/subcomponents")
    include_component_dir("${SUBCOMPONENTS_DIR}")
  endif()
# }}}
endfunction()

# - include component -
function(include_component COMP_REL_DIR)
# {{{
  get_property(INCLUDE_COMP_REL_DIRS GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS)

  # - component was already included -
  list(FIND INCLUDE_COMP_REL_DIRS ${COMP_REL_DIR} ID_INDEX)
  if(NOT ${ID_INDEX} EQUAL "-1")
    return()
  endif()

  set(COMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${COMP_REL_DIR}")
  get_filename_component(COMP_NAME ${COMP_DIR} NAME)
  set(COMP_NAME "${PARENT_COMP_NAME}${COMP_NAME}")
  set(PARENT_COMP_NAME)

  if(EXISTS ${COMP_DIR}/CMakeLists.txt)

    # - read component cmake file -
    set(CONF_DO_BUILD FALSE)
    include(${COMP_DIR}/CMakeLists.txt)

    if(CONF_DO_BUILD)

      # - append this component header directory -
      list(APPEND CONF_HEADER_DIRS "${COMP_DIR}/header_files")

      # - process component dependencies -
      foreach(COMP_COMP_REL_DIR ${CONF_COMPONENTS})
        include_component(${COMP_COMP_REL_DIR})

        # FIXME debug output header directories
        #message("${COMP_NAME} -> ${COMP_COMP_REL_DIR}")

        get_property(INCLUDE_COMP_REL_DIRS GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS)
        get_property(INCLUDE_COMP_HEADER_DIRS GLOBAL PROPERTY GP_INCLUDE_COMP_HEADER_DIRS)

        list(FIND INCLUDE_COMP_REL_DIRS ${COMP_COMP_REL_DIR} ID_INDEX)

        if(${ID_INDEX} EQUAL "-1")
          message("WARNING: Component ${COMP_NAME} depends on unavailable component: ${COMP_COMP_REL_DIR}")
          return()
        endif()

        # - update component header directories -
        list(GET INCLUDE_COMP_HEADER_DIRS ${ID_INDEX} COMP_COMP_INCLUDE_DIRS_STR)
        string(REPLACE "," ";" COMP_COMP_INCLUDE_DIRS "${COMP_COMP_INCLUDE_DIRS_STR}")
        list(APPEND CONF_HEADER_DIRS ${COMP_COMP_INCLUDE_DIRS})

        # - update component link libraries -
        get_filename_component(COMP_COMP_NAME ${COMP_COMP_REL_DIR} NAME)
        list(APPEND CONF_LINK_LIBS ${COMP_COMP_NAME})

      endforeach()

      # - remove header directories duplicities -
      list(REMOVE_DUPLICATES CONF_HEADER_DIRS)
      string(REPLACE ";" "," CONF_HEADER_DIRS_STR "${CONF_HEADER_DIRS}")

      # FIXME debug output header directories
      #message("${COMP_NAME} -> ${CONF_HEADER_DIRS_STR}")

      set_property(GLOBAL APPEND PROPERTY GP_INCLUDE_COMP_REL_DIRS ${COMP_REL_DIR})
      set_property(GLOBAL APPEND PROPERTY GP_INCLUDE_COMP_HEADER_DIRS "${CONF_HEADER_DIRS_STR}")

      set(COMP_SOURCE_DIRS "${COMP_DIR}/source_files")
      set(COMP_INCLUDE_DIRS ${CONF_INCLUDE_DIRS} ${CONF_HEADER_DIRS})
      set(COMP_COMP_DEFS ${CONF_COMP_DEFS})
      set(COMP_LINK_LIBS ${CONF_LINK_LIBS})

      process_component()
    endif()
  endif()
# }}}
endfunction()

# - include component directory -
function(include_component_dir COMPS_DIR)
# {{{
file(GLOB MODULE_DIRS "" "${CMAKE_CURRENT_SOURCE_DIR}/${COMPS_DIR}/*")
foreach(COMP_DIR ${MODULE_DIRS})
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" COMP_DIR_WP "${COMP_DIR}")
  include_component(${COMP_DIR_WP})
endforeach()
# }}}
endfunction()

# - process component directories -
if(NOT DEFINED CS_COMPONENT_DIRS)
  set(CS_COMPONENT_DIRS
      "src/cclib/libs"
      "src/cclib/tests"
      )
endif()

foreach(COMPONENT_DIR ${CS_COMPONENT_DIRS})
  include_component_dir("${COMPONENT_DIR}")
endforeach()

# - process components -
if(NOT DEFINED CS_COMPONENTS)
  set(CS_COMPONENTS)
endif()

foreach(COMPONENT ${CS_COMPONENTS})
  include_component("${COMPONENT}")
endforeach()

## - process programs -
#if(NOT DEFINED CS_INSTALL_BINS)
#  set(CS_INSTALL_BINS
#      cs_recorder
#      cs_player
#      cs_info
#      cs_fuser
#      cs_config
#      libquirc_cll_test_payload)
#endif()

install(TARGETS ${CS_INSTALL_BINS} RUNTIME DESTINATION bin)

