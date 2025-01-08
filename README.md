# NoStickyKey

>这是我用 windsurf 写的一个小工具，用于防止按键卡住。

我们在使用一些模拟按键软件时, 软件有时不会释放按键, 导致按键卡住。我在使用 AutoHotkey 等模拟按键软件时经常遇到的问题，所以我写了一个小工具来尝试解决这个问题。 #个人编写

这个 README.md 文档也是用 windsurf 自动生成的, 仅供参考

## 功能特点

- 监控物理按键和虚拟按键状态
- 自动检测并释放卡住的按键
- 不影响 AutoHotkey, quicker 等模拟按键软件的运行

## 系统要求

- Windows 操作系统
- [Interception 驱动](https://github.com/oblitum/Interception)

## 编译和运行

1. 安装 xmake 构建工具
2. 克隆代码库
3. 在项目目录下运行：
   ```bash
   xmake
   xmake run
   ```

## 下载使用

请到 release 页面下载最新版本

## 注意事项

- 尽量管理员权限启动软件
- 建议先安装 Interception 驱动
