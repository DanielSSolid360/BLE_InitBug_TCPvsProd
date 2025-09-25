#include "variables.h"


void Procesa_consola(void)
{
	uint16_t b;
	char x,i;
	// char str[64];

	perroMain=0;
	if (comando_consola)
	{
		comando_consola = NO;
		strcpy((void *)buffer_in, (void *)buffer_in_RS232_TEMP);
		memset(buffer_in_RS232_TEMP,0,sizeof(buffer_in_RS232_TEMP));
		Indice_A_buffer_RS232_TEMP = 0;
		b = strlen((void *)buffer_in);
		b = RX_App();
		memset(buffer_in,0,sizeof(buffer_in));
		print((void *)buffer_out);
	}
	if (comando_BLE)
	{
		comando_BLE = NO;
		strcpy((void *)buffer_in, (void *)buffer_RX_desde_BLE);
		memset ((void *)buffer_RX_desde_BLE,0,255);
		Indice_a_buffer_RX_desde_BLE = 0;
		b = RX_App();
		memset(buffer_in,0,sizeof(buffer_in));
		print((void *)buffer_out);
		WR_en_BLE((void *)buffer_out,b);
		LastBLECommand = 1;
	}
	if (comando_TCP)
		{
		strcpy((void *)buffer_in, (void *)buffer_RX_desde_TCP);
		b = strlen((void *)buffer_in);
		b = RX_App();
		print((void *)buffer_out);
		WR_en_TCP((void *)buffer_out,b);
		comando_TCP = NO;
		}

	if (comando_TCP_ETH)
		{
		strcpy((void *)buffer_in, (void *)buffer_RX_desde_TCP);
		b = RX_App();
		print((void *)buffer_out);
		WR_en_TCP((void *)buffer_out,b);
		comando_TCP_ETH = NO;
		}

	if (comando_MQTT)
	{
		comando_MQTT = NO;
		strcpy((void *)buffer_in, (void *)buffer_RX_desde_MQTT);
		Indice_a_buffer_RX_desde_MQTT = 0;
		memset(buffer_RX_desde_MQTT,0,sizeof(buffer_RX_desde_MQTT));
		
		b = strlen((void *)buffer_in);
		i=0;
		for (x=0;x<b;x++)
		{
			if ((buffer_in[x]!=0xd)&&(buffer_in[x]!=0xa)&&(buffer_in[x]!='.') && (buffer_in[x]!=' ')) //Sending spaces in the topic will resultin errors. 
			{
				copiaUltimoCmd[i]=buffer_in[x];
				i++;
				copiaUltimoCmd[i]=0;
			}
		}
		if(!memcmp(copiaUltimoCmd,"ESPSEND", 7)) copiaUltimoCmd[7]=0;
		
		memset(buffer_RX_desde_MQTT,0,sizeof(buffer_RX_desde_MQTT));
		b = RX_App();
		memset(buffer_in,0,sizeof(buffer_in));
		print((void *)buffer_out);
		WR_en_MQTT((void *)buffer_out,strlen(buffer_out),SI);
		}
}
	
void Copia_SSDI(void)
{
	char x;
	
	for (x=0;x<0x40;x++)
	{
		ssid[x]=EspejoFlash[x+0x10];
		if ((ssid[x]==0)||(ssid[x]==0xff)) break;
	}

}	
void Copia_PASSWORD(void)
{
	char x;
	
	for (x=0;x<0x40;x++)
		{
		password[x]=EspejoFlash[x+0x50];
		if ((password[x]==0)||(password[x]==0xff)) break;
		}

}	

