#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "spi.h"
#include "dma.h"

#include <stdio.h>

#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

/*  @brief  启动指定通道的加热功能
 *  @note   通道号从0开始
 *          我太懒了，目前电机和加热丝都是该接口
 *  @author 江榕煜（2021.1.27）
 *  @param 
 *          channel（uint32_t）   通道号
 *  @retval
 *          None
**/
void heaterBegin(uint32_t channel);

/*  @brief  停止指定通道的加热功能
 *  @note   通道号从0开始
 *  @author 江榕煜（2021.1.27）
 *  @param 
 *          channel（uint32_t）   加热通道号
 *  @retval
 *          None
**/
void heaterStop(uint32_t channel);

/*  @brief  加热器占空比设置函数
 *  @note   通道号从0开始
 *  @author 江榕煜（2021.1.27）
 *  @param 
 *          duty（uint32_t）     占空比，0~1000 <-> 0%~100%
 *          channel（uint32_t）  通道号
 *  @retval
 *          None
**/
void heaterSet(uint32_t duty,uint32_t channel);

/*  @brief  热电偶通道选择
 *  @note   编号从0开始
 *  @author 江榕煜（2021.1.27）
 *  @param 
 *          channel（uint32_t）   通道号
 *  @retval
 *          None
**/
void ThermocoupleSelect(uint32_t channel);

