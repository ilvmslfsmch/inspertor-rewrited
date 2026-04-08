project (CredentialManager)

include (platform/nk)

project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

if (SERVER)
    set (CREDENTIAL_MANAGER_SRC "src/credential_manager_online.cpp")
else()
    set (CREDENTIAL_MANAGER_SRC "src/credential_manager_offline.cpp")
endif()

add_executable (CredentialManager "src/main.cpp" "src/credential_manager.cpp"
    ${CREDENTIAL_MANAGER_SRC} "../shared/src/ipc_messages_logger.cpp"
    "../shared/src/ipc_messages_server_connector.cpp")
add_dependencies (CredentialManager CREDENTIAL_MANAGER_EDL SERVER_CONNECTOR_IDL)

target_compile_definitions (CredentialManager PRIVATE ENTITY_NAME="Credential Manager")
target_compile_definitions (CredentialManager PRIVATE PARTNER_ID="${PARTNER_ID}")
bind_nk_targets (CredentialManager vfs::client MbedTLS::mbedtls ${CMAKE_THREAD_LIBS_INIT} CREDENTIAL_MANAGER_EDL SERVER_CONNECTOR_IDL)