
find_package(OpenSSL 1.0.2)

set(CONF_COMPONENTS
  "src/cclib/libs/libbase_ccl")
set(CONF_HEADER_DIRS)

if(OPENSSL_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${OPENSSL_INCLUDE_DIR})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${OPENSSL_LIBRARIES})
endif()
