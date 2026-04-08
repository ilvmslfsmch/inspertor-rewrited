/**
 * \file
 * \~English
 * \brief Declaration of AutopilotConnectorInterface IDL-interface methods.
 * \details Interaction between AutopilotConnector component and other components
 * of the security module is done via IPC message sent through AutopilotConnectorInterface IDL-interface.
 * The file contains declaration of the C++ methods of this interface. All methods have a standard set of parameters.
 * \param[in] self Pointer to the AutopilotConnectorInterface structure.
 * \param[in] req Pointer to a structure with fixed-length data received in an IPC message.
 * \param[in] reqArena Pointer to a structure with variable length data received in an IPC message.
 * \param[out] res Pointer to a structure with fixed-length data sent in an IPC response.
 * \param[out] resArena Pointer to a structure with variable length data sent in an IPC response.
 * \return Returns NK_EOK if message and response were successfully exchanged.
 *
 * \~Russian
 * \brief Объявление методов IDL-интерфейса AutopilotConnectorInterface.
 * \details Взаимодействие с компонентом AutopilotConnector другими компонентами
 * модуля безопасности осуществляется через отправку IPC-сообщение через
 * IDL-интерфейс AutopilotConnectorInterface. В этом файле объявлены методы этого интерфейса
 * на языке C++. Все методы имеют стандартный набор параметров.
 * \param[in] self Указатель на структуру, соответствующую интерфейсу AutopilotConnectorInterface.
 * \param[in] req Указатель на структуру с полученными в IPC-сообщении данными фиксированной длины.
 * \param[in] reqArena Указатель на структуру с полученными в IPC-сообщении данными произвольной длины.
 * \param[out] res Указатель на структуру с отправляемыми в IPC-ответе данными фиксированной длины.
 * \param[out] resArena Указатель на структуру с отправляемыми в IPC-ответе данными произвольной длины.
 * \return Возвращает NK_EOK в случае успешного обмена сообщением и ответом.
 */

#pragma once

#include "../include/autopilot_connector.h"

#define NK_USE_UNQUALIFIED_NAMES
#include <drone_controller/AutopilotConnector.edl.cpp.h>

using namespace kosipc::stdcpp::drone_controller;

class IAutopilotConnector : public AutopilotConnectorInterface {
public:
/**
 * \~English IPC message handler. See \ref waitForArmRequest.
 * \~Russian Обработчик IPC-сообщения. См. \ref waitForArmRequest.
 */
    void WaitForArmRequest(uint8_t& success) {
        success = isArmRequested();
    }

/**
 * \~English IPC message handler. See \ref permitArm.
 * \~Russian Обработчик IPC-сообщения. См. \ref permitArm.
 */
    void PermitArm(uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::ArmPermit);
    }

/**
 * \~English IPC message handler. See \ref forbidArm.
 * \~Russian Обработчик IPC-сообщения. См. \ref forbidArm.
 */
    void ForbidArm(uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::ArmForbid);
    }

/**
 * \~English IPC message handler. See \ref pauseFlight.
 * \~Russian Обработчик IPC-сообщения. См. \ref pauseFlight.
 */
    void PauseFlight(uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::PauseFlight);
    }

/**
 * \~English IPC message handler. See \ref resumeFlight.
 * \~Russian Обработчик IPC-сообщения. См. \ref resumeFlight.
 */
    void ResumeFlight(uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::ResumeFlight);
    }

/**
 * \~English IPC message handler. See \ref abortMission.
 * \~Russian Обработчик IPC-сообщения. См. \ref abortMission.
 */
    void AbortMission(uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::AbortMission);
    }

/**
 * \~English IPC message handler. See \ref changeSpeed.
 * \~Russian Обработчик IPC-сообщения. См. \ref changeSpeed.
 */
    void ChangeSpeed(int32_t speed, uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::ChangeSpeed, speed);
    }

/**
 * \~English IPC message handler. See \ref changeAltitude.
 * \~Russian Обработчик IPC-сообщения. См. \ref changeAltitude.
 */
    void ChangeAltitude(int32_t altitude, uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::ChangeAltitude, altitude);
    }

/**
 * \~English IPC message handler. See \ref changeWaypoint.
 * \~Russian Обработчик IPC-сообщения. См. \ref changeWaypoint.
 */
    void ChangeWaypoint(int32_t latitude, int32_t longitude, int32_t altitude, uint8_t& success) {
        success = sendAutopilotCommand(AutopilotCommand::ChangeWaypoint, latitude, longitude, altitude);
    }

/**
 * \~English IPC message handler. See \ref setMission.
 * \~Russian Обработчик IPC-сообщения. См. \ref setMission.
 */
    void SetMission(const std::vector<uint8_t>& mission, uint32_t size, uint8_t& success) {
        uint8_t m[MaxMissionLength] = {0};
        std::memcpy(m, mission.data(), mission.size());
        success = sendAutopilotCommand(AutopilotCommand::SetMission, m, size);
    }
};