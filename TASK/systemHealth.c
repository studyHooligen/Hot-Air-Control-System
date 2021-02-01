/* 
 * systemHealth.c - The C source file of the system status thread
 * NOTE: This file is based on HAL library of stm32 platform
 *       该文件提供系统正常运行状态指示线程，分别有两大线程：
 *          - 系统状态显示线程：thread_systemStatus
 *          - 系统super卫士线程：thread_systemDefender
 *       提供用户启动线程的接口：
 *          - 线程初始化启动：systemThreadInit()
 *                              目前该接口会在系统初始化阶段被自动调用
 *          - 关闭状态指示灯：stopStatusLED()
 *          - 恢复状态指示灯：continueStatusLED()
 *      
 *
 * Copyright (c) 2020-, Wawo hair dryer Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-02-01     江榕煜       first version   2373180028@qq.com
**/

#include "BSP.h"

/************信号量定义**************/
static struct rt_semaphore status_sem;

/************静态线程定义**************/
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread thread_system;
static char stack_system[128];

static struct rt_thread thread_defender;
static char stack_defender[128];

/*  @brief  系统运行状态检测
 *  @note   
 *          在该线程中，将以1Hz的频率变换系统状态指示灯
 *  @author 江榕煜（2021.2.1）
 *  @param  None
 *  @retval None
**/
static void thread_systemStatus(void *param)
{
    while(1)
    {
        rt_thread_mdelay(1000);
        HAL_GPIO_TogglePin(LED_RUN_GPIO_Port,LED_RUN_Pin);
        
        if(rt_sem_trytake(&status_sem) == RT_EOK)
        {
            rt_thread_suspend(rt_thread_self());
            rt_schedule();
        }
    }
}

/*  @brief  系统错误发生报警提示线程
 *  @note   
 *          在该线程中，将检查整个设备的可能发生错误、危险的物理量
 *          如果检测到危险、错误，将会向用户报警
 *          ---该线程目前暂时每开发---
 *  @author 江榕煜（2021.2.1）
 *  @param  None
 *  @retval None
**/
static void thread_systemDefender(void *param)
{
    // 暂时先不开发该部分程序
    rt_thread_resume(rt_thread_self()); //直接挂起程序
    rt_schedule();
}


/***********用户服务使用接口************/

/*  @brief  系统状态线程初始化启动接口
 *  @note
 *  @author 江榕煜（2021.2.1）
 *  @param 
 *  @retval
**/
void systemThreadInit(void)
{
    // 初始化控制信号量
    rt_sem_init(&status_sem, "status", 0, RT_IPC_FLAG_FIFO);
    
    // 初始化状态线程
    rt_thread_init(&thread_system,      // 线程句柄
                    "systemstatus",       //线程名称
                    thread_systemStatus,  //线程入口
                    RT_NULL,            //线程参数
                    &stack_system[0],       // 线程使用堆栈
                    sizeof(stack_system),   // 堆栈大小
                    10,     // 线程优先级
                    10);    // 线程时间切片量
                    
    // 初始化super护卫线程
                    // 哈哈哈哈哈哈哈哈哈这个线程好像很牛逼（但其实现在啥用没有）
    rt_thread_init(&thread_defender,      // 线程句柄
                    "defender",       //线程名称
                    thread_systemDefender,  //线程入口
                    RT_NULL,            //线程参数
                    &stack_defender[0],       // 线程使用堆栈
                    sizeof(stack_defender),   // 堆栈大小
                    10,     // 线程优先级
                    10);    // 线程时间切片量
    // 启动线程
    rt_thread_startup(&thread_system);
    // 启动线程
    rt_thread_startup(&thread_defender);
}

INIT_APP_EXPORT(systemThreadInit);  // 系统初始化的时候自动调用

/*  @brief  停止系统状态LED
 *  @note
 *  @author 江榕煜（2021.2.1）
 *  @param  None
 *  @retval None
**/
void stopStatusLED(void)
{
    rt_sem_release(&status_sem); // 释放线程挂起信号量
    HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_RESET);  // 熄灭LED
}

MSH_CMD_EXPORT(stopStatusLED,stop system LED);

/*  @brief  恢复系统状态LED
 *  @note 
 *  @author 江榕煜（2021.2.1）
 *  @param  None
 *  @retval None
**/
void continueStatusLED(void)
{
    rt_thread_resume(&thread_system);   // 恢复线程
}

MSH_CMD_EXPORT(continueStatusLED, resume system LED);