void GPIO_STATE(void)
{
	GPIO_processing_logic(1, CT1_GPIO_Port, CT1_Pin, &last_RL1_Event, &Last_RL1_Op, &openedGate1Watcher, &RL1_monitorTimer);
	GPIO_processing_logic(2, CT2_GPIO_Port, CT2_Pin, &last_RL2_Event, &Last_RL2_Op, &openedGate2Watcher, &RL2_monitorTimer);
	GPIO_processing_logic(3, CT3_GPIO_Port, CT3_Pin, &last_RL3_Event, &Last_RL3_Op, &openedGate3Watcher, &RL3_monitorTimer);
	GPIO_processing_logic(4, CT4_GPIO_Port, CT4_Pin, &last_RL4_Event, &Last_RL4_Op, &openedGate4Watcher, &RL4_monitorTimer);
	GPIO_processing_logic(5, CT5_GPIO_Port, CT5_Pin, &last_RL5_Event, &Last_RL5_Op, &openedGate5Watcher, &RL5_monitorTimer);
	GPIO_processing_logic(6, CT6_GPIO_Port, CT6_Pin, &last_RL6_Event, &Last_RL6_Op, &openedGate6Watcher, &RL6_monitorTimer);
	GPIO_processing_logic(7, CT7_GPIO_Port, CT7_Pin, &last_RL7_Event, &Last_RL7_Op, &openedGate7Watcher, &RL7_monitorTimer);
	GPIO_processing_logic(8, CT8_GPIO_Port, CT8_Pin, &last_RL8_Event, &Last_RL8_Op, &openedGate8Watcher, &RL8_monitorTimer);
	GPIO_processing_logic(9, CT9_GPIO_Port, CT9_Pin, &last_RL9_Event, &Last_RL9_Op, &openedGate9Watcher, &RL9_monitorTimer);
	GPIO_processing_logic(10, CT10_GPIO_Port, CT10_Pin, &last_RL10_Event, &Last_RL10_Op, &openedGate10Watcher, &RL10_monitorTimer);

}

void GPIO_processing_logic(uint8_t RLn, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_X, volatile uint16_t* last_RL_Event, volatile uint32_t* Last_RL_Op, volatile uint32_t* openedGateWatcher, volatile uint16_t* RL_monitorTimer)
{
	char log[80];
	if (HAL_GPIO_ReadPin(GPIO_Port,GPIO_X) &&  *last_RL_Event > timeAmongEvents) 
	{
		*last_RL_Event = 0;
		if(*Last_RL_Op > ForcedDoorTime && !*openedGateWatcher)
		{
			sprintf(log,"^Cerradura %u, FORZADA\r\n$",RLn); 
			*openedGateWatcher = 1;
		  pendingMQTTmsg = true; alarm_topic = true;
			strcat(MQTTpendingMsg,log);
		}
		else if (*Last_RL_Op < ForcedDoorTime && !*openedGateWatcher)
		{
			sprintf(log,"^Cerradura %u, abierta\r\n$",RLn);
			*RL_monitorTimer = 0; //Stop timer to monitor if the door was opened after sending the .ABRE command.
			*openedGateWatcher = 1; //Start timer to watch if the user left the door opened
		  pendingMQTTmsg = true; strcat(MQTTpendingMsg,log);
		}
	}
	/******** Doors left opened. Watcher ********/
	if(*openedGateWatcher && *last_RL_Event > 0)
	{
		if(!HAL_GPIO_ReadPin(GPIO_Port, GPIO_X))//If detects that magnetic sensor contact is closed then stop timer that will trigger the opened door alarm.
    {
			if(*openedGateWatcher <= GATE_LEFT_OPENED_MAX_TIME) sprintf(log,"^Cerradura %u, cerrada\r\n$",RLn);
			else sprintf(log,"^Cerradura %u, dejada abierta ya cerrada\r\n$",RLn);
			*openedGateWatcher = 0; 
			*last_RL_Event = 0;
			pendingMQTTmsg = true; strcat(MQTTpendingMsg,log);
		}
	}
}
/********************************************************************************************/
/****************************** Pending and handlers functions ******************************/
/********************************************************************************************/
void pending_MQTT_messages() //CREATED
{
	char auxmsg[400];
	if(pendingMQTTmsg)
	{
		pendingMQTTmsg = false;
		strcpy(auxmsg,MQTTpendingMsg);
		memset(MQTTpendingMsg,0,sizeof(MQTTpendingMsg)); //Erase buffer
		print(auxmsg);
		if (estado_MQTT == CONECTADO) 
		{	
			WR_en_MQTT(auxmsg, strlen(auxmsg), NO);
			HAL_Delay(10);
		}
		if(BLE0){ enviar_por_conexion_BLE = '0'; WR_en_BLE(auxmsg,strlen(auxmsg)); }
		if(BLE1){ enviar_por_conexion_BLE = '1'; WR_en_BLE(auxmsg,strlen(auxmsg)); }
		
	}
}


