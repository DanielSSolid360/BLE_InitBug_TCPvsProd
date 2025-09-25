	#include "variables.h"

void Test_pedir_IP_WIFI(void) //MODIFIED	
{
	char straux[64];

	if(acaboDeRecibirIP_WIFI)
	{
		strcpy((void *)straux, "AT+CIPSTA?\r\n");
		Send_RS232_ESP_ACK(500,(void *)straux);
		acaboDeRecibirIP_WIFI=false;
	}
	else if(acaboDeRecibirIP_ETH)
	{
		strcpy((void *)straux, "AT+CIPETH?\r\n");
		Send_RS232_ESP_ACK(500,(void *)straux);	
		acaboDeRecibirIP_ETH = false;
	}
	queryIP = false; 
	
}

void check_BLE_status(void) //MODIFIED*
{
	char straux[64];
	
	//BLE Connections desync watcher
	if(BLESyncTimer > SYNC_WATCH_MAX_TIME)
  {
	  numero_conexiones_BLE = 0;
		if(BLE0 == SI)numero_conexiones_BLE++;
		if(BLE1 == SI)numero_conexiones_BLE++; 
		if (numero_conexiones_BLE == 0) BLE_desconectado = SI;
	  else BLE_desconectado = NO;
		BLESyncTimer=1;
   }
	
	//BLE ADV watcher
	if((numero_conexiones_BLE < 2) && (BLECONN_checkTim > MAX_TIME_WITHOUT_ADV)) BLE_reconectar = SI;

	if (BLE_reconectar==SI)
	{ //print("THIS IS NEW FIRMWARE. OTA SUCCESFULL\r\n");
		strcpy((void *)straux, "AT+BLEADVSTART\r\n"); 
		Send_RS232_ESP_ACK(500,straux);
		
		if(ErrorEnESP)
		{ 
			ERROR_bug_counter++;
			if(ERROR_bug_counter>2){ print("ERROR BUG. RESETING SYSTEM...\r\n"); HAL_NVIC_SystemReset();}
		}
		else ERROR_bug_counter = 0;

		BLE_reconectar=NO;  //ESP-AT firmware will wait automatically for incoming connections, no need to periodically send the ADV AT command.
		BLECONN_checkTim = 1;
	}
	
	else if(ForceBLEDISCONN && (BLE0 == SI || BLE1 == SI))
	{
		ForceBLEDISCONN=false;
		if(BLE0 == SI)
		{   print("Cerrando conexión BLE0 ya que no ha enviado comandos en los últimos 30 segundos\r\n");
			  strcpy((void *)straux, "AT+BLEDISCONN=0\r\n");
			  Send_RS232_ESP_ACK(500,(void *)straux);
		}
		if(BLE1 == SI)
		{   print("Cerrando conexión BLE1 ya que no ha enviado comandos en los últimos 30 segundos\r\n");
			  strcpy((void *)straux, "AT+BLEDISCONN=1\r\n");
			  Send_RS232_ESP_ACK(500,(void *)straux);		
		}	
	}
}

void WR_en_BLE(uint8_t *data,uint8_t bytes) //MODIFIED*
{
	char straux[64];
	uint16_t w,quedan,a;
	uint8_t *paux;
	
	paux=data;
	quedan=bytes;
	
	while (quedan > 0)
	{
			if (quedan > 20) a=20;
			else a=quedan;
			
			sprintf((void *)straux, "AT+BLEGATTSIND=%c,1,7,%d\r\n",enviar_por_conexion_BLE,a); //Indicate the characteristic value from the server to a client.
			Send_RS232_ESP_ACK(500,(void *)straux);
			timer10MiliSeg=0;
			while ((Recibido_Prompt_ESP==NO)&&(timer10MiliSeg < 400)){}
			
			for (w=0; w<a ;w++)
			{
				straux[w]=*paux;
				paux++;
			}
			straux[a]=0;
			Send_RS232_ESP_ACK(500, straux);
			quedan-=a;
   }
	recibido_de_conexion_BLE=0;
}

