#include "appTask.h"

//--------Task1----------//
void Task1(void* pvParameters);//任务1函数
TaskHandle_t Task1_Handle_t;//任务1句柄
//--------Task1----------//

//--------Task2----------//
void Task2(void* pvParameters);//任务2函数
TaskHandle_t Task2_Handle_t;//任务2句柄
//--------Task2----------//

//--------Task3----------//
void Task3(void* pvParameters);//任务3函数
TaskHandle_t Task3_Handle_t;//任务3句柄
//--------Task3----------//

u8 RFID_ID1[4] = { 0 };

/******************************************************
*函 数 名：AppStartTask
*函数功能：串口初始化
*参    数：Baud
*返 回 值：None
*备    注：
*******************************************************/
void AppStartTask(void)
{

	taskENTER_CRITICAL();//进入临界区(关中断)
	xTaskCreate(Task1, "task1", 128, NULL, 4, &Task1_Handle_t);//任务创建出来让任务到就绪态
	//	xTaskCreate(Task2,"task2",128,NULL,4,&Task2_Handle_t);//任务创建出来让任务到就绪态
	//	xTaskCreate(Task3,"task3",128,NULL,4,&Task3_Handle_t);//任务创建出来让任务到就绪态

	taskEXIT_CRITICAL();//退出临界区(开中断)

	vTaskStartScheduler();//开启任务调度  最高优先级任务就到运行态
}

//任务函数1
void Task1(void* pvParameters)
{
	while (1)
	{
		CY8CMBR3116_Open_Door();
	}
}

//任务函数2
void Task2(void* pvParameters)
{
	while (1)
	{

	}
}

//任务函数3
void Task3(void* pvParameters)
{
	while (1)
	{

	}
}


