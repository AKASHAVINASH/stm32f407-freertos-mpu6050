/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "gpio.h"
#include "i2c.h"
#define MPU6050_ADDR  (0x68 << 1)
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}
uint8_t Rec_Data[6];
int16_t Accel_X_RAW;
int16_t Accel_Y_RAW;
int16_t Accel_Z_RAW;
float Ax;
float Ay;
float Az;
char ax_str[16], ay_str[16], az_str[16];
int int_i = 0;
int i = 0;
char int_buf[10];
int16_t  Gyro_X_RAW;
int16_t  Gyro_Y_RAW;
int16_t  Gyro_Z_RAW;
float Gx;
float Gy;
float Gz;
char *leftSideDown     = "Left side down";
char *rightSideDown    = "Right side down";
char *frontSideDown    = "Front side down";
char *backSideDown     = "Back side down";
char *bottomDown       = "CENTER";
char *topSideDown      = "Top side down (flipped)";
char *newline          = "\r\n";
void float_to_string(float value, char *buf, int precision)
{
    i = 0;
    if (value < 0)
    {
        buf[i++] = '-';
        value = -value;
    }

    int int_part = (int)value;
    float frac_part = value - (float)int_part;


     int_i = 0;

    if (int_part == 0) {
        int_buf[int_i++] = '0';
    } else {
        while (int_part > 0)
        {
            int_buf[int_i++] = (int_part % 10) + '0';
            int_part /= 10;
        }
    }

    for (int j = int_i - 1; j >= 0; j--)
        buf[i++] = int_buf[j];

    buf[i++] = '.';

    for (int p = 0; p < precision; p++)
    {
        frac_part *= 10;
        int digit = (int)frac_part;
        buf[i++] = digit + '0';
        frac_part -= digit;
    }

    buf[i] = '\0';
}

void append_string(char *dest, const char *src, int *index)
{
    while (*src)
    {
        dest[(*index)++] = *src++;
    }
}

void MPU6050_Read_Gyro (void)
{
	char buf[64];
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register
	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, 0x43, 1, Rec_Data, 6, 1000);

	Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	Gx = (float)Gyro_X_RAW/131.0;
	Gy = (float)Gyro_Y_RAW/131.0;
	Gz = (float)Gyro_Z_RAW/131.0;

	float_to_string(Gx, ax_str, 2);
	float_to_string(Gy, ay_str, 2);
	float_to_string(Gz, az_str, 2);

	int index = 0;

	append_string(buf, "Ax: ", &index);
	append_string(buf, ax_str, &index);
	append_string(buf, " Ay: ", &index);
	append_string(buf, ay_str, &index);
	append_string(buf, " Az: ", &index);
	append_string(buf, az_str, &index);
	buf[index++] = '\r';
	buf[index++] = '\n';
	buf[index] = '\0';

	HAL_UART_Transmit(&huart4, (uint8_t*)buf, strlen(buf), 100);
	osDelay(10);

	HAL_UART_Transmit(&huart4, (uint8_t*)buf, strlen(buf), 100);
	osDelay(10);

	HAL_UART_Transmit(&huart4, (uint8_t*)buf, strlen(buf), 100);
	osDelay(10);
}
void MPU6050_Read_Accel (void)
{
	char buf[64];
	// Read 6 BYTES of data starting from ACCEL_XOUT_H (0x3B) register
	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, 0x3B, 1, Rec_Data, 6, 1000);
	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);
	Ax = (float)Accel_X_RAW/16384.0;
	Ay = (float)Accel_Y_RAW/16384.0;
	Az = (float)Accel_Z_RAW/16384.0;

	float abs_x = fabsf(Ax);
	float abs_y = fabsf(Ay);
	float abs_z = fabsf(Az);

	if (abs_x > abs_y && abs_x > abs_z)
	{
	    HAL_UART_Transmit(&huart4, (uint8_t*)((Ax > 0) ? leftSideDown : rightSideDown), strlen((Ax > 0) ? leftSideDown : rightSideDown), 100);
	    HAL_UART_Transmit(&huart4, (uint8_t*)newline, 2, 100);
	    osDelay(5);
	}
	else if (abs_y > abs_x && abs_y > abs_z)
	{
		if(Ay > 0)
		{
			HAL_UART_Transmit(&huart4, (uint8_t*) frontSideDown , strlen(frontSideDown), 100);
			HAL_UART_Transmit(&huart4, (uint8_t*)newline, 2, 100);
			osDelay(5);
		}
		else
		{
			 HAL_UART_Transmit(&huart4, (uint8_t*) backSideDown, strlen(backSideDown), 100);
			 HAL_UART_Transmit(&huart4, (uint8_t*)newline, 2, 100);
			 osDelay(5);
		}

	}
	else
	{
		if(Az > 0)
		{
			 HAL_UART_Transmit(&huart4, (uint8_t*)bottomDown, strlen(bottomDown), 100);
			 HAL_UART_Transmit(&huart4, (uint8_t*)newline, 2, 100);
			 osDelay(5);
		}
		else
		{
			 HAL_UART_Transmit(&huart4, (uint8_t*)topSideDown, strlen(topSideDown), 100);
			 HAL_UART_Transmit(&huart4, (uint8_t*)newline, 2, 100);
			 osDelay(5);
		}

	}
