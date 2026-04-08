/**
 * \file
 * \~English
 * \brief Implementation of methods for simulator autopilot communication.
 * \details The file contains implementation of methods,
 * that provide interaction between the security module and a
 * compatible ArduPilot SITL firmware.
 *
 * \~Russian
 * \brief Реализация методов для взаимодействия с симулятором автопилота.
 * \details В файле реализованы методы, обеспечивающие
 * взаимодействие между модулем безопасности и совместимой
 * SITL-прошивкой ArduPilot.
 */

#include "../include/autopilot_connector.h"

#include <kos_net.h>

/** \cond */
int autopilotSocket = NULL;
#ifdef IS_INSPECTOR
uint16_t autopilotPort = 5775;
#else
uint16_t autopilotPort = 5765;
#endif
/** \endcond */

int initAutopilotConnector() {
    if (!wait_for_iface(DEFAULT_INTERFACE, IWF_EXISTS, DEFAULT_TIMEOUT) || !configure_net_iface(DEFAULT_INTERFACE, DEFAULT_ADDR, DEFAULT_MASK, DEFAULT_GATEWAY, DEFAULT_MTU)) {
        logEntry("Connection to network has failed", ENTITY_NAME, LogLevel::LOG_ERROR);
        return 0;
    }

    return 1;
}

int initConnection() {
    close(autopilotSocket);
    autopilotSocket = NULL;

    if ((autopilotSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        logEntry("Failed to create socket", ENTITY_NAME, LogLevel::LOG_WARNING);
        return 0;
    }

    struct sockaddr_in address = { 0 };
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SIMULATOR_IP);
    address.sin_port = htons(autopilotPort);

    if (connect(autopilotSocket, (struct sockaddr*)&address, sizeof(address)) != 0) {
        char logBuffer[256] = {0};
        snprintf(logBuffer, 256, "Connection to %s:%d has failed", SIMULATOR_IP, autopilotPort);
        logEntry(logBuffer, ENTITY_NAME, LogLevel::LOG_WARNING);
        return 0;
    }

    return 1;
}

ssize_t readBytes(uint32_t byteNum, uint8_t* bytes) {
    return read(autopilotSocket, bytes, byteNum);
}

int sendAutopilotBytes(uint8_t* bytes, ssize_t size) {
    write(autopilotSocket, bytes, size);

    return 1;
}