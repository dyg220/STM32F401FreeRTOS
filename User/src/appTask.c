#include "appTask.h"

//--------Task1----------//
void Task1(void* pvParameters);//����1����
TaskHandle_t Task1_Handle_t;//����1���
//--------Task1----------//

//--------Task2----------//
void Task2(void* pvParameters);//����2����
TaskHandle_t Task2_Handle_t;//����2���
//--------Task2----------//

//--------Task3----------//
void Task3(void* pvParameters);//����3����
TaskHandle_t Task3_Handle_t;//����3���
//--------Task3----------//

u8 RFID_ID1[4] = { 0 };

/******************************************************
*�� �� ����AppStartTask
*�������ܣ����ڳ�ʼ��
*��    ����Baud
*�� �� ֵ��None
*��    ע��
*******************************************************/
void AppStartTask(void)
{

	taskENTER_CRITICAL();//�����ٽ���(���ж�)
	xTaskCreate(Task1, "task1", 128, NULL, 4, &Task1_Handle_t);//���񴴽����������񵽾���̬
	//	xTaskCreate(Task2,"task2",128,NULL,4,&Task2_Handle_t);//���񴴽����������񵽾���̬
	//	xTaskCreate(Task3,"task3",128,NULL,4,&Task3_Handle_t);//���񴴽����������񵽾���̬

	taskEXIT_CRITICAL();//�˳��ٽ���(���ж�)

	vTaskStartScheduler();//�����������  ������ȼ�����͵�����̬
}

//������1
void Task1(void* pvParameters)
{
	while (1)
	{
		CY8CMBR3116_Open_Door();
	}
}

//������2
void Task2(void* pvParameters)
{
	while (1)
	{

	}
}

//������3
void Task3(void* pvParameters)
{
	while (1)
	{

	}
}