void TCP_handler(void)
{
	char straux[64];

	if(!ESTADO_ALTA && (tengo_IP_ETH || tengo_IP_WIFI) && !TCP_SERVER_ON && TCP_RESTART_TIMER>5)
	{
		//Creating TCP server
		//strcpy((void *)straux, "AT+CWMODE=3\r\n"); //softAP + STA mode
		//Send_RS232_ESP_ACK(1000,(void *)straux);
		strcpy((void *)straux, "AT+CIPMUX=1\r\n"); //Enable multiple connections. Necessary to create the TCP server
		Send_RS232_ESP_ACK(1000,(void *)straux);
		strcpy((void *)straux, "AT+CIPSERVER=1,64500,\"TCP\"\r\n"); //Create the TCP server on the 64500 port
		Send_RS232_ESP_ACK(1000,(void *)straux);

		if(ErrorEnESP) {TCP_SERVER_ON=false; TCP_RESTART_TIMER=1;print("TCP SERVER COULDN'T BE STARTED.\r\n"); }
		else {TCP_SERVER_ON=true; TCP_RESTART_TIMER=0; print("TCP SERVER STARTED ON PORT 64500.\r\n"); }
	}
	if(ESTADO_ALTA && TCP_SERVER_ON) //Dado ya de alta, cerrar servidor TCP.
	{		
		strcpy((void *)straux, "AT+CIPSERVER=0,1\r\n"); //Delate the TCP server and close all conections
		Send_RS232_ESP_ACK(1000,(void *)straux);
		HAL_Delay(200);
		strcpy((void *)straux, "AT+CIPMUX=0\r\n");
		Send_RS232_ESP_ACK(1000,(void *)straux);

		if(ErrorEnESP) {TCP_SERVER_ON=true; ; print("TCP SERVER COULDN'T BE CLOSED.\r\n");}
		else {TCP_SERVER_ON=false;  print("TCP SERVER ON PORT 64500 CLOSED.\r\n");}

	}

}	
void WR_en_TCP( uint8_t connID, uint8_t *data, uint16_t size) //PEND*
{
	char straux[64];

	if(size>1024) 
	{
		print("Packet exceeds size limits(1024 bytes)\r\n");
	}
	else
	{
		sprintf((void *)straux, "AT+CIPSEND=%u,%u\r\n",connID,size); // Preparare TCP AT command to send a block of data
		Send_RS232_ESP_ACK(500,(void *)straux);
		
		timer10MiliSeg=0;
		while ((Recibido_Prompt_ESP==NO) && (timer10MiliSeg < 400)){}  
		*(data+size) = 0;
		Send_RS232_ESP_ACK(500,(void *)data);
	}

}

void WR_en_MQTT(uint8_t *data, uint16_t size,uint8_t respuestaAcomando) //MODIFIED
{
	char straux[64];
	uint16_t quedan;
	uint8_t *paux;
	
	if (estado_MQTT == CONECTADO) 
	{
		if (respuestaAcomando==SI) sprintf((void *)straux,"AT+MQTTPUBRAW=0,\"dt/ble10/%s/%s/\",%d,0,0\r\n",MAC_BLE,copiaUltimoCmd,size);
	  else if(respuestaAcomando==NO && !alarm_topic) sprintf((void *)straux,"AT+MQTTPUBRAW=0,\"dt/ble10/%s/ESTADO/\",%d,0,0\r\n",MAC_BLE,size);
		else if(respuestaAcomando==NO && alarm_topic) {sprintf((void *)straux,"AT+MQTTPUBRAW=0,\"dt/ble10/%s/ALARMA/\",%d,0,0\r\n",MAC_BLE,size); alarm_topic=false;}
			
		Send_RS232_ESP_ACK(500,(void *)straux);
		timer10MiliSeg=0;
		while ((Recibido_Prompt_ESP==NO)&& (timer10MiliSeg < 400)){}
		Send_RS232_ESP_Sin_Evento_HEX((void*)data, size);
		pubMqttOK = NO;
		timer10MiliSeg=0;
		while ((pubMqttOK==NO)&& (timer10MiliSeg < 400)){} //and if not what?
	}
}

