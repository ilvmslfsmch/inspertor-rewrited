/**
 * \file
 * \~English \brief Implementation of the security module NavigationSystem component main loop.
 * \~Russian \brief Реализация основного цикла компонента NavigationSystem модуля безопасности.
 */

#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/navigation_system_interface.h"

/** \cond */
std::thread sensorThread;
std::thread senderThread;
/** \endcond */

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
