/**
 * uart4Host.h - The C head file of the uart Driver for host
 * NOTE: This file is based on HAL library of stm32 platform
 *       该文件提供和上位机通信时使用的帧结构：
 *          - 上位机命令帧：   hostCommand
 *          - 本地状态帧：     localMsg
 *       用户接口：
 *          - 通信初始化：    uart4HostInit
 *      
 *
 * Copyright (c) 2020-, Wawo hair dryer Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-02-03     江榕煜       first version   2373180028@qq.com
**/

#include "BSP.h"

#define HostUartHandler huart4

/***********通信结构定义***********/

/** @brief  上位机命令帧结构
 *  @note   帧头和帧尾是固定的
 *          命令和参数见README文档说明
 *  @author 江榕煜（2021.2.3）
**/
typedef struct{
    uint8_t head; // 帧头（固定为0xA9：1010 1001）
    unsigned char command;   // 命令符号
    uint16_t arg_1;     // 参数1
    uint16_t arg_2;     // 参数2
    uint8_t reserve;    // 保留
    uint8_t tail; // 帧尾（固定为0xA6：1010 0110）
}hostCommand;

/** @brief  本地状态反馈帧结构
 *  @note   帧头和帧尾是固定的
 *          命令和参数见README文档说明
 *  @author 江榕煜（2021.2.3）
**/
typedef struct{
    uint8_t head; // 帧头（固定为0xA9：1010 1001）
    uint8_t status;     // 状态码
    uint16_t duty[16];  // 各通道占空比
    uint16_t temp[8];   // 各通道温度
    uint8_t tail; // 帧尾（固定为0xA6：1010 0110）
}localMsg;

/**************接口声明**************/

/** @brief  上位机通信初始化接口
 *  @note   
 *          使用上位机的功能前必须调用该接口
 *          该接口提供的是接收服务初始化！
 *          发送数据采用主动式接口
 *  @author 江榕煜（2021.2.3）
 *  @param 
 *          recvCachePtr(hostCommand *)     接收数据帧缓存块指针
 *  @retval 
 *          rt_sem_t    成功接收信号量控制器
**/
rt_sem_t uart4HostInit(hostCommand * recvCachePtr);

/** @brief  发送状态给上位机接口
 *  @note   
 *          使用上位机的功能前必须调用该接口
 *  @author 江榕煜（2021.2.3）
 *  @param 
 *          sendPtr(localMsg *)     发送数据信息块指针
 *  @retval 
 *          rt_sem_t    成功发送信号量控制器
**/
rt_sem_t uart4Host_SendStatus(localMsg * sendPtr);
