/**
 * tempCtrl.c - The C source file of the temperature control thread
 * NOTE: This file is based on HAL library of stm32 platform
 *       该文件提供温度闭环控制服务线程，分别有两大线程：
 *          - 温度检测线程：thread_tempDetect
 *          - 温度控制线程：thread_tempCtrl
 *       提供用户启动线程的接口：
 *          - 线程初始化启动：Temp_thread_init()
 *      
 *      闭环控制逻辑为：
 *          SPI总线读取热电偶传感器数据
 *                      ↓
 *          读取MCU主控内部温度ADC数据
 *                      ↓
 *                判断是否有过温
 *                      ↓
 *                 PID控制器运算
 *                      ↓
 *               更新加热丝PWM占空比
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

#include "PID.h"

#define TEMP_CTRLFREQ_EQUAL_SAMPFREQ    //定义该控制量，则温度控制器的运算率和采样率相同
#ifndef TEMP_CTRLFREQ_EQUAL_SAMPFREQ
    #define TEMP_CTRLFREQ 10    // 该控制量调整控制器运算率，单位：ms
#endif

#define ThermocoupleSampCycle 500   // 热电偶采样周期，单位：ms

#define TEMP_CHANNEL_COUNT 8    // 温度检测通道，不包含内部温度检测通道

#define TEMP_OVERHEAT 50        // 过温阈值，量纲：摄氏度

/************温度数据空间**************/
uint16_t tempCacheDMA[TEMP_CHANNEL_COUNT +1];  // 数据DMA存储空间
float tempData[TEMP_CHANNEL_COUNT +1];     // 温度数据，译码转换后

PID tempPIDer[TEMP_CHANNEL_COUNT];      // PID控制器属性控制量

/************静态线程定义**************/
static struct rt_thread threadProperty_tempDetect;  // 温度(检测）线程属性变量
static char stack_tempDetect[512];      // 温度（检测）线程栈

static struct rt_thread threadProperty_tempCtrl;  // 温度（控制）线程属性变量
static char stack_tempCtrl[512];      // 温度（控制）线程栈

/** @brief  温度数据采集服务线程
 *  @note   调用频率同采样率
 *          这是一个持续运行线程！
 *  @author 江榕煜（2021.1.25）
 *  @param  
 *          None
 *  @retval 
 *          None
**/
static void thread_tempDetect(void *param)
{
    while(1)    //循环线程
    {
        //各个风机温度通道
        for(uint32_t i = 0; i < TEMP_CHANNEL_COUNT-1; i++)
        {
            // 更换SPI片选码GPIO
            ThermocoupleSelect(i);
            
            // 从SPI总线读取数据
            HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);
            HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)&tempCacheDMA[i],1);
            
            // 交出CPU，等待读取完成
            rt_thread_mdelay(ThermocoupleSampCycle/TEMP_CHANNEL_COUNT);
            
            // 数据译码转换
            tempData[i] = 0.25 * (tempCacheDMA[i] >> 3);
        }
        
        //芯片工作温度
        tempData[TEMP_CHANNEL_COUNT-1] = (1.43 - ( (float)tempCacheDMA[TEMP_CHANNEL_COUNT-1] /4096*3.3) )/0.0043 + 25;
        HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&tempCacheDMA[TEMP_CHANNEL_COUNT],1);
        
        #ifdef TEMP_CTRLFREQ_EQUAL_SAMPFREQ
            //启动温度控制线程
            rt_thread_resume(&threadProperty_tempCtrl);
        #endif
    }
}

/** @brief  温度控制/保护服务线程
 *  @note   暂时不对芯片过温进行解决！！！
 *          这是一个持续运行线程！
 *  @author 江榕煜（2021.1.25）
 *  @param 
 *          None
 *  @retval
 *          None
**/

