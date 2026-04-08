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

#include "../include/autopilot_connector_interface.h"

/** \cond */
std::thread listenThread;
/** \endcond */

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
