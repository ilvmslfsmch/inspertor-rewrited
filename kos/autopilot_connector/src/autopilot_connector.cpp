/**
 * \file
 * \~English
 * \brief Implementation of methods for an autopilot communication.
 * \details The file contains implementation of methods, that provide
 * interaction between the security module with a compatible ArduPilot firmware.
 *
 * \~Russian
 * \brief Реализация методов для взаимодействия с автопилотом.
 * \details В файле реализованы методы, обеспечивающие
 * взаимодействие между модулем безопасности и прошивкой ArduPilot.
 */

#include "../include/autopilot_connector.h"
#include "../../shared/include/ipc_messages_server_connector.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** \cond */
bool armIsRequested = false;
bool correctHeader = true;
/** \endcond */

/**
 * \~English Persistently read the specified number of bytes.
 * \param[in] byteNum Number of expected bytes.
 * \param[out] bytes Pointer to where the bytes will be written.
 * \~Russian Считывает указанное число байтов.
 * \param[in] byteNum Ожидаемое число байтов.
 * \param[out] bytes Указатель на то, куда будут записаны байты.
 */
void getBytes(uint32_t byteNum, uint8_t* bytes) {
    while (true) {
        ssize_t size = readBytes(byteNum, bytes);
        if (size != byteNum) {
            bytes += size;
            byteNum -= size;
            continue;
        }
        break;
    }
}

int isArmRequested() {
    if (armIsRequested) {
        armIsRequested = false;
        return true;
    }
    else
        return false;
}

int sendAutopilotCommand(AutopilotCommand command) {
    ssize_t size = sizeof(AutopilotCommandMessage);
    uint8_t *bytes = (uint8_t*)malloc(size);

    AutopilotCommandMessage message = AutopilotCommandMessage(command);
    memcpy(bytes, &message, sizeof(AutopilotCommandMessage));

    int result = sendAutopilotBytes(bytes, size);
    free(bytes);
    return result;
}

int sendAutopilotCommand(AutopilotCommand command, int32_t value) {
    ssize_t size = sizeof(AutopilotCommandMessage) + sizeof(int32_t);
    uint8_t *bytes = (uint8_t*)malloc(size);

    AutopilotCommandMessage message = AutopilotCommandMessage(command);
    memcpy(bytes, &message, sizeof(AutopilotCommandMessage));
    memcpy(bytes + sizeof(AutopilotCommandMessage), &value, sizeof(int32_t));

    int result = sendAutopilotBytes(bytes, size);
    free(bytes);
    return result;
}

int sendAutopilotCommand(AutopilotCommand command, int32_t valueFirst, int32_t valueSecond, int32_t valueThird) {
    ssize_t size = sizeof(AutopilotCommandMessage) + 3 * sizeof(int32_t);
    uint8_t *bytes = (uint8_t*)malloc(size);

    AutopilotCommandMessage message = AutopilotCommandMessage(command);
    memcpy(bytes, &message, sizeof(AutopilotCommandMessage));

    int shift = sizeof(AutopilotCommandMessage);
    memcpy(bytes + shift, &valueFirst, sizeof(int32_t));
    shift += sizeof(int32_t);
    memcpy(bytes + shift, &valueSecond, sizeof(int32_t));
    shift += sizeof(int32_t);
    memcpy(bytes + shift, &valueThird, sizeof(int32_t));

    int result = sendAutopilotBytes(bytes, size);
    free(bytes);
    return result;
}

int sendAutopilotCommand(AutopilotCommand command, uint8_t* rawBytes, int32_t byteSize) {
    ssize_t size = sizeof(AutopilotCommandMessage) + byteSize;
    uint8_t *bytes = (uint8_t*)malloc(size);

    AutopilotCommandMessage message = AutopilotCommandMessage(command);
    memcpy(bytes, &message, sizeof(AutopilotCommandMessage));
    memcpy(bytes + sizeof(AutopilotCommandMessage), rawBytes, byteSize);

    int result = sendAutopilotBytes(bytes, size);
    free(bytes);
    return result;
}

void listenAutopilot() {
    while (true) {
        uint8_t byte;
        for (int i = 0; i < AUTOPILOT_COMMAND_MESSAGE_HEAD_SIZE; i++) {
            getBytes(sizeof(uint8_t), &byte);
            if (byte != AutopilotCommandMessageHead[i]) {
                if (correctHeader) {
                    correctHeader = false;
                    logEntry("Received message has an unknown header", ENTITY_NAME, LogLevel::LOG_WARNING);
                }
                continue;
            }
        }
        correctHeader = true;

        getBytes(sizeof(uint8_t), &byte);
        if (byte == AutopilotCommand::ArmRequest)
            armIsRequested = true;
        else if (byte == AutopilotCommand::AutopilotEvent) {
            uint32_t dataLength;
            getBytes(sizeof(uint32_t), (uint8_t*)(&dataLength));

            uint8_t* data = (uint8_t*)malloc(dataLength + 1);
            getBytes(dataLength, data);
            data[dataLength] = 0;

            char message[256] = {0};
            switch (data[0]) {
            case 1:
                snprintf(message, 256, "type=info_firmware&event=%s", data + 1);
                break;
            case 2:
                snprintf(message, 256, "type=info_obstacle&event=%s", data + 1);
                break;
            case 3:
                snprintf(message, 256, "type=arm_state&event=%s", data + 1);
                break;
            case 4:
                snprintf(message, 256, "type=mission_mode&event=%s", data + 1);
                break;
            case 5:
                snprintf(message, 256, "type=mission_command&event=%s", data + 1);
                break;
            case 6:
                snprintf(message, 256, "type=waypoint&event=%s", data + 1);
                break;
            case 7:
                snprintf(message, 256, "type=kos_command&event=%s", data + 1);
                break;
            case 8:
                snprintf(message, 256, "type=obstacle&event=%s", data + 1);
                break;
            default:
                break;
            }

            if (!publishMessage("api/events", message))
                logEntry("Failed to publish event message", ENTITY_NAME, LogLevel::LOG_WARNING);

            free(data);
        }
    }
}