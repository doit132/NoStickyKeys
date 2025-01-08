# NoStickyKey

一个用于防止按键卡住的 Windows 工具，支持物理按键和模拟按键的状态同步。

## 功能特点

- 监控物理按键和虚拟按键状态
- 自动检测并释放卡住的按键
- 支持 AutoHotkey 等按键模拟软件
- 实时显示按键状态

## 系统要求

- Windows 操作系统
- 管理员权限（用于安装键盘钩子）
- [Interception 驱动](https://github.com/oblitum/Interception)

## 编译和运行

1. 安装 xmake 构建工具
2. 克隆代码库
3. 在项目目录下运行：
   ```bash
   xmake
   xmake run
   ```

## 项目结构

```
src/
├── keyboard_common.h    - 通用按键定义和工具
├── keyboard_logger.h    - 日志输出工具
├── physical_keyboard_monitor.h/.cpp - 物理键盘监控
├── virtual_keyboard_monitor.h/.cpp  - 虚拟键盘监控
└── main.cpp            - 程序入口
```

## 使用说明

1. 以管理员权限运行程序
2. 程序会自动监控键盘状态
3. 当检测到按键卡住时，会自动释放
4. 使用 Ctrl+C 退出程序

## 注意事项

- 需要管理员权限
- 建议先安装 Interception 驱动
- 如果使用其他按键模拟软件，建议先测试兼容性