/*************************************** ESP32 INIT ****************************************/

void Init_ESP32(void) //Slightly modified. PEND*
{  
	char straux[128];//delete
	char straux1[4];
	BLE0 = NO;
	BLE1 = NO;
	
	Recibido_READY_ESP32=NO;
	
	HAL_GPIO_WritePin(GPIOD, RST_ESP32_Pin, GPIO_PIN_RESET); 	//Reset ESP32
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOD, RST_ESP32_Pin, GPIO_PIN_SET); 	
	HAL_Delay(2000);

	timer10MiliSeg = 0;
	
	HAL_Delay(500);
	while (Recibido_READY_ESP32 == NO && (timer10MiliSeg < 1000)){} // Wait ESP32 ready reponse
	
	if (Recibido_READY_ESP32 == SI) print("\r\nESP32 detectado y preparado.\r\n\r\n");
	else
	{
		print("\r\nESP32 no detectado. Reiniciando...\r\n\r\n"); HAL_Delay(1000); HAL_NVIC_SystemReset(); 
	} 
	    
	if (HAL_GPIO_ReadPin(GPIOB,PULSADOR_Pin)==0) //Factory reset by switch
	{
		Inicializando=SI;
		print("\n\r\n\r !!!! PULSADOR ACCIONADO, INICIALIZO EL DISPOSITIVO �����\n\r");		
		HAL_Delay(500);
    FACTORY_RESET(); 
  }
      
	print("\r\nEsperando conexion a RED.....\r\n\r\n");
	
	HAL_Delay(500);
	
	timer10MiliSeg = 0;
	while ((ETH_conectado  == NO || WIFI_conectado == NO) && (timer10MiliSeg < 2000)) {} 

	if (ETH_conectado  == SI)
	{ 
			timer10MiliSeg = 0;
		  //sprintf((void *)straux, "AT+CIPETH=\"192.168.2.247\",\"192.168.2.1\",\"255.255.255.0\"\r\n");
		  //Send_RS232_ESP_ACK(500,(void *)straux);
			while (tengo_IP_ETH  == NO && (timer10MiliSeg < 2000)){} 
	}
	
	if (WIFI_conectado  == SI)
	{ 	//If ESP32 NVS has valid credentials stored but our stm32 flash is erased we restore the system.	
      if (nvs_vars.GOT_CREDENTIALS == 0xff || ssid[0]==0xff || password[0]==0xff) FACTORY_RESET(); 
			
		  timer10MiliSeg = 0;
			while (tengo_IP_WIFI  == NO && (timer10MiliSeg < 2000))	{}  // Espera recibir IP
			//Test_pedir_IP_WIFI();   		 				
			if (tengo_IP_WIFI == SI )nvs_vars.GOT_CREDENTIALS = SI;     //Previously saved credentials by both MCUs are valid			
	}
	
	HAL_Delay(1000);

	if (tengo_IP_ETH  == SI)
	{
		print("\r\nComo hay ETH desconectamos WIFI\r\n\r\n");
		strcpy((void *)straux, "AT+CWINIT=0\r\n");             //but later on it is activated again...
	  Send_RS232_ESP_ACK(500,(void *)straux);
	}	
	
    get_BLEMAC(true); //Needed to properly subscribe to the mqtt topic.
	
	HAL_Delay(100);
	
	if  (ETH_conectado  == SI || WIFI_conectado == SI) ConectarMQTT();

	Activar_BLE();
	
	strcpy((void *)straux, "AT+CWINIT=0\r\n"); Send_RS232_ESP_ACK(500,(void *)straux);
	
	AP_CONN_TIMEOUT = 70;
	
	if (tengo_IP_ETH  == NO && nvs_vars.GOT_CREDENTIALS == SI )
	{
		sprintf (buffer_in,".WIFIAPROV %s %s\r\n",ssid,password);
		RX_App();
	}
	else if(tengo_IP_ETH  == NO && nvs_vars.GOT_CREDENTIALS != SI) print("No hay credenciales WiFi guardadas, se cancela la conexion al AP. Esperando a recibir credenciales por BLE o consola.\r\n"); 
	
	BLECONN_checkTim = 1;
	BLESyncTimer = 1;
}
/*
** ===================================================================
**		void   Send_RS232_ESP_ACK(uint16_t espera,char *);
** ===================================================================
*/

