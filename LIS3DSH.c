/*!
 * \author no1wudi
 * \file LIS3DSH.c
 */

#include "LIS3DSH.h"



#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"

#include "stm32f4xx_gpio.h"


#define CS_H GPIO_SetBits(GPIOE,GPIO_Pin_3)			//para chip select
#define CS_L GPIO_ResetBits(GPIOE,GPIO_Pin_3)		//




void LIS3DSH_Init_SPI_Bus(void){
	SPI_InitTypeDef SPI_FOR_LIS3DSH;
	GPIO_InitTypeDef GPIO_FOR_LIS3DSH;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	//pasa los parametros para que los puertos sean SPI

	GPIO_FOR_LIS3DSH.GPIO_Mode = GPIO_Mode_AF;   //GPIO Alternate function Mode
	GPIO_FOR_LIS3DSH.GPIO_OType = GPIO_OType_PP;
	GPIO_FOR_LIS3DSH.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_FOR_LIS3DSH.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_FOR_LIS3DSH.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_FOR_LIS3DSH);

	//Pin A5, A6 y A7 se usan como SPI
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);


	//CS Pin
	GPIO_FOR_LIS3DSH.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_FOR_LIS3DSH.GPIO_Pin = GPIO_Pin_3;
	GPIO_FOR_LIS3DSH.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_FOR_LIS3DSH);

	CS_H;

	//configuracion SPI

	SPI_FOR_LIS3DSH.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_FOR_LIS3DSH.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_FOR_LIS3DSH.SPI_CPOL = SPI_CPOL_High;
	SPI_FOR_LIS3DSH.SPI_CRCPolynomial = 7;
	SPI_FOR_LIS3DSH.SPI_DataSize = SPI_DataSize_8b;
	SPI_FOR_LIS3DSH.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_FOR_LIS3DSH.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_FOR_LIS3DSH.SPI_Mode = SPI_Mode_Master;
	SPI_FOR_LIS3DSH.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_FOR_LIS3DSH);
	SPI_Cmd(SPI1,ENABLE);
}



unsigned char LIS3DSH_SPI_Read_Write(unsigned char Data){
	SPI_I2S_SendData(SPI1,Data);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}





/*!
 * \brief You can edit your own init process here.
 */

void LIS3DSH_Init(void) {
	LIS3DSH_Init_SPI_Bus();
}

/*!
 * \brief Read the content of the register
 * \param LIS3DSH_Reg The address of the object register.
 */

unsigned char LIS3DSH_Read(unsigned char LIS3DSH_Reg) {
	unsigned char Temp;
	CS_L;
	LIS3DSH_SPI_Read_Write(LIS3DSH_Reg | LIS3DSH_Read_Mask);
	Temp = LIS3DSH_SPI_Read_Write(0xAA);
	CS_H;
	return Temp;
}

/*!
 *\brief Write data to the register.
 *\param LIS3DSH_Reg The object register.
 *\param Data The data will be written.
 */

void LIS3DSH_Write(unsigned char LIS3DSH_Reg, unsigned char Data) {
	CS_L;
	LIS3DSH_SPI_Read_Write(LIS3DSH_Reg | LIS3DSH_Write_Mask);
	LIS3DSH_SPI_Read_Write(Data);
	CS_H;
}

/*!
 * \brief Set the output status.
 * \param LIS3DSH_Output The setting of the output.
 */

void LIS3DSH_Set_Output(unsigned char LIS3DSH_Output) {
	LIS3DSH_Write(LIS3DSH_Reg_Ctrl_4, LIS3DSH_Output);
}

/*!
 * \brief Set The output feature.
 * \param LIS3DSH_Output_Feature The feature of output.
 */
void LIS3DSH_Set_Output_Feature(unsigned char LIS3DSH_Output_Feature) {
	LIS3DSH_Write(LIS3DSH_Reg_Ctrl_5, LIS3DSH_Output_Feature);
}

/*!
 * \brief Get the output value of X-axis.
 * \param LIS3DSH_Sense Sensitivity of the sensor.
 * \return The value of x-axis
 */

float LIS3DSH_Get_X_Out(float LIS3DSH_Sense) {
	short Temp;

	Temp = LIS3DSH_Read(LIS3DSH_Reg_X_Out_H);
	Temp = Temp << 8;
	Temp = Temp + LIS3DSH_Read(LIS3DSH_Reg_X_Out_L);
	return (float) Temp * LIS3DSH_Sense / 1000;
}

/*!
 * \brief Get the output value of y-axis.
 * \param LIS3DSH_Sense Sensitivity of the sensor.
 * \return The value of y-axis
 */
float LIS3DSH_Get_Y_Out(float LIS3DSH_Sense) {
	short Temp;

	Temp = LIS3DSH_Read(LIS3DSH_Reg_Y_Out_H);
	Temp = Temp << 8;
	Temp = Temp + LIS3DSH_Read(LIS3DSH_Reg_Y_Out_L);
	return (float) Temp * LIS3DSH_Sense / 1000;
}

/*!
 * \brief Get the output value of Z-axis.
 * \param LIS3DSH_Sense Sensitivity of the sensor.
 * \return The value of Z-axis
 */

float LIS3DSH_Get_Z_Out(float LIS3DSH_Sense) {
	short Temp;

	Temp = LIS3DSH_Read(LIS3DSH_Reg_Z_Out_H);
	Temp = Temp << 8;
	Temp = Temp + LIS3DSH_Read(LIS3DSH_Reg_Z_Out_L);
	return (float) Temp * LIS3DSH_Sense / 1000;
}