void pending_wifi_handler()//CREATED
{
	char straux[20];
	
	if (pending_wifi_dis)
	{ 
		print("\r\nNEW ETHERNET CONNECTION, DISCONNECTING WIFI...\r\n");
		strcpy((void *)straux, "AT+CWINIT=0\r\n");
		Send_RS232_ESP_ACK(500,(void *)straux);
		
		if(!conexionMqttAntesIntentada)
		{
			strcpy((void *)straux, "AT+BLEINIT=0\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			ConectarMQTT();
		 	Activar_BLE();  
		}
		pending_wifi_dis = false;
	}
	else if(pending_wifi_con)
	{ 
		if(conexionMqttAntesIntentada)print("\r\nETHERNET CONEXION LOST, RECONECTING TO WIFI...\r\n");
		else conexionMqttAntesIntentada = true;
		Init_ESP32();
		
		pending_wifi_con = false;
	}
}
	
void mqtt_recon_handler(void) //CREATED-MODIFIED
{
	byte straux[128];
	
	/*if ( mqttReconTimer > 10)
	{ 
		if ((estado_MQTT == DESCONECTADO)&&(tengo_IP_ETH==SI))
		{ 
		  strcpy((void *)straux, "AT+BLEADDR?\r\n");
	    Send_RS232_ESP_ACK(500,(void *)straux);
			
			strcpy((void *)straux, "AT+BLEINIT=0\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);

			strcpy((void *)straux, "AT+CWINIT=0\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
	
			ConectarMQTT();
			Activar_BLE();
		}
		
		mqttReconTimer = 1;
	}*/
	
	if ((tiempoSinMQTT>TIEMPO_MAX_SIN_MQTT)&&(numero_conexiones_BLE==0))
		{
		print ("Forzando RESET...;(mas de 5 minutos sin conexion con el servidor MQTT)");
		NVIC_SystemReset();
		}
}	

void check_pending_download() //CREATED
{
	if(pending_download)
	{
		ORDEN(".DESCARGA\r\n");
		if (descargaOK==SI) actualizacionPendiente = SI;
		else actualizacionPendiente = NO;
		print((void *)buffer_out);
		pending_download = false;
	}
}


void Actualizacion_Programada(void) //MODIFIED
{       
	//if (((hora==HORA_UPGRADE)&&(minuto==MINUTO_UPGRADE))||(pulsadorActivado)||(actualizacionPendiente))
	if (pulsadorActivado||actualizacionPendiente == SI) ORDEN(".UPGRADE\r\n");
	//if ((hora==HORA_RESET)&&(minuto==MINUTO_RESET))
	//	{
	//		NVIC_SystemReset();
	//	}
}    


void Reenvia_TCP_por_BLE(void)
{
	if ((envia_por_BLE==SI)&&(BLE_desconectado==NO))
		{
		envia_por_BLE=NO;

		if (BLE0 == '0')enviar_por_conexion_BLE = '0';
		if (BLE1 == '1')enviar_por_conexion_BLE = '1';//Por alg�n motivo, originalmente s�lo se env�a por BLE0
			
		WR_en_BLE((void *)buffer_copia_TCP_en_BLE, strlen(buffer_copia_TCP_en_BLE));
		}
}

void Pinta_nuevos_eventos_BLE(void)
{
	if (eventos_BLE_nuevo==SI)
		{
		print(buffer_eventos_BLE);
		eventos_BLE_nuevo=NO;
		}
}

/**************************** FLASDH AND OTA FUNCTIONS ******************************/
void Escribe_en_flash	(char *que_grabo,unsigned short offset,unsigned short cuantos)
{
	char *paux;
	unsigned short x;
	uint32_t StartPageAddress;
	char buffer[0X200] __attribute__((aligned (16)));;
	__IO uint8_t data = 0;
	
	
	// Salva el valor del IDF de pruebas y MandoAsignado
	StartPageAddress=0x0801F800;
	for (x=0;x<0x200;x++)
		{
		buffer[x]=*(__IO uint8_t *)StartPageAddress;
		StartPageAddress++;
		}

	paux=que_grabo;
	for (x=0;x<cuantos;x++)
	{
		buffer[x+offset]=*paux;
		paux++;
	}				
  	
  	//hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
	//hiwdg.Init.Reload = 3999;
  	//HAL_IWDG_Init(&hiwdg);

	prg_flash (0x0801F800,0x100,(uint32_t *) buffer);
	// lo escribe en flash

  	//hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
	//hiwdg.Init.Reload = 79;
  	//HAL_IWDG_Init(&hiwdg);

}

void Lee_en_flash	(char *que_dejo,unsigned short offset,unsigned short cuantos)
{
	char *paux;
	unsigned short x;
	uint32_t StartPageAddress;
	__IO uint8_t data = 0;
	
	
	// Salva el valor del IDF de pruebas y MandoAsignado
	StartPageAddress=0x0801F800+offset;
	for (x=0;x<cuantos;x++)
	{
		que_dejo[x]=*(__IO uint8_t *)StartPageAddress;
		StartPageAddress++;
	}
}

////////////////////////////////////////////////////////////////////////////////////
// 
//
//   Permite hacer Delay equivalente a multiplicar el UINT de entrada  por 10uSeg 
//
//
//	ENTRADA.- (UINT) Cantidad de 10uSeg que durara la pausa
//	DEVUELVE.- NADA
//
//
///////////////////////////////////////////////////////////////////////////////////

void Delay(unsigned int retraso)
{
	volatile int VariableDelay;
	//if (VarDelay<retraso)
	VariableDelay=retraso*500;
		
	while (VariableDelay!=0) 
		{
		VariableDelay--;
		}
	return; 
}

void LeeIDF(unsigned char *aqui,unsigned char * DireccionMemoria,char cuantos)
{
	unsigned char i;

	// Lee 
	
	for (i=0;i<cuantos;i++)	
	{
		*aqui=*DireccionMemoria;
		aqui++;
		DireccionMemoria++;
	}
}
unsigned short crc16(unsigned char * data_p, unsigned short length, unsigned short cr_inico)

{
    unsigned char x;
    unsigned short crc;
 
    crc = cr_inico;
 
    while (length--)
		{
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
    }
    return crc;
}

void ActualizaHora(void)
{
	char straux[128];
	
	if (((ETH_conectado)||(WIFI_conectado))&&(pendienteSMTP))
		{
		strcpy((void *)straux, "AT+CIPSNTPCFG=1,100,\"0.pool.ntp.org\",\"time.google.com\"\r\n");
		Send_RS232_ESP_ACK(500,(void *)straux);
		if (Recibido_OK_ESP) 
		{
			pendienteSMTP=NO;
			tiempoEsperaRespuestaLargo = 10;
			while ((horaActualizada == NO) && (tiempoEsperaRespuestaLargo)) {}
			if (horaActualizada == SI)
			{
				strcpy((void *)straux, "AT+CIPSNTPTIME?\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);
			}
		}
		strcpy((void *)straux, "AT+CIPSNTPINTV=3600\r\n");
		Send_RS232_ESP_ACK(500,(void *)straux);
		}
}

 uint32_t y, m, d, hh, mm, ss;		// We use 32 bits for all variables for easier calculations later.

void Captura_fecha (char *deAqui)
{
	
	// Name of Day (not used)
	char* tok = strtok(buffer_RX_MODEM + strlen("+CIPSNTPTIME:"), " ");
	
	// Month
	tok = strtok(NULL, " ");
	switch( *tok )
	{
	 case 'J':		// Jan, Jun, Jul
	if( *(tok+1) == 'a' )		// Jan
	{
		mes = 1;
	}
	else if( *(tok+2) == 'n' )	// Jun
	{
		mes = 6;
	}
	else				// Jul
	{
		mes = 7;
	}
	break;
	 case 'F':		// Feb
		mes = 2;
	break;
	 case 'M':		// Mar, May
	if( *(tok+2) == 'y' )	// May
		{
		mes = 5;
		}
	else			// Mar
		{
		mes = 3;	
		}
	break;
	 case 'A':		// Apr
	if( *(tok+1) == 'g' )	// Ago, Apr
		{
		mes = 8;
		}
	else			// Mar
		{
		mes = 4;	
		}	
	break;
	 case 'S':		// Sep
		mes = 9;
	break;
	 case 'O':		// Oct
		mes = 10;
	break;
	 case 'N':	  	// Nov
		mes = 11;
	break;
	 case 'D':		// Dec
		mes = 12;
	break;
	 default:		// Error
	mes = 0;
	break;
	}

	// Day
	tok = strtok(NULL, " ");
	dia = strtoul(tok, NULL, 10);
	
	// Time
	tok = strtok(NULL, " ");
	hora = 10 * (*(tok+0) - '0') + (*(tok+1) - '0');
	minuto = 10 * (*(tok+3) - '0') + (*(tok+4) - '0');
	segundo = 10 * (*(tok+6) - '0') + (*(tok+7) - '0');
	
	// Year
	tok = strtok(NULL, " ");
	//yOff = strtoul(tok, NULL, 10) - 2000;
	year   = strtoul(tok, NULL, 10);
         
}        

void print(char* string)//CREATED
{
	  HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string),100);
}         
         