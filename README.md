# 热风控制系统

这是一个热风控制系统嵌入式项目，基于STM32F1芯片和RT—Thread实时系统开发温度和风速控制。

> LICENSE：Apache 2.0

## 硬件说明

- 主控芯片：方案中采用的STM32F103VET6，512k空间FLASH，64k空间SRAM
- 温度采样：使用MAX6675+K型热电偶方案，MCU使用SPI读取温度
- 电热丝驱动：采用UCC27211+NMOS方案，MCU使用70kHz的PWM控制发热功率
- 风机驱动：MCU出PWM控制全桥来间接控制电机
- 电源：功率部分使用整流桥，信号部分上小功率开关DC-DC

## 软件说明

项目文件组织结构

> 这是一个使用CubeMX配置MCU，使用Keil开发程序的项目

- 项目根
  - Algorithm：算法文件夹
    - PID.c
    - PID.h
  - BSP：板级支持包
    - BSP.h
    - BSPinit.c
    - PWMdevice.c
    - SPIdevice.c
    - uartFinshCharIO.c
    - uartFinshCharIO.h
  - Core：STM32单片机HAL库
  - Drives：基本的Arm驱动文件
  - MDK-ARM：Keil工程文件目录
  - RT-Thread：实时操作系统源文件
  - TASK：用户任务文件夹
    - PWMctrl.c
    - systemHealth.c
    - tempCtrl.c
    - UI.c
    - windSpeed.c
  - dryerV1.ioc：CubeMX工程文件
  - LICENSE：权限文件
  - README.md：项目说明文件

重要的文件分别在`Algorithm`、`BSP`和`TASK`三个文件夹中，用途不做赘述
