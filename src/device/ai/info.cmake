CHECK_SUB_ENABLE(MODULE_ENABLE)

if(${MODULE_ENABLE})
    file(GLOB CUR_SOURCES "${SUB_DIR}/*.cpp")
    set(CUR_INCLUDES ${LIB_DIR}/protocol ${SUB_DIR})

    SUB_ADD_SRC(CUR_SOURCES)
    SUB_ADD_INC(CUR_INCLUDES)
endif()
