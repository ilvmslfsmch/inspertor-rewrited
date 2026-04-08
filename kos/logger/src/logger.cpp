/**
 * \file
 * \~English
 * \brief Implementation of methods for work with a log.
 * \details The file contains implementation of methods, that are required to log messages.
 *
 * \~Russian
 * \brief Реализация методов для работы с логом.
 * \details В файле реализованы методы, необходимые для логирования сообщений.
 */

#include "../include/logger.h"
#include "../../shared/include/ipc_messages_server_connector.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdio>
#include <filesystem>
#include <component/logrr/cpp/logger.h>

/** \cond */
bool serverIsReady = false;
char logMessage[1024] = {0};
/** \endcond */

int createLog() {
    if (std::filesystem::exists("/logs/apps/Drone Controller/Drone Controller.log")) {
        int count = std::distance(std::filesystem::directory_iterator("/logs/apps/Drone Controller"),
            std::filesystem::directory_iterator{});
        char newFileName[64] = {0};
        snprintf(newFileName, 64, "/logs/apps/Drone Controller/prev_%d.log", count);
        std::rename("/logs/apps/Drone Controller/Drone Controller.log", newFileName);
        std::remove("/logs/apps/Drone Controller/Drone Controller.log");
    }

    logrr::Init("Drone Controller");
    return 1;
}

int addLogEntry(char* entry, int level) {
    char logHeader[32] = {0};
    switch (level) {
    case 0:
        LOG(TRACE, entry);
        strcpy(logHeader, "[Trace]");
        break;
    case 1:
        LOG(DEBUG, entry);
        strcpy(logHeader, "[Debug]");
        break;
    case 2:
        LOG(INFO, entry);
        strcpy(logHeader, "[Info]");
        break;
    case 3:
        LOG(WARNING, entry);
        strcpy(logHeader, "[Warning]");
        break;
    case 4:
        LOG(ERROR, entry);
        strcpy(logHeader, "[Error]");
        break;
    case 5:
        LOG(CRITICAL, entry);
        strcpy(logHeader, "[Critical]");
        break;
    }

    if (serverIsReady) {
        time_t now = time(NULL);
        char timeStr[32] = {0};
        strftime(timeStr, 32, "[%Y-%m-%d %H:%M:%S]", localtime(&now));
        snprintf(logMessage, 1024, "log=%s %s %s", timeStr, logHeader, entry);
        publishMessage("api/logs", logMessage);
    }
    else if (strstr(entry, "[Server Connector] Initialization is finished"))
        serverIsReady = true;

    return 1;
}