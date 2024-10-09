/*
 * dh11.h
 *
 *  Created on: Oct 1, 2024
 *      Author: vaece
 */

#ifndef INC_DH11_H_
#define INC_DH11_H_
#include <stdint.h>

void DH11_GPIO_Init(uint8_t *temp, uint8_t *humi);
void GET_DH11_MSG();

#endif /* INC_DH11_H_ */
