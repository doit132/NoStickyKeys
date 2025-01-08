#pragma once
#include <windows.h>

class VirtualKeyboardMonitor {
public:
    VirtualKeyboardMonitor();
    ~VirtualKeyboardMonitor();

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

    static VirtualKeyboardMonitor* instance;
    HHOOK keyboardHook;
    VirtualKeyStates virtualKeyStates;
    bool isRunning;
};