uint8_t Send_RS232_ESP_ACK(uint16_t timeout,char *data)
{

	Recibido_OK_ESP = NO;
	Recibido_Prompt_ESP = NO;
	ErrorEnESP = NO;
	tiempoEsperaRespuesta = timeout;
	HAL_UART_Transmit(&huart1, (void *)data, strlen((void *)data), 100);
	
	
	while (tiempoEsperaRespuesta > 0)
	{
		if ((Recibido_OK_ESP == SI) || (Recibido_Prompt_ESP == SI))
		{
			HAL_Delay(50);
			return OK;
		}
		else if ((ErrorEnESP == SI))
		{
			HAL_Delay(250);
			return ERROR;
		}
	}
	return ERROR;
}

/*
** ===================================================================
**		void   Send_RS232_ESP_ACK_delayLargo(char *);
** ===================================================================
*/

char Send_RS232_ESP_ACK_delayLargo(char *que_envio)
{

	Recibido_OK_ESP = NO;
	Recibido_Prompt_ESP = NO;
	ErrorEnESP = NO;
	tiempoEsperaRespuestaLargo = 20;
	HAL_UART_Transmit(&huart1, (void *)que_envio, strlen((void *)que_envio), 100);
	while (1)
	{
		if (Recibido_OK_ESP == SI)
		{
			HAL_Delay(50);
			return OK;
		}
		else if ((ErrorEnESP == SI) || (tiempoEsperaRespuestaLargo == 0))
		{
			HAL_Delay(250);
			return ERROR;
		}
	}
}
/*
** ===================================================================
**  void   Send_RS232_ESP_Sin_Evento_HEX(char  *que_envio,int cuantos);
** ===================================================================
*/

void Send_RS232_ESP_Sin_Evento_HEX(char *data, int size)
{
	HAL_UART_Transmit(&huart1, (void *)data, size, 100);
	HAL_Delay(50);
}
/*
** ===================================================================
**		void   Send_RS232_ESP_Sin_Evento_HEX(char *);
** ===================================================================
*/

void Send_RS232_ESP_Sin_Evento(char *que_envio)
{
	HAL_UART_Transmit(&huart1, (void *)que_envio, strlen((void *)que_envio), 100);
	HAL_Delay(250);
}

