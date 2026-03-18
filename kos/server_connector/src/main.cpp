/**
 * \file
 * \~English \brief Implementation of the security module ServerConnector component main loop.
 * \~Russian \brief Реализация основного цикла компонента ServerConnector модуля безопасности.
 */

#include <string>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/server_connector.h"
#include "../../shared/include/initialization_interface.h"
#include "../../shared/include/ipc_messages_initialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/ServerConnector.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace drone_controller;


class IServerConnector : public ServerConnectorInterface
{
public:
    void GetBoardId(
            uint8_t& success,                   // out UInt8 success
            std::string& id                     // out string<MaxIdLength> id
            ){

        //TODO: rewrite to cpp way
        id = std::string(getBoardName());
        success = (id.size() > 0);

    }

    void SendRequest(
            const std::string& query,           // in string<MaxQueryLength> query
            uint8_t& success,                   // out UInt8 success
            std::string& response               // out string<MaxResponseLength> response
            ){

        //TODO: rewrite to cpp way
        char q[MaxQueryLength + 1] = {0};
        query.copy(q, query.size() + 1);
        char r[MaxResponseLength + 1] = {0};

        success = requestServer(q, r, MaxResponseLength + 1);
        response = std::string(r);

    }

    void PublishMessage(
            const std::string& topic,           // in string<MaxTopicLength> topic
            const std::string& publication,     // in string<MaxPublicationLength> publication
            uint8_t& success                    // out UInt8 success
            ){

        //TODO: rewrite to cpp way
        char t[MaxTopicLength + 1] = {0};
        topic.copy(t, topic.size() + 1);
        char p[MaxPublicationLength + 1] = {0};
        publication.copy(p, publication.size() + 1);

        success = publish(t, p);

    }

    void ReceiveSubscription(
            const std::string& topic,           // in string<MaxTopicLength> topic
            std::string& subscription,          // out string<MaxSubscriptionLength> subscription
            uint8_t& success                    // out UInt8 success
            ){

        //TODO: rewrite to cpp way
        char t[MaxTopicLength + 1] = {0};
        topic.copy(t, topic.size() + 1);
        char s[MaxSubscriptionLength + 1] = {0};

        success = getSubscription(t, s, MaxSubscriptionLength + 1);
        subscription = std::string(s);

    }

};


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
    while (!waitForInit("logger_connection", "Logger")) {
        logEntry("Failed to receive initialization notification from Logger. Trying again in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

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
