# 热风控制系统

这是一个热风控制系统嵌入式项目，基于STM32F1芯片和RT—Thread实时系统开发温度和风速控制。

> LICENSE：Apache 2.0

## 硬件说明

- 主控芯片：方案中采用的STM32F103VET6，512k空间FLASH，64k空间SRAM
- 温度采样：使用MAX6675+K型热电偶方案，MCU使用SPI读取温度
- 电热丝驱动：采用UCC27211+NMOS方案，MCU使用70kHz的PWM控制发热功率
- 风机驱动：MCU出PWM控制全桥来间接控制电机
- 电源：功率部分使用整流桥，信号部分上小功率开关DC-DC

### PWM引脚

- 风机

  风机通道号 | 定时器实例 | 定时器通道 | MCU引脚
  :---: | :--- | :---: | ---:
  0 | htim1 | 1 | E9
  1 | htim1 | 2 | E11
  2 | htim1 | 3 | E13
  3 | htim1 | 4 | E14
  4 | htim2 | 1 | A15
  5 | htim2 | 2 | B3
  6 | htim2 | 3 | B10
  7 | htim2 | 4 | B11

- 加热丝

  加热通道号 | 定时器实例 | 定时器通道 | MCU引脚
  :---: | :--- | :---: | ---:
  0 | htim3 | 1 | B4
  1 | htim3 | 2 | B5
  2 | htim3 | 3 | B0
  3 | htim5 | 1 | A0
  4 | htim4 | 1 | D12
  5 | htim4 | 2 | D13
  6 | htim4 | 3 | D14
  7 | htim4 | 4 | D15

### UART通信

通信用途 | 串口实例 | RX引脚 | TX引脚 | 说明
:---: | :--- | :--- | :--- | ---:
FinSH控制台 | USART3 | D9 | D8 | 见`./BSP/uartFinshCharIO.c`
上位机通信接口 | UART4 | C11 | C10 | 见`./BSP/uart4Host.c`

## 软件说明

### 项目文件组织结构

> 这是一个使用CubeMX配置MCU，使用Keil开发程序的项目

- 项目根
  - Algorithm：算法文件夹
    - PID.c
    - PID.h
  - BSP：板级支持包
    - BSP.h（BSP顶层接口文件）
    - BSPinit.c（BSP初始化定义）
    - PWMdevice.c
    - SPIdevice.c
    - uart4Host.c（服务上位机的uart）
    - uart4Host.h
    - uartFinshCharIO.c（服务FinSH的uart）
    - uartFinshCharIO.h
  - Core：STM32单片机HAL库
  - Drives：基本的Arm驱动文件
  - MDK-ARM：Keil工程文件目录
  - RT-Thread：实时操作系统源文件
  - TASK：用户任务文件夹
    - Host.c（上位机线程）
    - PWMctrl.c
    - systemHealth.c（系统维护线程）
    - tempCtrl.c（温度控制线程）
    - UI.c（用户控制台服务）
    - windSpeed.c（风速控制线程）
  - HostUI-Qt4Python：上位机程序，使用Qt框架和Python编写
    - dryerV1.py（ui文件生成的界面python文件）
    - dryerV1.ui（Qt设计ui的源文件）
    - main.py（上位机逻辑）
    - sendCommand.py（串口发命令模块）
  - dryerV1.ioc：CubeMX工程文件
  - LICENSE：权限文件
  - README.md：项目说明文件

重要的文件分别在`Algorithm`、`BSP`和`TASK`三个文件夹中，用途不做赘述

### 上位机通信

本项目采用的通信帧结构为：

- 上位机命令帧（方向：上位机->本地）
     序号 | 变量名 | 数据类型 | 备注 |
     :---: | --- | --- | ---
     1 | head | uint8_t | 数据帧头，固定为0xA9：1010 1001
     2 | command | unsigned char | 命令
     3 | arg_1 | uint16_t | 参数1，长度16bit
     4 | arg_2 | uint16_t | 参数2，长度16bit
     5 | tail | uint8_t | 数据帧尾，固定为0xA6：1010 0110

    帧结构实现为：

    ```C
    typedef struct{
    uint8_t head; // 帧头（固定为0xA9：1010 1001）
    unsigned char command;   // 命令符号
    uint16_t arg_1;     // 参数1
    uint16_t arg_2;     // 参数2
    uint8_t tail; // 帧尾（固定为0xA6：1010 0110）
    }hostCommand;
     ```

- 本地状态帧（方向：本地->上位机）

    序号 | 变量名 | 数据类型 | 备注 |
    :---: | --- | --- | ---
    1 | head | uint8_t | 数据帧头，固定为0xA9：1010 1001
    2 | status | uint8_t | 状态码
    3 | duty  | uint16_t [16] | 8个电机通道+8个温度加热通道占空比数值
    4 | temp | uint16_t [8] | 8个温度检测通道数据
    5 | tail | uint8_t | 数据帧尾，固定为0xA6：1010 0110

    ```C
    typedef struct{
    uint8_t head; // 帧头（固定为0xA9：1010 1001）
    uint8_t status;     // 状态码
    uint16_t duty[16];  // 各通道占空比
    uint16_t temp[8];   // 各通道温度
    uint8_t tail; // 帧尾（固定为0xA6：1010 0110）
    }localMsg;
    ```

#### 上位机命令

本项目支持的上位机命令有：

命令 | ASCII码 | 参数1 | 参数2 | 备注
:---: | :--- | ---: | ---: | :---:
更新电机PWM占空比 | M | 通道号 | 占空比 | 占空比数值范围0~1000，对应0.0%~100.0%
更新加热丝PWM占空比 | R | 通道号 | 占空比 | 占空比数值范围0~1000，对应0.0%~100.0%
更新目标温度值 | T | 通道号 | 温度值（°C） | 目标温度值支持精度为1°C
紧急停止 | S | None | None | 发送该命令将停止设备所有的功率输出
开始工作 | K | None | None | 发送该命令后，建议发送各个通道的PWM设置命令
刷新状态值 | U | None | None | 发送该命令后设备将立即发送所有的最新状态值回来

#### 上位机运行

- 本上位机使用Python编写，所以可使用pip3直接安装依赖包：

  ```Python
  pip install PyQt5
  pip install pyqt5-tools
  ```

- UI界面设计文件`dryerV1.ui`可使用Qt Designer打开
- 设计好的文件转换为py文件

  ```Python
  pyuic5 dryerV1.ui -o dryerV1.py
  ```

- 在`main.py`中可按需要修改逻辑
- 直接跑`python3 main.py`就可了
