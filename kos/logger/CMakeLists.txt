project (Logger)

project_header_default ("STANDARD_GNU_17:YES" "STRICT_WARNINGS:NO")

add_executable (Logger "src/main.cpp" "src/logger.cpp" "src/logger_interface.cpp"
    "../shared/src/initialization_interface.cpp" "../shared/src/ipc_messages_logger.cpp"
    "../shared/src/ipc_messages_server_connector.cpp")


add_dependencies (Logger LOGGER_EDL LOGGER_IDL)
target_compile_definitions (Logger PRIVATE ENTITY_NAME="Logger")
target_compile_features (Logger PRIVATE cxx_std_17)
bind_nk_targets (Logger vfs::client LOGGER_EDL LOGGER_IDL)
