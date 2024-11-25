function(load_encryption_key_from_env_file)

    if (DEFINED ENV{GITHUB_ACTIONS} AND ENV{GITHUB_ACTIONS} STREQUAL "true")
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.env")
            set(ADMIN_KEY $ENV{MASTER_ADMIN_KEY})
            set(GROUP_DECRYPT_KEY $ENV{GROUP_DECRYPT_KEY})
            set(AERO_DYNAMICS_KEY $ENV{AERO_DYNAMICS_KEY})
            set(DELTA_SIM_KEY $ENV{DELTA_SIM_KEY})
            set(LUNAR_SIM_KEY $ENV{LUNAR_SIM_KEY})
            set(OUROBOROS_KEY $ENV{OUROBOROS_KEY})
            set(QBIT_SIM_KEY $ENV{QBIT_SIM_KEY})
        else ()
            message(FATAL_ERROR ".env file is missing. Please create the file with necessary variables.")
        endif ()
    else()
        file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.env" ENV_FILE)
        string(REGEX MATCH "MASTER_ADMIN_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(ADMIN_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "GROUP_DECRYPT_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(GROUP_DECRYPT_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "AERO_DYNAMICS_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(AERO_DYNAMICS_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "DELTA_SIM_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(DELTA_SIM_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "LUNAR_SIM_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(LUNAR_SIM_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "OUROBOROS_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(OUROBOROS_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "QBIT_SIM_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(QBIT_SIM_KEY ${CMAKE_MATCH_1})
    endif()


    if (ADMIN_KEY)
        message("${Blue}ADMIN_KEY found, defining: ${Green}ADMIN_KEY${ColorReset}")
        add_compile_definitions(ADMIN_KEY="${ADMIN_KEY}")
    else ()
        message(FATAL_ERROR "ADMIN_KEY is not set. Please set it in the .env file.")
    endif ()

    if (GROUP_DECRYPT_KEY)
        message("${Blue}GROUP_DECRYPT_KEY found, defining: ${Green}GROUP_DECRYPT_KEY${ColorReset}")
        add_compile_definitions(GROUP_DECRYPT_KEY="${GROUP_DECRYPT_KEY}")
    else ()
        message(FATAL_ERROR "GROUP_DECRYPT_KEY is not set. Please set it in the .env file.")
    endif ()

    if (AERO_DYNAMICS_KEY)
        message("${Blue}AERO_DYNAMICS_KEY found, defining: ${Green}AERO_DYNAMICS_KEY${ColorReset}")
        add_compile_definitions(AERO_DYNAMICS_KEY="${AERO_DYNAMICS_KEY}")
    else ()
        message(FATAL_ERROR "AERO_DYNAMICS_KEY is not set. Please set it in the .env file.")
    endif ()

    if (DELTA_SIM_KEY)
        message("${Blue}DELTA_SIM_KEY found, defining: ${Green}DELTA_SIM_KEY${ColorReset}")
        add_compile_definitions(DELTA_SIM_KEY="${DELTA_SIM_KEY}")
    else ()
        message(FATAL_ERROR "DELTA_SIM_KEY is not set. Please set it in the .env file.")
    endif ()

    if (LUNAR_SIM_KEY)
        message("${Blue}LUNAR_SIM_KEY found, defining: ${Green}LUNAR_SIM_KEY${ColorReset}")
        add_compile_definitions(LUNAR_SIM_KEY="${LUNAR_SIM_KEY}")
    else ()
        message(FATAL_ERROR "LUNAR_SIM_KEY is not set. Please set it in the .env file.")
    endif ()

    if (OUROBOROS_KEY)
        message("${Blue}OUROBOROS_KEY found, defining: ${Green}OUROBOROS_KEY${ColorReset}")
        add_compile_definitions(OUROBOROS_KEY="${OUROBOROS_KEY}")
    else ()
        message(FATAL_ERROR "OUROBOROS_KEY is not set. Please set it in the .env file.")
    endif ()

    if (QBIT_SIM_KEY)
        message("${Blue}QBIT_SIM_KEY found, defining: ${Green}QBIT_SIM_KEY${ColorReset}")
        add_compile_definitions(QBIT_SIM_KEY="${QBIT_SIM_KEY}")
    else ()
        message(FATAL_ERROR "QBIT_SIM_KEY is not set. Please set it in the .env file.")
    endif ()
endfunction()


function(improved_loading_keys)
    # Admin
    set(ADMIN_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/admin_key.bin")
    set(ADMIN_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/admin_key.h")
    execute_process(
        COMMAND xxd -n "admin_key" -i ${ADMIN_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${ADMIN_KEY_HEADER}" "${xxd_output}")

    # Aero
    set(AERO_DYNAMICS_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/aero_dynamics_key.bin")
    set(AERO_DYNAMICS_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/aero_dynamics_key.h")
    execute_process(
        COMMAND xxd -n "aero_dynamics_key" -i ${AERO_DYNAMICS_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${AERO_DYNAMICS_KEY_HEADER}" "${xxd_output}")

    set(DELTA_SIM_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/delta_sim_key.bin")
    set(DELTA_SIM_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/delta_sim_key.h")
    execute_process(
        COMMAND xxd -n "delta_sim_key" -i ${DELTA_SIM_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${DELTA_SIM_KEY_HEADER}" "${xxd_output}")

    # Group
    set(GROUP_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/group_key.bin")
    set(GROUP_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/group_key.h")
    execute_process(
        COMMAND xxd -n "group_key" -i ${GROUP_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${GROUP_KEY_HEADER}" "${xxd_output}")

    # Lunar Sim
    set(LUNAR_SIM_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/lunar_sim_key.bin")
    set(LUNAR_SIM_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/lunar_sim_key.h")
    execute_process(
        COMMAND xxd -n "lunar_sim_key" -i ${LUNAR_SIM_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${LUNAR_SIM_KEY_HEADER}" "${xxd_output}")

    # Ouroboros
    set(OUROBOROS_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/ouroboros_key.bin")
    set(OUROBOROS_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/ouroboros_key.h")
    execute_process(
        COMMAND xxd -n "ouroboros_key" -i ${OUROBOROS_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${OUROBOROS_KEY_HEADER}" "${xxd_output}")

    # Qbit Sim
    set(QBIT_SIM_KEY_BIN "${CMAKE_CURRENT_SOURCE_DIR}/keys/qbit_sim_key.bin")
    set(QBIT_SIM_KEY_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/keys/include/qbit_sim_key.h")
    execute_process(
        COMMAND xxd -n "qbit_sim_key" -i ${QBIT_SIM_KEY_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE xxd_result
        OUTPUT_VARIABLE xxd_output
        ERROR_VARIABLE xxd_error
    )
    file(WRITE "${QBIT_SIM_KEY_HEADER}" "${xxd_output}")
endfunction(improved_loading_keys)
