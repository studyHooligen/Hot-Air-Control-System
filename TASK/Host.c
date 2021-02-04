/**
 * Host.c - The C source file of the host service thread
 * NOTE: This file is based on HAL library of stm32 platform
 *      该文件提供上位机命令解析服务线程
 *      - 命令解析执行线程： thread_recvHostService
 *      - 状态反馈线程：     thread_feedbackHostService
 *      用户使用的接口：
 *      - 初始化：  hostThreadInit      已设置为系统自动调用
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

#include "uart4Host.h"

#define __HOST_THREAD_DEBUG__   // 使能调试打印

/*********静态数据空间定义***********/

hostCommand recvCmdCache;  // 接收的DMA缓存
static rt_sem_t recvCmdSem = RT_NULL;  // 接收成功信号量

static struct rt_thread thread_recvHostHandle;
static char stack_recvHost[2048];

/**********线程定义***************/

/** @brief  接收上位机命令服务线程
 *  @note   
 *  @author 江榕煜（2021.2.3）
 *  @param 
 *  @retval
**/
void thread_recvHostService(void * param)
{
    uint32_t channel =0;
    uint32_t duty =0;
    uint32_t i =0;
    
    #ifdef __HOST_THREAD_DEBUG__
    char channelString[6];
    char dutyString[6];
    char cmd[2] = "0";
    #endif
    
    while(1)
    {
        // 等待成功接收信号量，由DMA接收中断释放
        rt_sem_take(recvCmdSem,RT_WAITING_FOREVER);
        
        // 帧头和帧尾安全检测
        if(recvCmdCache.head == 0xA9 && recvCmdCache.tail == 0xA6)
        {
            switch(recvCmdCache.command)  // 命令解析
            {
                case('M'):  // 更新电机输出占空比命令
                    channel = recvCmdCache.arg_1;
                    duty = recvCmdCache.arg_2;
                    motorSet(duty,channel);
                    break;
                
                case('R'):  // 更新加热丝占空比
                    channel = recvCmdCache.arg_1;
                    duty = recvCmdCache.arg_2;
                    heaterSet(duty,channel);
                    break;
                
                case('T'):  // 更新目标控制温度值
                    // TODO：添加目标温度改变命令
                    break;
                
                case('S'):  // 紧急停机命令！
                    for(i = 0;i < 8; i++)
                    {
                        heaterStop(i);  // 先关加热器
                        motorStop(i);   // 再关风机
                    }
                    break;
                    
                case('K'):  // 开始工作命令
                    for(i = 0; i<8; i++)
                    {
                        motorBegin(i);  // 先开风机
                        heaterBegin(i); // 再开加热器
                    }
                    break;
                    
                case('U'):  // 反馈最新状态值
                    break;
            }
            
            #ifdef __HOST_THREAD_DEBUG__
                cmd[0] = recvCmdCache.command;
                rt_kprintf("cmd:"); rt_kprintf(cmd);
            
                sprintf(dutyString, "%u",duty);
                rt_kprintf(",duty:"); rt_kprintf(dutyString);
            
                sprintf(channelString,"%u",channel);
                rt_kprintf(",channel from msg:"); rt_kprintf(channelString);
                rt_kprintf("\n");
                
            #endif
        }
        else{
            #ifdef __HOST_THREAD_DEBUG__
            rt_kprintf("msg from host with head and tail error!");
            #endif
        }
        
        // 清空数据
        recvCmdCache.arg_1 = 0;
        recvCmdCache.arg_2 = 0;
        recvCmdCache.command = 0;
        recvCmdCache.head = 0;
        recvCmdCache.tail = 0;
        
        // 重新启动接收
        HAL_UART_Receive_DMA(&HostUartHandler, (uint8_t *) &recvCmdCache, sizeof(hostCommand) );
    }
}

/** @brief  状态反馈线程
 *  @note   
 *          暂时不实现该功能
 *  @author 江榕煜（2021.2.3）
 *  @param  None
 *  @retval None
**/
void thread_feedbackHostService(void)
{
    while(1)
    {
        rt_thread_suspend(rt_thread_self());
    }
}

/**********用户接口定义************/

/** @brief  初始化命令接收服务线程
 *  @note   
 *          该命令将在系统内核初始化完成后被自动调用
 *  @author 江榕煜（2021.2.1）
 *  @param  None
 *  @retval None
**/
void hostThreadInit(void)
{
    // 初始化底层串口接收命令BSP接口，获取接收成功通知信号量
    recvCmdSem = uart4HostInit(&recvCmdCache);
    
    // 初始化状态线程
    rt_thread_init(&thread_recvHostHandle,      // 线程句柄
                    "host Service",       //线程名称
                    thread_recvHostService,  //线程入口
                    RT_NULL,            //线程参数
                    &stack_recvHost[0],       // 线程使用堆栈
                    sizeof(stack_recvHost),   // 堆栈大小
                    10,     // 线程优先级
                    10);    // 线程时间切片量
                    
    // 启动服务！
    rt_thread_startup(&thread_recvHostHandle);
    
}

INIT_APP_EXPORT(hostThreadInit);    // 该接口将被系统自动调用


