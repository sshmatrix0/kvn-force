//
// Created by ksp on 13.04.2026.
//
#pragma once
enum class MessageType : int {
    START,
    STOP,
    CONNECTING,
    CONNECTED,
    FAILED,
    DISCONNECTED
};