//	float Ax_ms2 = Ax * 9.81f;
//	float Ay_ms2 = Ay * 9.81f;
//	float Az_ms2 = Az * 9.81f;
//
//	float_to_string(Ax_ms2, ax_str, 2);
//	float_to_string(Ay_ms2, ay_str, 2);
//	float_to_string(Az_ms2, az_str, 2);
//
//
//	int index = 0;
//
//	append_string(buf, "Ax: ", &index);
//	append_string(buf, ax_str, &index);
//	append_string(buf, " Ay: ", &index);
//	append_string(buf, ay_str, &index);
//	append_string(buf, " Az: ", &index);
//	append_string(buf, az_str, &index);
//	buf[index++] = '\r';
//	buf[index++] = '\n';
//	buf[index] = '\0';
//
//	HAL_UART_Transmit(&huart4, (uint8_t*)buf, strlen(buf), 100);
//	osDelay(5);



}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
	 uint8_t check;
	 uint8_t Data;
	 int I2C_Status = 0;

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  if(HAL_I2C_IsDeviceReady(&hi2c1 ,MPU6050_ADDR, 3,  100) == HAL_OK )
  {
	  char msg[20];
	  HAL_I2C_Mem_Read (&hi2c1,  0x68 << 1, 0x75, 1, &check, 1, 1000);  // read WHO_AM_I
      if(check == 0x68)
      {

    	  strcpy(msg, "ready to work\r\n");
    	  HAL_UART_Transmit(&huart4, (uint8_t*)msg, strlen(msg), 100);
    	  osDelay(1);
    	  Data = 0;
    	  HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x6B, 1, &Data, 1, 1000);
    	  osDelay(1);
    	  Data = 0x07;
    	  if(HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x19, 1, &Data, 1, 1000) == HAL_OK)
    	  {
    		  strcpy(msg, "fffffft\r\n");
    		  HAL_UART_Transmit(&huart4, (uint8_t*)msg, strlen(msg), 100);
    		  Data = 0x00;  // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
    		  osDelay(1);
			  HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1C, 1, &Data, 1, 1000);
			  osDelay(10);
			  Data = 0x00;  // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 ̐/s
			  HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1B, 1, &Data, 1, 1000);
			  I2C_Status = 1;
    		  osDelay(1);
    	  }
      }
      else
      {
    	  strcpy(msg, "found but not ready \n");
    	  HAL_UART_Transmit(&huart4, (uint8_t*)msg, strlen(msg), 100);
    	  osDelay(10);
      }

  }
  else
  {
	  char msg[] = "DEVICE not FOUND\r\n";
	  HAL_UART_Transmit(&huart4, (uint8_t*)msg, strlen(msg), 100);
	  osDelay(1);
  }
  char msg[] = "Rtos Running like baremetal\r\n";
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	  osDelay(1);
	  HAL_UART_Transmit(&huart4, (uint8_t*)msg, strlen(msg), 100);
	  osDelay(4);
	  if(I2C_Status == 1)
	  {
		  MPU6050_Read_Accel();
		 // MPU6050_Read_Gyro();

	  }

	  osDelay(500);

  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
