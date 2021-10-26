include_guard()

find_package(Vulkan)
find_package(Python)

set(SHINJI_HOME ${CMAKE_CURRENT_LIST_DIR})

function (shinji_load TARGET_NAME)
    set(SHINJI_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/shinji.json")
    file(READ "${SHINJI_CONFIG_FILE}" SHINJI_CONFIG)
    string(JSON SHINJI_GENERATED_FILE GET "${SHINJI_CONFIG}" "generated_file")

    set(ENV{SHINJI_HOME} "${SHINJI_HOME}")
    set(ENV{CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ENV{GLSLC} "${Vulkan_GLSLC_EXECUTABLE}")

    execute_process(
            COMMAND "${Python_EXECUTABLE}" "${SHINJI_HOME}/shinji_gen.py" "${SHINJI_CONFIG_FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_sources(${TARGET_NAME} PRIVATE ${SHINJI_GENERATED_FILE})
endfunction()
