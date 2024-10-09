/*
 * dh11.c
 *
 *  Created on: Oct 1, 2024
 *      Author: vaece
 */
#include "dh11.h"
#include "main.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include "oled.h"
#define PIN_DAT  GPIO_PIN_11

void DH11_SET_GPIO(uint8_t model) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = model;
	GPIO_InitStructure.Pin = PIN_DAT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t DH11_GET_DATA() {
	// 字段组成为 50us开始+26~28的0 或者70的1 重复 40次数 8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据+8bit校验和
	//开始获取数据
	//一组8bit 一共5组
	//因为每次开始都会有50微秒的开始位 0为25-28 1为70 所以直接延时 90us 如果为高电平这代表1 否则为0 并且为0时电平上拉
	uint8_t temp;
	uint8_t ReadDat = 0;

	uint8_t retry = 0;
	uint8_t i;
	//二进数除以十即为数据

	for (i = 0; i < 8; i++) {
		//消耗准备信号低电平50us
		while (HAL_GPIO_ReadPin(GPIOA, PIN_DAT) == GPIO_PIN_RESET && retry < 100) {
			delay_us(1);
			retry++;
		}
		retry = 0;

		//虽然0信号持续时间为28us，retry循环非常巧妙，
		//避免问题,相当于如果一直是0的话可以避免延时带来的多出时间
		delay_us(30);		//如果是0，那temp=0起作用，非0，仍然在高电平时间内

		//因为如果数据是0的话持续时间短，故先预置0，简化代码。

		temp = 0;		//数字信号0，temp=0
		//数字0信号高电平持续28us，数字1信号高电平70us，延时30us以确认数字0或1
		if (HAL_GPIO_ReadPin(GPIOA, PIN_DAT) == GPIO_PIN_SET) {
			temp = 1;
		}
		//数字1信号高电平剩余40us
		while (HAL_GPIO_ReadPin(GPIOA, PIN_DAT) == GPIO_PIN_SET && retry < 100) {
			delay_us(1);
			retry++;
		}
		retry = 0;
		//<<=左移赋值符号，|=先或再赋值，得到1位数据，即8bit
		ReadDat <<= 1;
		ReadDat |= temp;
	}
	return ReadDat;
}

void DH11_GPIO_Init(uint8_t *temp, uint8_t *humi) {

	uint8_t i, data[5];
	uint8_t retry = 0;

	//stm32 PB14设置为输出，发送开始信号低电平18ms，高电平40us
	DH11_SET_GPIO(GPIO_MODE_OUTPUT_PP);
	HAL_GPIO_WritePin(GPIOA, PIN_DAT, RESET);		//拉低引脚输出电平
	HAL_Delay(18);		//保证dht11能收到起始信号

	HAL_GPIO_WritePin(GPIOA, PIN_DAT, SET);		//拉高引脚输出电平
	delay_us(40);		//延时40us，等待dht11响应信号

	//stm32 PB14设置为输入，检查并接收响应信号低电平80us，高电平80us
	DH11_SET_GPIO(GPIO_MODE_INPUT);		//可以不切换，直接输出高电平，输入也是开启的
	delay_us(20);
	//延时20us，dht11响应低电平80us，还剩60us，检查是否是低电平以确定是否有响应信号

	if (HAL_GPIO_ReadPin(GPIOA, PIN_DAT) == GPIO_PIN_RESET) {//第一个循环消耗响应低电平60us
		while (HAL_GPIO_ReadPin(GPIOA, PIN_DAT) == GPIO_PIN_RESET && retry < 100)//接收响应信号低电平剩余60us
		{
			delay_us(1);
			retry++;
		}

		retry = 0;	//超过100us自动向下运行，以免卡死
		//消耗dht11拉高80us
		while (HAL_GPIO_ReadPin(GPIOA, PIN_DAT) == GPIO_PIN_SET && retry < 100)	//接收响应信号高电平80us
		{
			delay_us(1);
			retry++;
		}
		retry = 0;

		//接收8字节数据,每传送1bit之前先进行拉低50us
		//接收了5位数据，40bit
		for (i = 0; i < 5; i++) {
			data[i] = DH11_GET_DATA();
		}
		delay_us(50);			//DHT11拉低总线50us作为一字节结束信号，或者使用以下语句
		if ((data[0] + data[1] + data[2] + data[3]) == data[4]) {
			*humi = data[0];
			*temp = data[2];
		}
	}
}

char data[50];
uint8_t temp, humi;
void GET_DH11_MSG() {
	//获取温湿
	DH11_GPIO_Init(&temp, &humi);
	sprintf(data, "温度：%d,湿度：%d", temp, humi);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*) data, strlen(data));

	for (uint8_t i = 0; i < 64; i++) {
		OLED_NewFrame();
		OLED_PrintString(64 - 2 * i, 10, data, &fontdh1116x16,
				OLED_COLOR_NORMAL);
		OLED_ShowFrame();
	}

//	HAL_Delay(2000);
}
