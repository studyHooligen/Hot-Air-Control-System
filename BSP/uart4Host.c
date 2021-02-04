/**
 * uart4Host.c - The C source file of the uart Driver for host
 * NOTE: This file is based on HAL library of stm32 platform
 *       
 * Copyright (c) 2020-, Wawo hair dryer Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-02-02     江榕煜       first version   2373180028@qq.com
**/

#include "uart4Host.h"

/***********通信收发缓存定义**********/

struct rt_semaphore recv_sem;    // 成功接收一帧数据信号量

/*************函数定义*************/

/** @brief  上位机通信初始化接口
 *  @note   
 *          使用上位机的功能前必须调用该接口
 *  @author 江榕煜（2021.2.1）
 *  @param 
 *          None
 *  @retval 
 *          rt_sem_t    成功接收信号量控制器
**/
rt_sem_t uart4HostInit(hostCommand * recvCachePtr)
{
    // 初始化接收信号量
    rt_sem_init(&recv_sem, "host Recv", 0, RT_IPC_FLAG_FIFO);

//    char str[8];
//    sprintf(str,"%d",sizeof(hostCommand));
//    rt_kprintf("size of hostCommand:");
//    rt_kprintf(str);
//    rt_kprintf("\n");
    
    // 启动DMA串口数据接收
    HAL_UART_Receive_DMA(&HostUartHandler,(uint8_t *) recvCachePtr, sizeof(hostCommand));
    
    // 返回接收控制量句柄
    return &recv_sem;
}


/** @brief  串口接收完毕回调函数
 *  @note   
 *          启动串口中断接收或者DMA接收
 *  @author 江榕煜（2021.2.1）
 *  @param 
 *          huart   uart句柄
 *  @retval None
**/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //rt_interrupt_enter();
    
    if(huart == &HostUartHandler)
    {
        // 接收到一帧了
        //rt_kprintf("get a msg frame\n");
        
        // 由用户处理命令时启动
//        // 重新启动接收
//        HAL_UART_Receive_DMA(&HostUartHandler, &recvCache, sizeof(recvCache));
        
        // 发出成功接收信号量
        rt_sem_release(&recv_sem);
    }
    
    //rt_interrupt_leave();
}
