/**
 * \file
 * \~English
 * \brief Declaration of LoggerInterface IDL-interface methods.
 * \details Interaction between Logger component and other components
 * of the security module is done via IPC message sent through LoggerInterface IDL-interface.
 * The file contains declaration of the C++ methods of this interface. All methods have a standard set of parameters.
 * \param[in] self Pointer to the LoggerInterface structure.
 * \param[in] req Pointer to a structure with fixed-length data received in an IPC message.
 * \param[in] reqArena Pointer to a structure with variable length data received in an IPC message.
 * \param[out] res Pointer to a structure with fixed-length data sent in an IPC response.
 * \param[out] resArena Pointer to a structure with variable length data sent in an IPC response.
 * \return Returns NK_EOK if message and response were successfully exchanged.
 *
 * \~Russian
 * \brief Объявление методов IDL-интерфейса LoggerInterface.
 * \details Взаимодействие с компонентом Logger другими компонентами
 * модуля безопасности осуществляется через отправку IPC-сообщение через
 * IDL-интерфейс LoggerInterface. В этом файле объявлены методы этого интерфейса
 * на языке C++. Все методы имеют стандартный набор параметров.
 * \param[in] self Указатель на структуру, соответствующую интерфейсу LoggerInterface.
 * \param[in] req Указатель на структуру с полученными в IPC-сообщении данными фиксированной длины.
 * \param[in] reqArena Указатель на структуру с полученными в IPC-сообщении данными произвольной длины.
 * \param[out] res Указатель на структуру с отправляемыми в IPC-ответе данными фиксированной длины.
 * \param[out] resArena Указатель на структуру с отправляемыми в IPC-ответе данными произвольной длины.
 * \return Возвращает NK_EOK в случае успешного обмена сообщением и ответом.
 */

#pragma once

#include "../include/logger.h"

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/Logger.edl.cpp.h>

using namespace kosipc::stdcpp::drone_controller;

class ILogger : public LoggerInterface {
public:
/**
 * \~English IPC message handler. See \ref logEntry.
 * \~Russian Обработчик IPC-сообщения. См. \ref logEntry.
 */
    void Log(const std::string& logEntry, uint8_t logLevel, uint8_t& success) {
        //TODO: Rewrite to cpp way
        char l[MaxLogEntry + 1] = {0};
        logEntry.copy(l, logEntry.size() + 1);
        success = addLogEntry(l, logLevel);
    }
};