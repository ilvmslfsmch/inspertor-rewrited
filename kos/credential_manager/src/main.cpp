/**
 * \file
 * \~English \brief Implementation of the security module CredentialManager component main loop.
 * \~Russian \brief Реализация основного цикла компонента CredentialManager модуля безопасности.
 */

#include <string>
#include <iostream>
#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include "../include/credential_manager.h"
#include "../../shared/include/initialization_interface.h"
#include "../../shared/include/ipc_messages_initialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/CredentialManager.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace drone_controller;


class ICredentialManager : public CredentialManagerInterface
{
public:
    void SignMessage(
            const std::string& message,         // in string<MaxMessageLength> message
            uint8_t& success,                   // out UInt8 success
            std::string& signature              // out string<MaxSignatureLength> signature
            ) {

        //TODO: Rewrite to cpp way
        char m[MaxMessageLength + 1] = {0};
        char s[MaxSignatureLength + 1] = {0};
        message.copy(m, message.size() + 1);
        success = getMessageSignature(m, s);
        signature = std::string(s);

    }

    void CheckSignature(
            const std::string& message,         // in string<MaxMessageLength> message
            uint8_t& success,                   // out UInt8 success
            uint8_t& correct                    // out UInt8 correct
            ) {

        //TODO: Rewrite to cpp way
        char m[MaxMessageLength + 1] = {0};
        message.copy(m, message.size() + 1);
        success = checkMessageSignature(m, correct);

    }
};
/**
 * \~English \brief CredentialManager component main program entry point.
 * \details First, waits for the Logger component to initialize. After that, the saved RSA key
 * of the security module is loaded (if there is no saved key, a new one is generated).
 * Then, through the ServerConnector component, the public parts of the key are exchanged with the ATM server.
 * The public part of the ATM server key is saved. Then the program enters a loop, where it receives
 * IPC messages from other security module components, performs the requested actions and sends IPC responses.
 * \~Russian \brief Точка входа в основную программу компонента CredentialManager.
 * \details Сначала производится ожидание инициализации компонента Logger. После этого загружается сохраненный
 * RSA-ключ модуля безопасности (при отсутствии сохраненного ключа генерируется новый). Затем через компонент
 * ServerConnector происходит обмен открытыми частями ключа с сервером ОРВД. Открытая часть ключа сервера ОРВД
 * сохраняется. Далее программа входит в цикл, в котором получает IPC-сообщения от других компонентов модуля
 * безопасности, исполняет запрашиваемые действия и отправляет IPC-ответы.
 */
int main(void) {
    while (!waitForInit("logger_connection", "Logger")) {
        logEntry("Failed to receive initialization notification from Logger. Trying again in 1s", ENTITY_NAME, LogLevel::LOG_WARNING);
        sleep(1);
    }

    if (!getRsaKey())
        return EXIT_FAILURE;

    if (!shareRsaKey())
        return EXIT_FAILURE;

    logEntry("Initialization is finished", ENTITY_NAME, LogLevel::LOG_INFO);

    kosipc::Application app     =  kosipc::MakeApplicationAutodetect();
    kosipc::components::Root       root;
    ICredentialManager             interface;
    root.interface              = &interface;
    kosipc::EventLoop loop      =  app.MakeEventLoop(ServeStaticChannel("credential_manager_connection", root));
    loop.Run();

    return EXIT_SUCCESS;
}
