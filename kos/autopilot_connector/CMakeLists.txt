project (AutopilotConnector)

include (platform/nk)

project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

if (SIMULATION)
    set (AUTOPILOT_CONNECTOR_SRC "src/autopilot_connector_simulator.cpp")
else()
    set (AUTOPILOT_CONNECTOR_SRC "src/autopilot_connector_real.cpp")
endif()

add_executable (AutopilotConnector "src/main.cpp" ${AUTOPILOT_CONNECTOR_SRC}
    "src/autopilot_connector.cpp" "../shared/src/ipc_messages_logger.cpp"
    "../shared/src/ipc_messages_server_connector.cpp")
add_dependencies (AutopilotConnector AUTOPILOT_CONNECTOR_EDL AUTOPILOT_CONNECTOR_IDL)

target_compile_definitions (AutopilotConnector PRIVATE ENTITY_NAME="Autopilot Connector")

if (SIMULATION)
    target_compile_definitions (AutopilotConnector PRIVATE SIMULATOR_IP="${SIMULATOR_IP}")
    bind_nk_targets (AutopilotConnector vfs::client AUTOPILOT_CONNECTOR_EDL AUTOPILOT_CONNECTOR_IDL)
else()
    bind_nk_targets (AutopilotConnector bsp::client uart::client AUTOPILOT_CONNECTOR_EDL AUTOPILOT_CONNECTOR_IDL)
endif()

if (INSPECTOR_ROLE)
    target_compile_definitions (AutopilotConnector PRIVATE IS_INSPECTOR=TRUE)
endif()