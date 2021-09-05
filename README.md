## Table of Contents

---

[toc]

# 简介

---

cocmd 工具通过在进程空间中加载多个 cmd.dll 实例实现了真正意义上的多线程执行批处理脚本，其中 cmd.dll 修改自 Win7 SP1 位中的 32 位 cmd.exe

不同于 start /b 的启动方式，因为环境变量和进程环境是绑定的，所以多个脚本线程间可以实时共享变量以及 GDI 句柄等资源

不过肯定没有 powershell 这种原生的好用就是了（x

兼容 Windows 7~10 32/64 位系统

# 编译

---

运行目录下 make.bat 即可编译 cocmd，需要 mingw64 以及 libpthread

# 线程控制

---

cocmd 通过多个 cmd 实例并行执行脚本，支持通过锁机制对线程进行控制，避免冲突

## 启动多线程控制台

---

`cocmd [string1] [string2] ...`

举例：`cocmd 1.bat 2.bat 3.bat`

在同一进程空间下并行执行 1.bat、2.bat、3.bat 三个脚本

## 启动交互式控制台

---

`cocmd ""`

仅启动一个 cmd 实例

## 获取全局线程锁

---

`set thread=lock`

## 释放全局线程锁

---

`set thread=unlock`

# API 交互

---

cocmd 集成了由 Aloxaf 开发的 CAPIx，所有线程均支持 CAPI SetCall 调用

## 内存操作

---

`set CAPI=Mem Alloc/Free/Put/Print/Copy`

## 调用函数

---

`set CAPI=API Call/Exec`

## 获取基本信息

---

`set CAPI=CAPIDLL Ver`

## 设置调用方式

---

`set CAPI=SetCall Enable/Disable`

`set CAPI=GetCall Enable/Disable`

# 项目引用

---

## CAPIx

https://github.com/Aloxaf/CAPIx

GNU Lesser General Public License v3.0

