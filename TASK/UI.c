/* 
 * UI.c - The C source file of the User Interface
 * NOTE: This file is based on HAL library of stm32 platform
 *       该文件提供用户和系统交互（FinSH控制台）的命令定义
 *          - 启动所有的PWM输出：beginAllPWM
 *          - 停止所有的PWM输出：stopAllPWM
 *          - 设置PWM占空比：PWM_set
 *      
 *
 * Copyright (c) 2020-, Wawo hair dryer Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-01-31     江榕煜       first version   2373180028@qq.com
**/

#include "BSP.h"

/*  @brief  启动所有PWM接口（控制台命令）
 *  @note
 *  @author 江榕煜（2020.9.23）
 *  @param  无
 *  @retval 无
**/
int beginAllPWM(int argc,char** argv)
{
    // 启动输出
    for(int i = 0; i < 16;i++)
    {
        heaterBegin(i);
    }
    rt_kprintf("all PWM channels are enabled\n");
    return 0;
}

MSH_CMD_EXPORT(beginAllPWM,begin PWM output);

/*  @brief  停止所有PWM接口（控制台命令）
 *  @note
 *  @author 江榕煜（2021.1.25）
 *  @param  无
 *  @retval 无
**/

int stopAllPWM(int argc,char ** argv)
{
    for(int i =0; i < 16; i++)
    {
        heaterStop(i);
    }
    rt_kprintf("all PWM channels are disabled\n");
    return 0;
}

MSH_CMD_EXPORT(stopAllPWM,stop PWM output);

/*  @brief  设置PWM占空比接口（控制台命令）
 *  @note   占空比范围0~1000 <-> 0%~100%
 *  @author 江榕煜（2021.1.25）
 *  @param  
 *          arg[1]：占空比
 *          arg[2]：通道号
 *  @retval 无
**/
int PWM_set(int argc,char ** argv)
{
    if(argc < 3)
    {
        rt_kprintf("please input the duty param and channelNum!!!\n");
        return 0;
    }
    else{
        int duty,channel;
        sscanf(argv[1],"%d",&duty);
        sscanf(argv[2],"%d",&channel);
        switch(channel)
        {
            case(1):
                htim1.Instance->CCR1 = duty;
                break;
            case(2):
                htim1.Instance->CCR2 = duty;
                break;
            case(3):
                htim1.Instance->CCR3 = duty;
                break;
            case(4):
                htim1.Instance->CCR4 = duty;
                break;
            case(5):
                htim2.Instance->CCR1 = duty;
                break;
            case(6):
                htim2.Instance->CCR2 = duty;
                break;
            case(7):
                htim2.Instance->CCR3 = duty;
                break;
            case(8):
                htim2.Instance->CCR4 = duty;
                break;
            case(9):
                htim3.Instance->CCR1 = duty;
                break;
            case(10):
                htim3.Instance->CCR2 = duty;
                break;
            case(11):
                htim3.Instance->CCR3 = duty;
                break;
            case(12):
                htim3.Instance->CCR4 = duty;
                break;
            case(13):
                htim4.Instance->CCR1 = duty;
                break;
            case(14):
                htim4.Instance->CCR2 = duty;
                break;
            case(15):
                htim4.Instance->CCR3 = duty;
                break;
            case(16):
                htim4.Instance->CCR4 = duty;
                break;
            default:
                rt_kprintf("error param\n");
        }
        rt_kprintf("set success\n");
        return 0;
    }
}

MSH_CMD_EXPORT(PWM_set,set PWM duty);