void Activar_BLE()
{
	char straux[256];
	
	print ("Configurando BLE.....\r\n");
	
	strcpy((void *)straux, "AT+CWINIT=0\r\n"); Send_RS232_ESP_ACK(500,(void *)straux);	
	HAL_Delay(100);

	strcpy((void *)straux, "AT+CWINIT=1\r\n"); Send_RS232_ESP_ACK(500,(void *)straux);	
	HAL_Delay(100);
	//strcpy((void *)straux, "AT+CWMODE=1\r\n"); Send_RS232_ESP_ACK(500,(void *)straux);
	//HAL_Delay(100);

	timer10MiliSeg = 0;
	Recibido_READY_ESP32=NO;
	strcpy((void *)straux, "AT+BLEINIT=2\r\n");
	Send_RS232_ESP_ACK(300,(void *)straux);

	if ((ErrorEnESP)||(tiempoEsperaRespuesta==0)||(Recibido_READY_ESP32))// Reseting system due to BLE init 'ready' error
	{
		BLE_bug_counter++;
		EspejoFlash[2] = BLE_bug_counter;
		Escribe_en_flash (EspejoFlash,0,0x200);
		
		if(BLE_bug_counter >= 2) // Reiniciar y resetear por completo el ESP32 para solucionar el problema de que arroja "ready" cada vez que 
		{
			BLE_bug_counter = 0; EspejoFlash[2] = BLE_bug_counter;
			Escribe_en_flash (EspejoFlash,0,0x200);
			ESP_RESET();	
		} 
		else NVIC_SystemReset();
	}	
	BLE_bug_counter = 0; EspejoFlash[2] = BLE_bug_counter;
	Escribe_en_flash (EspejoFlash,0,0x200);
	HAL_Delay(500);
	
	//strcpy((void *)straux, "AT+BLEADDR?\r\n");
	//Send_RS232_ESP_ACK(500,(void *)straux);
        
	if (EspejoFlash[0]) sprintf((void *)straux, "AT+BLEADVDATAEX=\"Ares %s BLE10 B\",\"A002\",\"31323334\",1\r\n", MAC_BLE);
	else sprintf((void *)straux, "AT+BLEADVDATAEX=\"Ares %s BLE10 A\",\"A002\",\"31323334\",1\r\n", MAC_BLE);
	Send_RS232_ESP_ACK(500,(void *)straux);
        
	strcpy((void *)straux, "AT+BLEGATTSSRVCRE\r\n");
	Send_RS232_ESP_ACK(500,(void *)straux);
	
	strcpy((void *)straux, "AT+BLEGATTSSRVSTART\r\n");
	Send_RS232_ESP_ACK(500,(void *)straux);
	
	strcpy((void *)straux, "AT+BLEADVSTART\r\n");
	Send_RS232_ESP_ACK(500,(void *)straux);

	print ("Configuracion BLE, OK\r\n");
	HAL_Delay(250);
}

void get_BLEMAC(bool ESP32_INIT) //Created
{
	char straux[18];
	HAL_Delay(250);
	
  if(ESP32_INIT) 
	{
		strcpy((void *)straux, "AT+BLEINIT=2\r\n");
		Send_RS232_ESP_ACK(500,(void *)straux);
		if (ErrorEnESP) NVIC_SystemReset();	
	}
	//During normal program execution we just call this command 
	strcpy((void *)straux, "AT+BLEADDR?\r\n");
	Send_RS232_ESP_ACK(500,(void *)straux);	
	
	if(ESP32_INIT)
	{
		strcpy((void *)straux, "AT+BLEINIT=0\r\n"); //Deactivates BLE temporaly so MQTT can be set up.
		Send_RS232_ESP_ACK(500,(void *)straux);
	}
}

