/**
 * \file
 * \~English \brief Implementation of wrapper methods that send IPC messages to Logger component.
 * \~Russian \brief Реализация методов-оберток для отправки IPC-сообщений компоненту Logger.
 */

#include <iostream>
#include <string>
#include <kosipc/api.h>

#include "../include/ipc_messages_logger.h"

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/LoggerInterface.idl.cpp.h>

using namespace kosipc::stdcpp::drone_controller;

int logEntry(char* entry, char* entity, LogLevel level) {
    // TODO: rewrite without PureClient
    uint8_t success;
    kosipc::Application app = kosipc::MakeApplicationPureClient();
    auto proxy              = app.MakeProxy<LoggerInterface>(kosipc::ConnectStaticChannel("logger_connection", "interface"));
    // TODO: rewrite in cpp way
    char message[MAX_LOG_BUFFER + 1] = {0};
    std::snprintf(message, MAX_LOG_BUFFER, "[%s] %s", entity, entry);

    try {
        proxy->Log(std::string(message), (uint8_t)level, success);
    }
    catch (...) {
        std::cerr << "Exception on proxy->Log request: message="  << std::string(message) << ", level=" << level << std::endl;
        return 0;
    }

    return 1;
}