static void thread_tempCtrl(void *param)
{
    while(1)
    {
        //对各个通道进行控制运算和控制量输出
        for(uint32_t i = 0; i < TEMP_CHANNEL_COUNT-1; i++)
        {
            // 首先判断是否过温
            if(tempData[i] >= TEMP_OVERHEAT)
            {
                heaterStop(i);  //过温停止加热
                //TODO：添加恢复加热功能
            }
            else
            {
                float res = PID_updateStatus(tempPIDer + i ,tempData[i]);   //PID计算
                heaterSet(res,i);   //更新加热器控制量
            }
        }
        
        #ifdef TEMP_CTRLFREQ_EQUAL_SAMPFREQ
            // 挂起本线程
            rt_thread_suspend(rt_thread_self());
            rt_schedule();
        #endif
    }
}


/***********用户服务使用接口************/

/** @brief  温度工作初始化
 *  @note   在此之前务必先初始化好外设
 *  @author 江榕煜（2021.1.25）
 *  @param  None
 *  @retval None
**/
void Temp_thread_init(void)
{
    // 设置PID参数
    // 目前写法先假设每个通道的控制模型是一样的
    for(uint32_t i = 0; i < TEMP_CHANNEL_COUNT-1; i++)
    {
        tempPIDer[i].error_PPre = 0;
        tempPIDer[i].error_Pre = 0;
        
        tempPIDer[i].K_d = 0;
        tempPIDer[i].K_i = 0.0003;
        tempPIDer[i].K_p = 0.001;
        
        tempPIDer[i].max_ = 1000;       // 定时器的计数周期1000，即最大
        tempPIDer[i].min_ = 0;          // 最小值0
        
        tempPIDer[i].output_pre = 500;
        tempPIDer[i].target_ = 40;      // 目标加热温度，40摄氏度
    }
    
    
    //使用ADC之前进行自校准
    HAL_ADCEx_Calibration_Start(&hadc1);
    
    // 初始化温度采样线程
    rt_thread_init(&threadProperty_tempDetect,      // 线程句柄
                    "tempDetect",       //线程名称
                    thread_tempDetect,  //线程入口
                    RT_NULL,            //线程参数
                    &stack_tempDetect[0],       // 线程使用堆栈
                    sizeof(stack_tempDetect),   // 堆栈大小
                    10,     // 线程优先级
                    10);    // 线程时间切片量
                    
    // 初始化温度控制&保护线程
    rt_thread_init(&threadProperty_tempCtrl,      // 线程句柄
                    "tempCtrl",       //线程名称
                    thread_tempCtrl,  //线程入口
                    RT_NULL,            //线程参数
                    &stack_tempCtrl[0],       // 线程使用堆栈
                    sizeof(stack_tempCtrl),   // 堆栈大小
                    10,     // 线程优先级
                    10);    // 线程时间切片量
    
    // 启动温度检测线程
    rt_thread_startup(&threadProperty_tempDetect);
                    
    // 启动线程
    rt_thread_startup(&threadProperty_tempCtrl);
    #ifdef TEMP_CTRLFREQ_EQUAL_SAMPFREQ
    // 挂起线程
    rt_thread_suspend(&threadProperty_tempCtrl);
    #endif
    
    // 启动输出
    for(int i = 0; i < 16;i++)
    {
        heaterBegin(i);
    }
}

/** @brief  ADC转换完成回调函数
 *  @note   注意这是总的回调函数，需要检查传入的句柄是否为你想要的ADC
 *  @author 江榕煜（2021.1.25）
 *  @param  
 *          hadc    ADC设备句柄
 *  @retval
 *          None
**/
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//    if(hadc == &hadc1)
//    {
//        
//    }
//}

/** @brief  SPI使用DMA传输
 *  @note   
 *          在该回调函数中添加了500次运算的延时
 *  @author 江榕煜（2021.1.27）
 *  @param 
 *  @retval
**/
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi == &hspi1)
    {
        for(int i = 0;i<500; i++)
        {}
        HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_SET);
    }
}
