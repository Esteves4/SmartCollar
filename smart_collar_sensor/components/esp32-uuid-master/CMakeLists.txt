# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

#set(COMPONENT_SRCS "uuid/uuid.c" )
#set(COMPONENT_ADD_INCLUDEDIRS "uuid/include")

idf_component_register(
  SRCS "uuid/uuid.c"
  INCLUDE_DIRS . uuid/include )

#register_component()

target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Wstrict-prototypes -DLOG_LOCAL_LEVEL=ESP_LOG_VERBOSE)

