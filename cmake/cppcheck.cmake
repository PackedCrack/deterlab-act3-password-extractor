add_custom_target(cppcheck
    COMMAND /bin/bash cppcheck.sh
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Running CppCheck.."
    VERBATIM
)
