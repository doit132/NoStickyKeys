#include "keyboard_logger.h"
#include "physical_keyboard_monitor.h"
#include "virtual_keyboard_monitor.h"
#include <thread>
#include <windows.h>

using namespace keyboard;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow) {


  VirtualKeyboardMonitor virtualMonitor;
  PhysicalKeyboardMonitor physicalMonitor;

  // 设置虚拟按键监听器的状态检查回调
  virtualMonitor.SetKeyStateChecker([&physicalMonitor](DWORD vkCode) {
    return physicalMonitor.IsKeyDown(vkCode);
  });

  if (!virtualMonitor.Start() || !physicalMonitor.Start()) {
    Logger::LogError("Failed to start keyboard monitors!");
    return 1;
  }

  // 使用独立线程运行物理键盘监听
  std::thread physicalThread(
      [&physicalMonitor]() { physicalMonitor.RunLoop(); });

  // 主线程运行虚拟键盘监听
  virtualMonitor.RunMessageLoop();

  // 清理
  physicalMonitor.Stop();
  if (physicalThread.joinable()) {
    physicalThread.join();
  }

  return 0;
}
