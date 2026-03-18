/**
 * \file
 * \~English \brief Implementation of the security module AutopilotConnector component main loop.
 * \~Russian \brief Реализация основного цикла компонента AutopilotConnector модуля безопасности.
 */

#include <string>
#include <cstring>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/autopilot_connector.h"
#include "../../shared/include/initialization_interface.h"
#include "../../shared/include/ipc_messages_initialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <thread>

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/AutopilotConnector.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace drone_controller;

/** \cond */
std::thread listenThread;
/** \endcond */
class IAutopilotConnector : public AutopilotConnectorInterface
{
public:
    void WaitForArmRequest(
            uint8_t& success                    //  out UInt8 success
            ) {

        success = isArmRequested();

    }

    void PermitArm(
            uint8_t& success                    //  out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::ArmPermit);

    }

    void ForbidArm(
            uint8_t& success                    //  out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::ArmForbid);

    }

    void PauseFlight(
            uint8_t& success                    //  out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::PauseFlight);

    }

    void ResumeFlight(
            uint8_t& success                    //  out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::ResumeFlight);

    }

    void AbortMission(
            uint8_t& success                    //  out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::AbortMission);

    }

    void ChangeSpeed(
            int32_t speed,                      // in SInt32 speed
            uint8_t& success                    // out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::ChangeSpeed, speed);

    }

    void ChangeAltitude(
            int32_t altitude,                   // in SInt32 altitude
            uint8_t& success                    // out UInt8 success
            ) {

        success = sendAutopilotCommand(AutopilotCommand::ChangeAltitude, altitude);

    }

    void ChangeWaypoint(
            int32_t latitude,                   // in SInt32 latitude
            int32_t longitude,                  // in SInt32 longitude
            int32_t altitude,                   // in SInt32 altitude
            uint8_t& success                    // out UInt8 success
            ) {

        success = sendAutopilotCommand(
                AutopilotCommand::ChangeWaypoint,
                latitude,
                longitude,
                altitude
                );

    }

    void SetMission(
            const std::vector<uint8_t>& mission,// in sequence<UInt8, MaxMissionLength> mission
            uint32_t size,                      // in UInt32 size
            uint8_t& success                    // out UInt8 success
            ) {

        uint8_t m[MaxMissionLength] = {0};
        std::memcpy(m, mission.data(), mission.size());
        success = sendAutopilotCommand(AutopilotCommand::SetMission, m, size);
    }
};

/**
 * \~English \brief AutopilotConnector component main program entry point.
 * \details First, waits for the Logger component to initialize. After that,
 * communication with the autopilot is prepared and established. Then the program
 * enters a loop, where it receives IPC messages from other security module components,
 * performs the requested actions and sends IPC responses.
 * \~Russian \brief Точка входа в основную программу компонента AutopilotConnector.
 * \details Сначала производится ожидание инициализации компонента Logger. После этого подготавливается
 * и устанавливается связь с автопилотом. Далее программа входит в цикл, в котором получает IPC-сообщения
 * от других компонентов модуля безопасности, исполняет запрашиваемые действия и отправляет IPC-ответы.
 */
int main(void) {
    while (!waitForInit("logger_connection", "Logger")) {
        logEntry("Failed to receive initialization notification from Logger. Trying again in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

    if (!initAutopilotConnector())
        return EXIT_FAILURE;

    while (!initConnection()) {
        logEntry("Trying again to connect in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

    listenThread = std::thread(listenAutopilot);

    logEntry("Initialization is finished", ENTITY_NAME, LogLevel::LOG_INFO);

    kosipc::Application app     =  kosipc::MakeApplicationAutodetect();
    kosipc::components::Root       root;
    IAutopilotConnector            interface;
    root.interface              = &interface;
    kosipc::EventLoop loop      =  app.MakeEventLoop(ServeStaticChannel("autopilot_connector_connection", root));
    loop.Run();

    return EXIT_SUCCESS;
}
