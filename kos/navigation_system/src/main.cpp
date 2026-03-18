/**
 * \file
 * \~English \brief Implementation of the security module NavigationSystem component main loop.
 * \~Russian \brief Реализация основного цикла компонента NavigationSystem модуля безопасности.
 */

#include <string>
#include <cstring>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/navigation_system.h"
#include "../../shared/include/ipc_messages_initialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/NavigationSystem.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace drone_controller;

/** \cond */
std::thread sensorThread;
std::thread senderThread;
/** \endcond */

class INavigationSystem : public NavigationSystemInterface
{
public:
    void GetCoords(
            uint8_t& success,                   // out UInt8 success
            int32_t& lat,                       // out SInt32 lat
            int32_t& lng,                       // out SInt32 lng
            int32_t& alt                        // out SInt32 alt
            ) {

        success = getPosition(lat, lng, alt);

    }

    void GetGpsInfo(
            uint8_t& success,                   // out UInt8 success
            int32_t& dop,                       // out SInt32 dop
            int32_t& sats                       // out SInt32 sats
            ) {

        float d;
        success = getInfo(d, sats);
        std::memcpy(&dop, &d, sizeof(float));

    }

    void GetSpeed(
            uint8_t& success,                   // out UInt8 success
            int32_t& speed                      // out SInt32 speed
            ) {

        float s;
        success = getSpeed(s);
        std::memcpy(&speed, &s, sizeof(float));

    }
};
/**
 * \~English \brief AutopilotConnector component main program entry point.
 * \details First, waits for the Logger component to initialize. After that,
 * interfaces to receive current drone position are prepared. Then the program
 * enters a loop, where it receives IPC messages from other security module components,
 * performs the requested actions and sends IPC responses. In parallel with the main loop,
 * two processes are executed. In the first one, the current position data is constantly updated.
 * In the second process, current location are sent (2 times per second) to the ATM server.
 * Signing and sending are done through CredentialManager and ServerConnector components respectively.
 * \~Russian \brief Точка входа в основную программу компонента NavigationSystem.
 * \details Сначала производится ожидание инициализации компонента Logger. После этого подготавливаются интерфейсы
 * для получения данных о местоположении дрона. Далее программа входит в цикл, в котором получает IPC-сообщения
 * от других компонентов модуля безопасности, исполняет запрашиваемые действия и отправляет IPC-ответы.
 * Параллельно с основным циклом запускаются два процесса. В первом происходит постоянное обновление данных о
 * местоположении актуальными значениями. Во втором процессе происходит отправка (2 раза в секунду) данных о текущем
 * местоположении на сервер ОРВД. Подпись и отправка осуществляются через компоненты
 * CredentialManager и ServerConnector соответственно.
 */
int main(void) {
    while (!waitForInit("logger_connection", "Logger")) {
        logEntry("Failed to receive initialization notification from Logger. Trying again in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

    if (!initNavigationSystem())
        return EXIT_FAILURE;

    while (!initSensors()) {
        logEntry("Trying again to init sensors in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

    sensorThread = std::thread(getSensors);

    while (!hasPosition()) {
        logEntry("Inconsistent coordinates are received. Trying again in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

    senderThread = std::thread(sendCoords);

    logEntry("Initialization is finished", ENTITY_NAME, LogLevel::LOG_INFO);

    kosipc::Application app     =  kosipc::MakeApplicationAutodetect();
    kosipc::components::Root       root;
    INavigationSystem              interface;
    root.interface              = &interface;
    kosipc::EventLoop loop      =  app.MakeEventLoop(ServeStaticChannel("navigation_system_connection", root));
    loop.Run();

    return EXIT_SUCCESS;
}
