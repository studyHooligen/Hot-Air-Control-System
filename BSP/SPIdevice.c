#include "BSP.h"

/*  @brief  热电偶通道选择
 *  @note   编号从0开始
 *          目前实现8个通道
 *  @author 江榕煜（2021.1.27）
 *  @param 
 *          channel（uint32_t）   通道号
 *  @retval
 *          None
**/
void ThermocoupleSelect(uint32_t channel)
{
    switch(channel)
    {
        case(0):    // 000
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_RESET);
            break;
        case(1):    // 001
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_SET);
            break;
        case(2):    // 010
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_RESET);
            break;
        case(3):    // 011
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_SET);
            break;
        case(4):    // 100
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_RESET);
            break;
        case(5):    // 101
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_SET);
            break;
        case(6):    // 110
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_RESET);
            break;
        case(7):    // 111
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_SET);
            break;
        default:    // 000
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_2_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ThermocoupleCS_1_GPIO_Port,ThermocoupleCS_3_Pin,GPIO_PIN_RESET);
            break;
            
            
    }
}