/**
 * \file
 * \~English
 * \brief Declaration of CredentialManagerInterface IDL-interface methods.
 * \details Interaction between CredentialManager component and other components
 * of the security module is done via IPC message sent through CredentialManagerInterface IDL-interface.
 * The file contains declaration of the C++ methods of this interface. All methods have a standard set of parameters.
 * \param[in] self Pointer to the CredentialManagerInterface structure.
 * \param[in] req Pointer to a structure with fixed-length data received in an IPC message.
 * \param[in] reqArena Pointer to a structure with variable length data received in an IPC message.
 * \param[out] res Pointer to a structure with fixed-length data sent in an IPC response.
 * \param[out] resArena Pointer to a structure with variable length data sent in an IPC response.
 * \return Returns NK_EOK if message and response were successfully exchanged.
 *
 * \~Russian
 * \brief Объявление методов IDL-интерфейса CredentialManagerInterface.
 * \details Взаимодействие с компонентом CredentialManager другими компонентами
 * модуля безопасности осуществляется через отправку IPC-сообщение через
 * IDL-интерфейс CredentialManagerInterface. В этом файле объявлены методы этого интерфейса
 * на языке C++. Все методы имеют стандартный набор параметров.
 * \param[in] self Указатель на структуру, соответствующую интерфейсу CredentialManagerInterface.
 * \param[in] req Указатель на структуру с полученными в IPC-сообщении данными фиксированной длины.
 * \param[in] reqArena Указатель на структуру с полученными в IPC-сообщении данными произвольной длины.
 * \param[out] res Указатель на структуру с отправляемыми в IPC-ответе данными фиксированной длины.
 * \param[out] resArena Указатель на структуру с отправляемыми в IPC-ответе данными произвольной длины.
 * \return Возвращает NK_EOK в случае успешного обмена сообщением и ответом.
 */

#pragma once

#include "../include/credential_manager.h"

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/CredentialManager.edl.cpp.h>

using namespace kosipc::stdcpp::drone_controller;

class ICredentialManager : public CredentialManagerInterface {
public:
/**
 * \~English IPC message handler. See \ref signMessage.
 * \~Russian Обработчик IPC-сообщения. См. \ref signMessage.
 */
    void SignMessage(const std::string& message, uint8_t& success, std::string& signature) {
        //TODO: Rewrite to cpp way
        char m[MaxMessageLength + 1] = {0};
        char s[MaxSignatureLength + 1] = {0};
        message.copy(m, message.size() + 1);
        success = getMessageSignature(m, s);
        signature = std::string(s);
    }

/**
 * \~English IPC message handler. See \ref checkSignature.
 * \~Russian Обработчик IPC-сообщения. См. \ref checkSignature.
 */
    void CheckSignature(const std::string& message, uint8_t source, uint8_t& success, uint8_t& correct) {
        //TODO: Rewrite to cpp way
        char m[MaxMessageLength + 1] = {0};
        message.copy(m, message.size() + 1);
        success = checkMessageSignature(m, MessageSource(source), correct);
    }
};