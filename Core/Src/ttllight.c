/*
 * ttllight.c
 *
 *  Created on: Oct 1, 2024
 *      Author: vaece
 */
#include "main.h"

void startTTLLight(){
	  HAL_GPIO_WritePin(GPIOB, R_Pin, ENABLE);
	  HAL_Delay(500);
	  HAL_GPIO_WritePin(GPIOB, G_Pin, ENABLE);
	  HAL_Delay(500);
	  HAL_GPIO_WritePin(GPIOB, B_Pin, ENABLE);
	  HAL_Delay(500);
	  HAL_GPIO_WritePin(GPIOB, R_Pin, RESET);
	  HAL_GPIO_WritePin(GPIOB, G_Pin, RESET);
	  HAL_GPIO_WritePin(GPIOB, B_Pin, RESET);
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(GPIOB, R_Pin, ENABLE);
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(GPIOB, R_Pin, RESET);
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(GPIOB, G_Pin, ENABLE);
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(GPIOB, G_Pin, RESET);
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(GPIOB, B_Pin, ENABLE);
	  HAL_Delay(200);
 	  HAL_GPIO_WritePin(GPIOB, B_Pin, RESET);

}
