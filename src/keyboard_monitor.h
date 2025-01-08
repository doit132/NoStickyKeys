#pragma once
#include <windows.h>

class KeyboardMonitor {
public:
    KeyboardMonitor();
    ~KeyboardMonitor();

    bool Start();
    void Stop();
    void RunMessageLoop();

    struct VirtualKeyStates {
        bool leftCtrl = false;
        bool rightCtrl = false;
        bool leftShift = false;
        bool rightShift = false;
        bool leftAlt = false;
        bool rightAlt = false;
    };

    const VirtualKeyStates& GetKeyStates() const { return virtualKeyStates; }

private:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void PrintKeyStates() const;
    void UpdateKeyState(DWORD vkCode, bool isKeyDown);

    static KeyboardMonitor* instance;
    HHOOK keyboardHook;
    VirtualKeyStates virtualKeyStates;
    bool isRunning;
};
