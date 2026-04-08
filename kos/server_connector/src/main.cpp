/**
 * \file
 * \~English \brief Implementation of the security module ServerConnector component main loop.
 * \~Russian \brief Реализация основного цикла компонента ServerConnector модуля безопасности.
 */

#include <string>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/server_connector_interface.h"

/**
 * \~English \brief ServerConnector component main program entry point.
 * \details First, waits for the Logger component to initialize. After that, the connection to the ATM server
 * is established and the unique drone ID is determined. Then the program enters a loop,
 * where it receives IPC messages from other security module components,
 * \~Russian \brief Точка входа в основную программу компонента ServerConnector.
 * \details Сначала производится ожидание инициализации компонента Logger. После этого инициализируется
 * подключение к серверу ОРВД и определяется уникальный ID дрона. Далее программа входит в цикл, в котором
 * получает IPC-сообщения от других компонентов модуля безопасности, исполняет запрашиваемые действияи отправляет IPC-ответы.
 */
int main(void) {
    if (!initServerConnector())
        return EXIT_FAILURE;

    logEntry("Initialization is finished", ENTITY_NAME, LogLevel::LOG_INFO);

    kosipc::Application app     =  kosipc::MakeApplicationAutodetect();
    kosipc::components::Root       root;
    IServerConnector               interface;
    root.interface              = &interface;
    kosipc::EventLoop loop      =  app.MakeEventLoop(ServeStaticChannel("server_connector_connection", root));
    loop.Run();

    return EXIT_SUCCESS;
}
