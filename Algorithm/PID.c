/* 
 * PID.c - The C source file of PID controller
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
#include "PID.h"

T_PID PID_updateDelta(PID * pider, T_PID status){
    T_PID errNow = pider->target_ - status;  //目前误差值

    T_PID delta =   //输出增量
        pider->K_p * (errNow - pider->error_Pre) +    //比例P
        pider->K_i * errNow +                  //积分I
        pider->K_d * (errNow - 2*pider->error_Pre + pider->error_PPre);  //微分D

    if(delta>pider->max_) delta = pider->max_;  //输出限幅
    else if(delta<pider->min_) delta = pider->min_;

    pider->error_PPre = pider->error_Pre; //误差记忆
    pider->error_Pre = errNow;

    return delta;  //返回结果
}

#ifdef PID_use_integral

T_PID PID_updateStatus(PID * pider, T_PID status){

    pider->output_pre += PID_updateDelta(pider,status);
    
    if(pider->output_pre > pider->max_) 
        pider->output_pre = pider->max_;  //输出限幅
    else if(pider->output_pre < pider->min_) 
        pider->output_pre = pider->min_;

    return pider->output_pre;
}

#endif
