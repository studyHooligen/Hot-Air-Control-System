/**
 * PID.hpp - The C head file of PID controller
 * NOTE: This file is based on C99
 *
 * Copyright (c) 2020-, FOSH Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-01-26     StudyHooligen     first version   2373180028@qq.com
 */

/************ 用户参数控制 ***********/
#define T_PID float //PID计算数值类型

#define PID_use_integral    //是否使用位置PID

typedef struct {
    
/************ 内部参数 *************/
    T_PID K_p;  //比例增益
    T_PID K_i;  //积分增益
    T_PID K_d;  //微分增益

    T_PID target_;   //输出目标值

    T_PID error_Pre;    //上一次误差
    T_PID error_PPre;   //上上次误差

#ifdef PID_use_integral
    T_PID output_pre;   //上次输出值
#endif

    T_PID max_;  //PID输出最大值
    T_PID min_;  //PID输出最小值
} PID;

/**
 * @brief   增量PID计算接口
 * @note    暂时不支持微分滤波器
 * @param
 *          pider(PID *)    PID参数句柄接口
 *          status(T_PID)   反馈状态值
 * @retval
 *          计算增量结果
**/
T_PID PID_updateDelta(PID * pider,T_PID status);

/**
 * @brief   位置PID计算接口
 * @note    该函数本质基于上增量PID接口
 * @param
 *          pider(PID *)    PID参数句柄接口
 *          status(T_PID)   反馈状态值
 * @retval
 *          计算状态结果
**/
T_PID PID_updateStatus(PID * pider, T_PID status);