void ConectarMQTT()
{
	char straux[128];
	char estado_MQTT_BAK,reintentos;
	
	conexionMqttAntesIntentada=true;
	timer10MiliSeg=0;
	while (	tengo_IP_ETH  == NO && tengo_IP_WIFI == NO && (timer10MiliSeg <400)){}
	if (timer10MiliSeg >=400) print ("No Hay RED se cancela la conexion a MQTT.....\r\n");
	else 
	{
		print ("Configurando MQTT.....\r\n");
		HAL_Delay(250);
		
		char uuid[30];
		strcpy(uuid,MAC_BLE);
		if(!ESTADO_ALTA) strcat(uuid,"_BAJA");

		sprintf((void *)straux, "AT+MQTTUSERCFG=0,1,\"ble10_%s\",\"\",\"\",0,0,\"\"\r\n",uuid);
		Send_RS232_ESP_ACK(500,(void *)straux);
        	
		sprintf((void *)straux, "AT+MQTTCONNCFG=0,60,0,\"lwt/ble10/%s/\",\"%s_DESCONECTADO\",0,0\r\n",uuid,uuid);
		Send_RS232_ESP_ACK(500,(void *)straux);
        	
		HAL_Delay(250);
		
		reintentos=0;
		estado_MQTT = INTENTANDO_CONECTAR;
		
        	
      while (reintentos<5)       
			{
				strcpy((void *)straux, "AT+MQTTCONN=0,\"mqtt.boxbosses.com\",1883,1\r\n"); 
				Send_RS232_ESP_ACK(2000,(void *)straux);
							
				timer10MiliSeg=0;
				while ((estado_MQTT == INTENTANDO_CONECTAR)&& (timer10MiliSeg < 1000)){}		
				estado_MQTT_BAK=estado_MQTT;
						
				if (estado_MQTT == CONECTADO) 
				{
					HAL_Delay(500);
					
					sprintf((void *)straux, "AT+MQTTSUB=0,\"cmd/ble10/%s/\",0\r\n",MAC_BLE);
					Send_RS232_ESP_ACK(1000,(void *)straux);
							
					print ("Configuracion MQTT, OK\r\n");
					break;
				}
				//else if (estado_MQTT == DESCONECTADO) 
				else
				{
						reintentos++;
						sprintf (straux,"Fallo n�%d de 5\r\n",reintentos);
						print (straux);			
				}
      }
			if(estado_MQTT == INTENTANDO_CONECTAR) estado_MQTT = DESCONECTADO; //
	}
	mqttReconTimer = 1;
	perroMain=0;
}		

void EsperaIPWifi()
{
	timer10MiliSeg=0;
	while ((tengo_IP_WIFI==NO)&& (timer10MiliSeg < AP_CONN_TIMEOUT*100)){}
}

/***Restore factory default settings of the ESP32 module and erase STM32 flash***/
void FACTORY_RESET()
{
		
	Recibido_READY_ESP32=NO;
	strcpy((void *)copiaUltimoCmd, "AT+RESTORE\r\n");
	Send_RS232_ESP_ACK(500,(void *)copiaUltimoCmd);
	timer10MiliSeg = 0;
	
	HAL_Delay(500);
	while (Recibido_READY_ESP32 == NO && (timer10MiliSeg < 4000)){} // Wait ESP32 ready reponse
		
	memset (EspejoFlash,0xff,0x200);
	Escribe_en_flash ((void*)EspejoFlash,0,0x200);
	NVIC_SystemReset();
}

/***Restore factory default settings of the ESP32 module***/
void ESP_RESET()
{
		
	Recibido_READY_ESP32=NO;
	strcpy((void *)copiaUltimoCmd, "AT+RESTORE\r\n");
	Send_RS232_ESP_ACK(500,(void *)copiaUltimoCmd);
	timer10MiliSeg = 0;
	
	HAL_Delay(500);
	while (Recibido_READY_ESP32 == NO && (timer10MiliSeg < 4000)){} // Wait ESP32 ready reponse
	if(Recibido_READY_ESP32 == NO) print("ERROR DURING ESP32 MODULE RESTORE");
	NVIC_SystemReset();
}
void TestReconexionMQTT()
{
	char straux[128];
	
	if ((estado_MQTT == DESCONECTADO)&& (tengo_IP_WIFI  == SI))
		{ 
		ConectarMQTT();
		strcpy((void *)straux, "AT+CWINIT=0\r\n"); 
		Send_RS232_ESP_ACK(500,(void *)straux);
		strcpy((void *)straux, "AT+CWINIT=1\r\n"); 
		Send_RS232_ESP_ACK(500,(void *)straux);
		Activar_BLE();
		sprintf (buffer_in,".WIFIAPROV %s %s\r\n",ssid,password);
		RX_App();
		}

}