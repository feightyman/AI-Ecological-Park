# 智眸 AI 生态园管系统 (Smart Eye AI Ecological Park Management System) 🌱

## 📖 项目简介

**“智眸 AI 生态园管系统”** 旨在应对现代精细化农业对低成本、高可靠性、智能化环境监测与控制方案的迫切需求。本项目采用**异构双 MCU 架构**，以搭载 Cortex-M4 与 Cortex-M0+ 双核的 **英飞凌 PSoC 6 (CY8C62xA)** 作为核心主控，运行 **RT-Thread 实时操作系统**；并引入 **ESP32-S3** 作为 AI 视觉与语音计算的专职协处理器。

系统实现了从传统的“单一数据监测”向\*\*“数据-视觉双模态交叉验证”\*\*的跨越。不仅能在本地进行高效的闭环控制与低功耗管理，还能结合 OneNET 云平台与 3D 数字孪生技术，实现边缘智能的持续进化与可视化农业管理。
![Image_1752137454155](https://github.com/user-attachments/assets/7256f2b5-c68b-441c-951e-2144de9761c4)
![Image_1752137459145](https://github.com/user-attachments/assets/e5c4116c-ab75-46b0-bf93-26ae9e284ce8)
![IMG_20250710_165207](https://github.com/user-attachments/assets/9c9674cc-f1e8-4f3d-ad03-c1fb6d2b067c)


-----

## ✨ 核心创新点

1.  **“数据-视觉”双模态交叉验证机制**：首创性地将 PSoC 6 采集的物理传感器定量数据（如土壤湿度），与 ESP32-S3 提供的作物视觉表型定性分析（如叶片萎蔫状态）进行数据融合。解决传统农业物联网中“高湿度环境下植物依然萎蔫（如烂根、病害）”导致的误判问题。
2.  **极寒级低功耗控制**：深度结合 PSoC 6 的电源管理特性与 RT-Thread PM 组件，实现睡眠模式下仅 **8.5µA** 的极致功耗，满足农业物联网设备长期野外部署的需求。
3.  **云边协同的数字孪生**：基于 OneNET 建立环境与作物生长的历史数据库，并在上位机实现 3D 可视化数字孪生，未来可从“反应式控制”向“预测式控制”演进。

-----

## 🛠️ 技术实现与系统架构 (深度解析)

本项目的技术栈涵盖了从底层驱动、RTOS 调度、网络通信到 AI 算法部署的全链路嵌入式开发。

### 1\. 异构双 MCU 架构与通信

  * **主控节点 (Infineon PSoC 6 CY8C62xA)**：
      * 作为系统的“小脑”，负责时序极其严格的传感器轮询、数据滤波、本地自动化逻辑决策（如水泵与补光灯的 PID 控制）以及 LVGL 交互界面的渲染。
      * **RT-Thread 调度**：利用 RT-Thread 丰富的组件（IPC 通信、软定时器、工作队列等），将任务拆分为：`Sensor_Thread`、`Control_Thread`、`GUI_Thread` 与 `Cloud_Thread`，确保任务抢占的实时性。
  * **AI 协处理节点 (ESP32-S3)**：
      * 作为系统的“大脑视觉中枢”，专职处理高吞吐量的计算密集型任务。内部部署了轻量级卷积神经网络（CNN，如 MobileNet 变体）进行植物病虫害与缺水状态识别，同时运行语音识别模型实现自然语言指令交互。
      * **通信机制**：PSoC 6 与 ESP32-S3 之间通过高速 UART 结合 AT 指令集/SAL (Socket Abstraction Layer) 框架进行异步非阻塞通信。ESP32 仅在检测到状态跳变或接收到语音指令时，才通过中断唤醒 PSoC 6 主控，极大节省了系统总线带宽。

### 2\. 传感器矩阵与数据融合驱动 (`applications/` 模块)

系统底层利用 `IFX_PSOC6_HAL` 库结合 RT-Thread Device 框架，对多路传感器进行了标准化封装：

  * **I2C 传感器链**：
      * `SHT31.cpp`：高精度温湿度数据采集。
      * `BMP280.cpp`：大气压强与环境温度监测。
      * `BH1750.c`：光照强度动态监测，结合自动增益控制算法调整量程。
      * `SSD1306.cpp`：OLED 屏幕 I2C 驱动，通过 DMA 显存刷新避免阻塞 CPU。
  * **ADC 与 GPIO 控制模块**：
      * `Soil_moisture.c`：通过内部高精度 SAR ADC 采集土壤墒情，并加入卡尔曼滤波（Kalman Filter）滤除土壤盐分带来的毛刺噪声。
      * 执行器组：`water_pump.c` (水泵)、`ventilator.c` (通风风扇)、`Fill_light.c` (补光灯)、`RGB.cpp` (状态指示灯)。通过系统评估环境数据后输出 PWM 占空比进行无级调节。

### 3\. 数据-视觉双模态逻辑算法

系统在 `main.c` 的核心状态机中实现了交叉验证机制：

```text
IF (土壤湿度 > 阈值) AND (视觉模型输出 == "植株萎蔫") THEN
    -> 停止盲目灌溉，可能发生根系病害或缺氧，触发最高级预警并开启风扇排湿。
IF (土壤湿度 < 阈值) AND (视觉模型输出 == "植株健康") THEN
    -> 维持当前状态，推迟灌溉，节约水资源。
```

这种多源数据融合（Data Fusion）将系统的决策准确率提升了数个量级。

### 4\. 低功耗策略体系 (Ultra-Low Power Management)

  * 硬件级：充分利用 PSoC 6 的双核优势。在空闲状态下，关闭 Cortex-M4 主核，仅保留 Cortex-M0+ 运行，甚至进入 Deep Sleep 模式。
  * 软件级：引入 RT-Thread 的 `PM (Power Management)` 组件。基于 Tickless 技术，当就绪列表中无任务时，系统自动挂起 SysTick 定时器，并根据下一个软定时器的到期时间计算出睡眠深度。测试表明，系统在休眠态的电流低至 **8.5µA**。

### 5\. 云端接入与 UI 界面

  * **UI 交互**：通过 `applications/lvgl` 移植的轻量级图形库（LVGL），在本地屏幕绘制多仪表盘（Dashboards）、动态折线图（Charts）等。
  * **网络接入框架**：利用 RT-Thread 的 LwIP 协议栈及 `netdev` 组件，通过 ESP32 提供的 WiFi MAC 桥接上云，采用 MQTT 协议对接 OneNET 物联网平台，支持数据的下发与 OTA 升级（基于 `fal` 分区组件）。

-----

## 📂 项目文件结构

```text
├── GYLR
│   ├── applications/          # 应用层与外设驱动代码核心
│   │   ├── main.c             # 系统主入口与多线程初始化
│   │   ├── BH1750.* # 光照传感器驱动
│   │   ├── BMP280.* / SHT31.* # 环境温湿度及气压驱动
│   │   ├── Soil_moisture.* # 土壤湿度 ADC 驱动
│   │   ├── water_pump.* # 水泵控制与 PWM 驱动
│   │   ├── ventilator.* # 通风系统控制
│   │   ├── Fill_light.* # 补光灯控制
│   │   └── lvgl/              # LVGL GUI 图形界面应用
│   ├── board/                 # PSoC 6 板级配置与引脚初始化 (BSP)
│   ├── libraries/             # 依赖库与 HAL 层
│   │   ├── HAL_Drivers/       # RT-Thread 标准设备驱动层 (UART, I2C, SPI 等)
│   │   ├── IFX_PSOC6_HAL/     # Infineon PSoC 6 官方外设固件库 (PDL)
│   ├── rt-thread/             # RT-Thread 标准版操作系统源码
│   │   ├── components/        # RTT 组件 (DFS, SAL, LwIP, FinSH 等)
│   │   ├── libcpu/            # 架构底层汇编与调度 (ARM Cortex-M)
│   │   └── src/               # RTOS 内核 (线程, IPC, 定时器, 内存管理)
│   └── rtconfig.h             # RT-Thread 系统功能裁剪配置文件
└── 400994840a4946289f5fa89518b52589.pdf  # 项目完整报告文档
```

-----

## 🚀 编译与开发指南

### 1\. 开发环境要求

  * **IDE**: 推荐使用 RT-Thread Studio，或 MDK-ARM (Keil uVision 5) / IAR。
  * **编译器**: ARM GCC 或 ARMCC。
  * **硬件烧录工具**: J-Link, DAP-Link, 或是板载的 KitProg3 (Infineon 系列)。

### 2\. 构建步骤 (以 RT-Thread Studio 为例)

1.  将本项目克隆或下载到本地。
2.  打开 RT-Thread Studio，选择 `文件` -\> `导入` -\> `RT-Thread Studio 项目到工作空间`。
3.  选择本项目的 `GYLR` 目录。
4.  如需修改内核配置，双击打开 `RT-Thread Settings` 即可进行图形化裁剪（修改 `rtconfig.h`）。
5.  点击 **"Build (小锤子)"** 按钮编译工程。
6.  连接 PSoC 6 开发板，点击 **"Download (下载)"** 烧录固件。

-----

## 🔮 未来展望 (可扩展性)

在当前功能完善的原型基础上，项目预留了广阔的扩展空间：

1.  **云端训练与边缘更新**：利用云端强大算力训练更复杂的 CNN 模型，并将更新后的模型参数 OTA 推送至 ESP32-S3，实现边缘智能的持续进化。
2.  **预测式控制**：在 OneNET 云端通过历史数据机器学习，建立预测模型，根据未来几小时的天气预报提前决策最优灌溉量，实现“未雨绸缪”。
3.  **沉浸式数字孪生**：强化 3D 可视化平台的双向交互能力，允许用户在“虚拟环境”中直接下发调控策略。

-----

*版权所有 © 2024-2025 农业精英团队。本项目作为参赛作品开源交流，保留所有最终解释权。*
