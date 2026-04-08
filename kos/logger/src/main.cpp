/**
 * \file
 * \~English \brief Implementation of the security module Logger component main loop.
 * \~Russian \brief Реализация основного цикла компонента Logger модуля безопасности.
 */

#include <string>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/logger_interface.h"
#include "../../shared/include/ipc_messages_logger.h"

/**
 * \~English \brief Logger component main program entry point.
 * \details First, creates a log, that saves all logged messages to a file and prints them
 * to the console at the same time. Then the program enters a loop, where it receives
 * IPC messages from other security module components, performs the requested actions and sends IPC responses.
 * \~Russian \brief Точка входа в основную программу компонента Logger.
 * \details Сначала создается лог, производящий запись логируемых сообщений в файл и их параллельный вывод в консоль.
 * Далее программа входит в цикл, в котором получает IPC-сообщения от других компонентов модуля
 * безопасности, исполняет запрашиваемые действия и отправляет IPC-ответы.
 */
int main(void) {
    if (!createLog())
        return EXIT_FAILURE;

    addLogEntry("[Logger] Initialization is finished", (uint8_t)(LogLevel::LOG_INFO));

    kosipc::Application app     =  kosipc::MakeApplicationAutodetect();
    kosipc::components::Root       root;
    ILogger                        interface;
    root.interface              = &interface;
    kosipc::EventLoop loop      =  app.MakeEventLoop(ServeStaticChannel("logger_connection", root));
    loop.Run();

    return EXIT_SUCCESS;
}