#include "BSP.h"

void heaterBegin(uint32_t channel)
{
    switch(channel)
    {
        case(0):
            HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
            break;
        case(1):
            HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
            break;
        case(2):
            HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
            break;
        case(3):
            HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
            break;
        
        case(4):
            HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
            break;
        case(5):
            HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
            break;
        case(6):
            HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
            break;
        case(7):
            HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
            break;
        
        case(8):
            HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
            break;
        case(9):
            HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
            break;
        case(10):
            HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
            break;
        case(11):
//            HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
            HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);    // 该版本开发板B1引脚被按键占用
            break;
        
        case(12):
            HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
            break;
        case(13):
            HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
            break;
        case(14):
            HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
            break;
        case(15):
            HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
            break;
        default:
            break;
    }
    
}

void heaterStop(uint32_t channel)
{
    switch(channel)
    {
        case(0):
            HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
            break;
        case(1):
            HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
            break;
        case(2):
            HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
            break;
        case(3):
            HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
            break;
        
        case(4):
            HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
            break;
        case(5):
            HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
            break;
        case(6):
            HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);
            break;
        case(7):
            HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);
            break;
        
        case(8):
            HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
            break;
        case(9):
            HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
            break;
        case(10):
            HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3);
            break;
        case(11):
//            HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
            HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);    // 该版本开发板B1引脚被按键占用
            break;
        
        case(12):
            HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
            break;
        case(13):
            HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
            break;
        case(14):
            HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
            break;
        case(15):
            HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_4);
            break;
        
        default:
            break;
    }
}

void heaterSet(uint32_t duty,uint32_t channel)
{
    switch(channel)
    {
        case(0):
            htim1.Instance->CCR1 = duty;
            break;
        case(1):
            htim1.Instance->CCR2 = duty;
            break;
        case(2):
            htim1.Instance->CCR3 = duty;
            break;
        case(3):
            htim1.Instance->CCR4 = duty;
            break;
        case(4):
            htim2.Instance->CCR1 = duty;
            break;
        case(5):
            htim2.Instance->CCR2 = duty;
            break;
        case(6):
            htim2.Instance->CCR3 = duty;
            break;
        case(7):
            htim2.Instance->CCR4 = duty;
            break;
        case(8):
            htim3.Instance->CCR1 = duty;
            break;
        case(9):
            htim3.Instance->CCR2 = duty;
            break;
        case(10):
            htim3.Instance->CCR3 = duty;
            break;
        case(11):
            //htim3.Instance->CCR4 = duty;
            htim5.Instance->CCR1 = duty;
            break;
        case(12):
            htim4.Instance->CCR1 = duty;
            break;
        case(13):
            htim4.Instance->CCR2 = duty;
            break;
        case(14):
            htim4.Instance->CCR3 = duty;
            break;
        case(15):
            htim4.Instance->CCR4 = duty;
            break;
        default:
            break;
    }
}
