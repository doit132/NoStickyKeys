#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "keyboard_common.h"

namespace keyboard {

class Logger {
public:
    static void LogKeyEvent(DWORD vkCode, DWORD scanCode, DWORD flags, bool isKeyDown) {
        std::stringstream ss;
        ss << "Key Event: " << KeyCodeConverter::GetKeyName(vkCode)
           << " (vkCode=0x" << std::hex << std::setw(2) << std::setfill('0') << vkCode
           << ", scanCode=0x" << std::hex << scanCode
           << ", flags=0x" << std::hex << flags << ")"
           << (isKeyDown ? " pressed" : " released");
        LogMessage(ss.str());
    }

    static void LogKeyStates(const KeyStates& states) {
        std::stringstream ss;
        ss << "[States: "
           << "LCtrl:" << (states.leftCtrl ? "1" : "0") << " "
           << "RCtrl:" << (states.rightCtrl ? "1" : "0") << " "
           << "LShift:" << (states.leftShift ? "1" : "0") << " "
           << "RShift:" << (states.rightShift ? "1" : "0") << " "
           << "LAlt:" << (states.leftAlt ? "1" : "0") << " "
           << "RAlt:" << (states.rightAlt ? "1" : "0")
           << "]";
        LogMessage(ss.str());
    }

    static void LogError(const std::string& message) {
        std::cerr << "Error: " << message << std::endl;
    }

    static void LogMessage(const std::string& message) {
        std::cout << message << std::endl;
    }
};

} // namespace keyboard
