/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <ctype.h>
#include "variables.h"


/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

volatile bool nmi;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  while (1)
  {
		nmi = true;
		NVIC_SystemReset();
	}
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
volatile bool hard_fault;

void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
		hard_fault = true;
    NVIC_SystemReset();
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */
  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */
  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */
  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */
  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/******************************************************************************/
/** TIM14 period is 100 �second. Thus, 1 ms will be 10 TIM14 Periods(p), 10 ms->100p, 100ms->1000p, 1s->10000p and so on.**/
void TIM14_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim14);

	if (timerLEDstRojo!= 0) timerLEDstRojo--;

	timersegundo++;
	timer100milis++;
	timer10milis++;
	timerLEDstRojo--;
	timermilis++;

#ifndef EN_DEBUG
	HAL_IWDG_Refresh(&hiwdg); // 20 Mseg para PERR0
#endif

	// COMPORTAMIENTO LED ROJO
	if (Inicializando)
	{
		HAL_GPIO_WritePin(GPIOD, LED_Pin, GPIO_PIN_SET);
	}
	else
	{
			if (timerLEDstRojo == 0)
			{
				HAL_GPIO_TogglePin(GPIOD, LED_Pin);
				if (BLE_desconectado==SI) timerLEDstRojo=2000;
				else timerLEDstRojo=10000;
			}
	}

	if (timermilis == 10)
	{
			timermilis = 0;
			timerMilis++;
	}	
	
	if (timer10milis == 100)
	{
			timer10milis = 0;
			timer10MiliSeg++;
			if (tiempoEsperaRespuesta) tiempoEsperaRespuesta--;
	}

	if (timer100milis == 1000)
	{
			timer100milis = 0;
			tiempoPulsador++;
			
		  if (system_init_done)GPIO_STATE();
		
			/**************** Locker opening timer ****************/
			if(RL1_opening_lock_timer) RL1_opening_lock_timer++;
			if(RL1_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL1_GPIO_Port, RL1_Pin, GPIO_PIN_RESET);

			if(RL2_opening_lock_timer) RL2_opening_lock_timer++;
			if(RL2_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL2_GPIO_Port, RL2_Pin, GPIO_PIN_RESET);

			if(RL3_opening_lock_timer) RL3_opening_lock_timer++;
			if(RL3_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL3_GPIO_Port, RL3_Pin, GPIO_PIN_RESET);

			if(RL4_opening_lock_timer) RL4_opening_lock_timer++;
			if(RL4_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL4_GPIO_Port, RL4_Pin, GPIO_PIN_RESET);

			if(RL5_opening_lock_timer) RL5_opening_lock_timer++;
			if(RL5_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL5_GPIO_Port, RL5_Pin, GPIO_PIN_RESET);

			if(RL6_opening_lock_timer) RL6_opening_lock_timer++;
			if(RL6_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL6_GPIO_Port, RL6_Pin, GPIO_PIN_RESET);

			if(RL7_opening_lock_timer) RL7_opening_lock_timer++;
			if(RL7_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL7_GPIO_Port, RL7_Pin, GPIO_PIN_RESET);
			
			if(RL8_opening_lock_timer) RL8_opening_lock_timer++;
			if(RL8_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL8_GPIO_Port, RL8_Pin, GPIO_PIN_RESET);
			
			if(RL9_opening_lock_timer) RL9_opening_lock_timer++;
			if(RL9_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL9_GPIO_Port, RL9_Pin, GPIO_PIN_RESET);
			
			if(RL10_opening_lock_timer) RL10_opening_lock_timer++;
			if(RL10_opening_lock_timer > OPENING_LOCK_TIME) HAL_GPIO_WritePin(RL10_GPIO_Port, RL10_Pin, GPIO_PIN_RESET);
	}
	if (timersegundo == 10000)
	{
			timersegundo = 0;
			timerSegundo++;
			timerminuto++;
			perroMain++;
			segundo++;
			Last_RL1_Op++;
			Last_RL2_Op++;
	  	Last_RL3_Op++;
			Last_RL4_Op++;
		  Last_RL5_Op++;
			Last_RL6_Op++;
	  	Last_RL7_Op++;
	  	Last_RL8_Op++;
			Last_RL9_Op++;
		  Last_RL10_Op++;
		
      last_RL1_Event++;
		  last_RL2_Event++;
			last_RL3_Event++;
			last_RL4_Event++;
		  last_RL5_Event++;
		  last_RL6_Event++;
			last_RL7_Event++;
			last_RL8_Event++;
			last_RL9_Event++;
			last_RL10_Event++;
		
			if (segundo==60)
			{
				segundo=0;
				minuto++;
				if (minuto==60)
				{
					minuto=0;
					hora++;
					if (hora==24) hora=0;
				}
			}
			
			if (tiempoEsperaRespuestaLargo) tiempoEsperaRespuestaLargo--;
			
			if (perroMain == MAX_SEGUNDOS_FUERA_MAIN)
			{
				NVIC_SystemReset();//Watchdog reset
				while(1) {}		
			}
			if (timerminuto == 60)
			{
				timerMinuto ++;
				timerminuto = 0;
			}
			
			if ((tengo_IP_WIFI==NO)&&(tengo_IP_ETH==NO))tiempoSinInternet++;
			else tiempoSinInternet=0;
			
			if (estado_MQTT == CONECTADO) tiempoSinMQTT=0;
			else tiempoSinMQTT++;
			
			if (mqttReconTimer) mqttReconTimer++;		
			if(TCP_RESTART_TIMER)TCP_RESTART_TIMER++;
			
			//BLE checktime for advtertising
			if(BLECONN_checkTim)BLECONN_checkTim++;
			//Check TIMER for BLE potential desync between the real number of connections and the registered connections in the variable.
			if(BLESyncTimer)BLESyncTimer++;
			
			//BLE forced disconnection by no incoming BLE commands. 
			if(LastBLECommand && BLE_desconectado == NO && tiempoSinInternet==0) //This way, when using Instaladores app for first time, BLE connection won't be forced to disconnect.
			{
				LastBLECommand++;
				if (LastBLECommand == LastBLECommand_MAX)
				{
						LastBLECommand = 1;
						ForceBLEDISCONN = true;
				}				
			}
			


			/**************** Wait time since opening command ****************/
			//Maximum time that system will wait since the command .ABRE is executed untill calling that the locker wasn't actually opened.
			if(RL1_monitorTimer)RL1_monitorTimer++;
			if(RL1_monitorTimer > OPENING_WAIT_TIME)
      { 
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 1, no abierta\r\n$");
					RL1_monitorTimer = 0;
			} 
			if(RL2_monitorTimer)RL2_monitorTimer++;
			if(RL2_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 2, no abierta\r\n$");
					RL2_monitorTimer = 0;
			} 	
			if(RL3_monitorTimer) RL3_monitorTimer++;
			if(RL3_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 3, no abierta\r\n$");
					RL3_monitorTimer = 0;
			}
			if(RL4_monitorTimer) RL4_monitorTimer++;
			if(RL4_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 4, no abierta\r\n$");
					RL4_monitorTimer = 0;
			}
			if(RL5_monitorTimer) RL5_monitorTimer++;
			if(RL5_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 5, no abierta\r\n$");
					RL5_monitorTimer = 0;
			}
			if(RL6_monitorTimer) RL6_monitorTimer++;
			if(RL6_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 6, no abierta\r\n$");
					RL6_monitorTimer = 0;
			}
			if(RL7_monitorTimer) RL7_monitorTimer++;
			if(RL7_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 7, no abierta\r\n$");
					RL7_monitorTimer = 0;
			}
			if(RL8_monitorTimer) RL8_monitorTimer++;
			if(RL8_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 8, no abierta\r\n$");
					RL8_monitorTimer = 0;
			}
			if(RL9_monitorTimer) RL9_monitorTimer++;
			if(RL9_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 9, no abierta\r\n$");
					RL9_monitorTimer = 0;
			}
			if(RL10_monitorTimer) RL10_monitorTimer++;
			if(RL10_monitorTimer > OPENING_WAIT_TIME)
			{
					pendingMQTTmsg = true; strcat(MQTTpendingMsg, "^Cerradura 10, no abierta\r\n$");
					RL10_monitorTimer = 0;
			}
			
			/**************** Gate left opened watcher ****************/
			if(openedGate1Watcher)openedGate1Watcher++;
			if(openedGate1Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 1, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;		
			}
			if(openedGate2Watcher)openedGate2Watcher++;
			if(openedGate2Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 2, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate3Watcher) openedGate3Watcher++;
			if(openedGate3Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 3, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate4Watcher) openedGate4Watcher++;
			if(openedGate4Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 4, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate5Watcher) openedGate5Watcher++;
			if(openedGate5Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 5, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate6Watcher) openedGate6Watcher++;
			if(openedGate6Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 6, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate7Watcher) openedGate7Watcher++;
			if(openedGate7Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 7, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate8Watcher) openedGate8Watcher++;
			if(openedGate8Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 8, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate9Watcher) openedGate9Watcher++;
			if(openedGate9Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 9, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}
			if(openedGate10Watcher) openedGate10Watcher++;
			if(openedGate10Watcher == GATE_LEFT_OPENED_MAX_TIME)
			{
					strcat(MQTTpendingMsg, "^Cerradura 10, SIN HABERSE CERRADO\r\n$");
					pendingMQTTmsg = true; alarm_topic = true;
			}		
	}
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  	uint32_t laux;
  	uint16_t x,i,a,w;
  	char straux[64];
    //UART DMA buffer is copied to strmodem 4 bytes array. Data is again transfered to other larger buffer(buffer_RX_MODEM) that will hold the ESP response commands
	buffer_RX_MODEM[Indice_A_buffer_RX_MODEM] = strmodem[0]; //Index has never been initialized ???

	if (capturaLog == SI) //Never set to a value
	{
		buffer_in_RS232_TEMP[Indice_A_buffer_RS232_TEMP]=strmodem[0]; //This index is never initialized ???
		Indice_A_buffer_RS232_TEMP++;
		buffer_in_RS232_TEMP[Indice_A_buffer_RS232_TEMP]=0;
		if (Indice_A_buffer_RS232_TEMP==255)
			{
				buffer_in_RS232_TEMP[255]=0;
				capturaLog=NO;
			}
	}        
	if (MonitorizaRS232_ESP32 == SI)
	{        
		HAL_UART_Transmit(&huart2, (void *)strmodem, 1, 20); //Incoming bytes from UART1 are transmitted by UART2
	}    
	
	if(mqtt_log == true) {pData[pInd] = strmodem[0]; pInd++;}
                 
	if ((buffer_RX_MODEM[Indice_A_buffer_RX_MODEM]) == '>')	Recibido_Prompt_ESP = SI;
	         
	if ((Indice_A_buffer_RX_MODEM) && (buffer_RX_MODEM[Indice_A_buffer_RX_MODEM] == 0xa) && (buffer_RX_MODEM[Indice_A_buffer_RX_MODEM - 1] == 0xd)) //"\r\n" means end of the AT response command.
	{   
		/***********************Here starts AT command responses processing***********************/    

		buffer_RX_MODEM[Indice_A_buffer_RX_MODEM + 1] = 0; //String Null terminator. Not checking buffer overflow(???).The code is assuming that the buffer will never overflow.
		laux = Indice_A_buffer_RX_MODEM;
		Indice_A_buffer_RX_MODEM = 0;
        
		/*********************************************  ESP32 GENERAL  *********************************************/         
		if (!memcmp(buffer_RX_MODEM, "OK", 2))	Recibido_OK_ESP = SI;
		else if (!memcmp(buffer_RX_MODEM, "ready", 5)) Recibido_READY_ESP32 = SI;
		else if (!memcmp(buffer_RX_MODEM, "busy p...", 9)) HAL_NVIC_SystemReset();
		else if (!memcmp(buffer_RX_MODEM, "ERROR",5)) ErrorEnESP = SI; 	
		
	
		/**************************************************  TCP  **************************************************/
		else if (!memcmp(buffer_RX_MODEM, "CONNECT",7)) 
		{
			conexion_TCP=true; 
			envia_msg_bienvenida_TCP=true;			
		}
		else if (!memcmp(buffer_RX_MODEM, "+IPD,",5))	//+IPD,0,4
		{
			i=5;
			x=0;

			
			while (buffer_RX_MODEM[i]!=',')
			{ 
				straux[x]=buffer_RX_MODEM[i];
				i++;x++;
			}
			i++;
			straux[x] = 0;
			sscanf((char *)straux, "%u", &w);
			TCP_connID = (uint8_t)w;
			w=0;
			x=0;
			while (buffer_RX_MODEM[i]!=':')
			{ 
				straux[x]=buffer_RX_MODEM[i];
				i++;x++;
			}
			i++;
			straux[x] = 0;
			sscanf((char *)straux, "%d", &w);
			
			uint8_t j;
			for (j=0; j<w; j++)
			{ 
				if(buffer_RX_MODEM[i+j] != '\r' && buffer_RX_MODEM[i+j] != '\n') {buffer_RX_desde_TCP[TCP_BUFF_INDEX]=buffer_RX_MODEM[i+j]; TCP_BUFF_INDEX++;}
				else break;
			}
			
			buffer_RX_desde_TCP[TCP_BUFF_INDEX]='\r';
			buffer_RX_desde_TCP[TCP_BUFF_INDEX+1]='\n';
			buffer_RX_desde_TCP[TCP_BUFF_INDEX+2]=0;
		
			TCP_COMMAND=SI;	
			//strcpy (buffer_copia_TCP_en_BLE,buffer_RX_desde_TCP);
			//envia_por_BLE=SI;		
		}
		else if (!memcmp(buffer_RX_MODEM, "0,DISCONNECT",12)) conexion_TCP=NO; 	
		else if (!memcmp(buffer_RX_MODEM, "+HTTPCLIENT:",12))	//+HTTPCLIENT:512, (hex:01234567890123456)//It represents the size of the data recieved sent by the client				        		
	    { //Respuesta al comando HTTPCLIENT con petición GET. Tras especificar el tamaño nos va a devolver el archivo que hemos solicitado
			test_Conexion_ok = SI;
			i=12; //We want to skip the first 12 characters(+HTTPCLIENT:)
			x=0;
			while (buffer_RX_MODEM[i]!=',') //Extract the size of the data sent by the client and store it in the TCP Rx buffer
			{ 
				buffer_RX_desde_TCP[x]=buffer_RX_MODEM[i];
				i++;x++;
			}
			buffer_RX_desde_TCP[x]=0; //Null terminator for the buffer
			sscanf((char *)buffer_RX_desde_TCP, "%d", &w); //The data size string is converted into an integer value(w)
			testW = i;
			x++; //Add 1 again since the index starts at 0 but we want to represent now the number of elements
			if (laux == (w+i+2)) // Check if the size of the buffer recieved is correct(data size + position where data starts + \r\n)
			{
				memcpy ((buffer_aligned+Indice_A_buffer_aligned),(buffer_RX_MODEM+12+x),w);
				Indice_A_buffer_aligned+=w;
			}
			else
			{
				Indice_A_buffer_RX_MODEM = laux;
				if (Indice_A_buffer_RX_MODEM < sizeof(buffer_RX_MODEM)) Indice_A_buffer_RX_MODEM++;
			}
     
	  	}
		/****************************************************************************************************/

		/**************************************************  WIFI/ETH  **************************************************/
		else if (!memcmp(buffer_RX_MODEM, "WIFI GOT IP", 11))
		{
			tengo_IP_WIFI=SI;
			WIFI_conectado = SI; 
			queryIP = true;
			acaboDeRecibirIP_WIFI = true;
			tiempoSinInternet = 0;
			//abre_sck_TCP=SI;
			pending_TCP_conn = true;
		}
		else if (!memcmp(buffer_RX_MODEM, "WIFI CONNECTED", 14))
		{
			WIFI_conectado = SI; 
		}
		else if (!memcmp(buffer_RX_MODEM, "+CWJAP:", 7))
		{
			error_WIFI = SI; 
			causaError_WIFI = buffer_RX_MODEM[7];
		}
		//modem reponse to CIPSTA AT to query IP address of the ESP32 and also it gives info about gateway and netmask.  	
		else if (!memcmp(buffer_RX_MODEM, "+CIPSTA:ip:\"", 12)) //+CIPSTA:ip:"192.168.1.17"(AT command response example)
		{
			for (x=0;x<16;x++) //Save the IP Adress in the global variable used then by .INFO command.
			{//The first two conditions are redundant
					if ((buffer_RX_MODEM[12+x]==0x0d)||(buffer_RX_MODEM[12+x]==0x0a)||(buffer_RX_MODEM[12+x]=='\"')) break;
					IP_WIFI[x]=buffer_RX_MODEM[12+x];
			}
			IP_WIFI[x]=0; //Null terminator
		}
		else if (!memcmp(buffer_RX_MODEM, "+ETH_GOT_IP:", 12)) 	//+ETH_GOT_IP:192.168.1.20
		{
			acaboDeRecibirIP_ETH = true;
			ETH_conectado = SI;
			tengo_IP_ETH=SI;
			queryIP = true;
			pending_TCP_conn = true;
			for (x=0;x<16;x++)
			{
					if ((buffer_RX_MODEM[12+x]==0x0d)||(buffer_RX_MODEM[12+x]==0x0a)) break;
					IP_ETH[x]=buffer_RX_MODEM[12+x];
			}
			IP_ETH[x]=0;
		}		
		else if (!memcmp(buffer_RX_MODEM, "+ETH_CONNECTED", 14))
		{
			pending_wifi_dis = true;
			ETH_conectado = SI;
		}
		else if (!memcmp(buffer_RX_MODEM, "+ETH_DISCONNECTED", 17))
		{
			ETH_conectado = NO;
			tengo_IP_ETH=NO;
			pending_wifi_con = true;
		}
		else if (!memcmp(buffer_RX_MODEM, "WIFI DISCONNECT", 15))
		{
			WIFI_conectado = NO;
			tengo_IP_WIFI=NO;
		}   
		else if(!memcmp(buffer_RX_MODEM, "+CWDHCP:", 8))
		{
			uint8_t result = buffer_RX_MODEM[8] - '0';
			DHCP_WIFI = result & 1;
			DHCP_ETH = (result >> 2) & 1;
		}   
		/****************************************************************************************************/	
		else if (!memcmp(buffer_RX_MODEM, "+CIPSNTPTIME:", 13))	//+CIPSNTPTIME:Thu Dec  7 17:45:44 2023
		{
			Captura_fecha (buffer_RX_MODEM);
			horaActualizada = SI;
		}
		else if (!memcmp(buffer_RX_MODEM, "+TIME_UPDATED", 13))
		{
			horaActualizada = SI;
		}
		else if (!memcmp(buffer_RX_MODEM, "+PING:TIMEOUT", 13)) pingData = -1;	
		else if (!memcmp(buffer_RX_MODEM, "+PING:", 6))	
		{
			char str[10];
			uint8_t i = 0;
			while(buffer_RX_MODEM[i + 6] != '\r' && buffer_RX_MODEM[i + 6] != '\n')
			{
				str[i] = buffer_RX_MODEM[i + 6];
				i++;
				if (i >= sizeof(str) - 1) break; //avoid overflow
			}
			str[i] = '\0';
			pingData = atoi(str); 	

		}
		/**************************************************  MQTT  *************************************************/
		else if (!memcmp(buffer_RX_MODEM, "+MQTTDISCONNECTED:0", 19))
		{
			estado_MQTT = DESCONECTADO;
		}
		else if (!memcmp(buffer_RX_MODEM, "+MQTTCONNECTED:0", 16))
		{
			estado_MQTT = CONECTADO;
		}
		else if (!memcmp(buffer_RX_MODEM, "+MQTTPUB:OK",11)) pubMqttOK = SI; 
		else if (!memcmp(buffer_RX_MODEM, "+BLEADDR:\"",10))
		{
			i=0;
			for (x=0;x<18;x++)
			{
				if (buffer_RX_MODEM[10+x]=='\"') break;
				if (buffer_RX_MODEM[10+x]!=':') 
					{
					MAC_BLE[i]=toupper(buffer_RX_MODEM[10+x]);
					i++;
					}
			}
			MAC_BLE[x]=0;
		}     
		else if (!memcmp(buffer_RX_MODEM, "+MQTTSUBRECV:0", 14)) //+MQTTSUBRECV:0,"cmd/ble10/1122334455/",15,TRANSFIERO ESTO (//012345678901234567890123456789012345678901234567890123456789)
		{						 
			sprintf(straux,"cmd/ble10/%s/",MAC_BLE);						
			a=strlen(straux);
			if (!memcmp(buffer_RX_MODEM+16, straux , a))
			{
				i=18+a;
				x=0;
				while (buffer_RX_MODEM[i]!=',')
				{ 
					straux[x]=buffer_RX_MODEM[i];
					i++;x++;
				}
				straux[x]=0;
				sscanf((char *)straux, "%d", &w);
				x=i+1;
				for (i=0;i<w;i++)
				{ 
					a=buffer_RX_MODEM[x+i];
					if ((a!=0xd)&&(a!=0xa)) buffer_RX_desde_MQTT[i+Indice_a_buffer_RX_desde_MQTT]=buffer_RX_MODEM[x+i];
				  		else break;
				}
				
				Indice_a_buffer_RX_desde_MQTT=Indice_a_buffer_RX_desde_MQTT+i;
				buffer_RX_desde_MQTT[Indice_a_buffer_RX_desde_MQTT]='\r';
				buffer_RX_desde_MQTT[1+Indice_a_buffer_RX_desde_MQTT]='\n';
				buffer_RX_desde_MQTT[2+Indice_a_buffer_RX_desde_MQTT]=0;
				Indice_a_buffer_RX_desde_MQTT=0;
				comando_MQTT=SI;				
			}
		}      
		/**************************************************  BLE  **************************************************/
		else if (!memcmp(buffer_RX_MODEM, "+BLEDISCONN:", 12))	//+BLEDISCONN:0,"44:75:30:95:71:09" 								
			{//MODIFIED*
			eventos_BLE_nuevo=SI;
			BLE_reconectar = SI;
				
			if(numero_conexiones_BLE > 0) numero_conexiones_BLE--;
			if (numero_conexiones_BLE == 0 ) BLE_desconectado = SI;
			
			if (buffer_RX_MODEM[12]=='0'){ BLE0=NO; strcpy((void *)buffer_eventos_BLE,"BLE disconecction in 0 , with MAC address: ");}
			else if (buffer_RX_MODEM[12]=='1'){ BLE1=NO; strcpy((void *)buffer_eventos_BLE,"BLE disconecction in 1 , with MAC address: ");}
			strcat (buffer_eventos_BLE,buffer_RX_MODEM+14);
			}
		else if (!memcmp(buffer_RX_MODEM, "+BLECONN:", 9)) 	//+BLECONN:0,"7c:d6:92:bb:89:ad" //0123456789012345678901234567890123456789
			{//MODIFIED*
			BLE_desconectado = NO;
			eventos_BLE_nuevo=SI;
			LastBLECommand = 1;
			
			if (numero_conexiones_BLE < 2) numero_conexiones_BLE++;
			if (numero_conexiones_BLE < 2) BLE_reconectar = SI;
		
			if (buffer_RX_MODEM[9]=='0'){ BLE0=SI; strcpy((void *)buffer_eventos_BLE,"BLE conecction in 0 , with MAC address: ");}
			else if (buffer_RX_MODEM[9]=='1'){ BLE1=SI; strcpy((void *)buffer_eventos_BLE,"BLE conecction in 1 , with MAC address: ");}
			//else if (buffer_RX_MODEM[9]=='2'){ BLE1=SI; strcpy((void *)buffer_eventos_BLE,"BLE conecction in 2 , with MAC address: ");}
			strcat (buffer_eventos_BLE,buffer_RX_MODEM+11);	
			}
    /*else if (!memcmp(buffer_RX_MODEM, "+BLECONNPARAM:",14)) //Created
		{
			print("Visualizing connection parameters: ");
			print((void*)buffer_RX_MODEM);
			print("\r\n");
		}*/

		recibido_de_conexion_BLE=0;
		if (!memcmp(buffer_RX_MODEM, "+WRITE:0,1,5,,", 14)) recibido_de_conexion_BLE='0';
		else if (!memcmp(buffer_RX_MODEM, "+WRITE:1,1,5,,", 14)) recibido_de_conexion_BLE='1';
		if (recibido_de_conexion_BLE == '0' || recibido_de_conexion_BLE == '1' )		//+WRITE:0,1,5,,4,text    // +write:<conn_index>,<srv_index>,<char_index>[,<desc_index>],<length>
		{				
		  //Extract length of the incoming BLE packet
			i=14;
			x=0;
			while (buffer_RX_MODEM[i]!=',')
			{ 
				straux[x]=buffer_RX_MODEM[i];
				i++;x++;
			}
			straux[x]=0; //add null terminator 	
			sscanf((char *)straux, "%d", &w); //Convert it to an integer
			
			for (i=0;i<w;i++)
			{ 
				a=buffer_RX_MODEM[15+x+i];
				if ((a!=0xd)&&(a!=0xa))
				{
					buffer_RX_desde_BLE[i+Indice_a_buffer_RX_desde_BLE]=buffer_RX_MODEM[15+x+i];
				}
				else break;
			}
			Indice_a_buffer_RX_desde_BLE+=i; //update the index to the amount of data recieved
			
			if ((a==0xd)||(a==0xa))
			{
				buffer_RX_desde_BLE[i+Indice_a_buffer_RX_desde_BLE]='\r';
				buffer_RX_desde_BLE[i+1+Indice_a_buffer_RX_desde_BLE]='\n';
				buffer_RX_desde_BLE[i+2+Indice_a_buffer_RX_desde_BLE]=0;
				enviar_por_conexion_BLE=recibido_de_conexion_BLE; //para qué si mediante el if esto sólo aplica para 0 ...
				Indice_a_buffer_RX_desde_BLE=0;
				comando_BLE=SI;				
			}	
		}
	}		
	else if (Indice_A_buffer_RX_MODEM < sizeof(buffer_RX_MODEM)) Indice_A_buffer_RX_MODEM++;

	HAL_UART_Receive_IT(&huart1, (void *)strmodem, 1);

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)//PEND*
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	HAL_UART_IRQHandler(&huart2);
	HAL_UART_Receive_IT(&huart2, (void *)strconsole, 1);

	if (strconsole[0] == 0x08)
	{
		Indice_A_buffer_RS232_TEMP--; // BackSpace
	}
	else if (strconsole[0] != 0x0a)
	{
		buffer_in_RS232_TEMP[Indice_A_buffer_RS232_TEMP] = strconsole[0];
		if (Indice_A_buffer_RS232_TEMP < sizeof(buffer_in_RS232_TEMP)) Indice_A_buffer_RS232_TEMP++;
		if ((strconsole[0] == 0x0DU) || (strconsole[0] == '}'))
		{
			comando_consola = SI;
			buffer_in_RS232_TEMP[Indice_A_buffer_RS232_TEMP] = 0;
		}
	}
  //HAL_UART_IRQHandler(&huart2);//REDUNDANT. DELETE. COULD CAUSE POTENTIAL ISSUES.
}
 
