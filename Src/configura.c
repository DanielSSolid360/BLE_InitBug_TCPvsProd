#include <ctype.h>
#include "variables.h"


void ORDEN(char *orden)
{
	strcpy((void *)buffer_in, orden);
	RX_App(strlen((void *)buffer_in), 0);
}

/*************************************************************************
uint16_t RX_App()
 *************************************************************************/

uint16_t RX_App()
{

	byte a,b,i,x;
	byte *paux;

	uint32_t origen;
	uint32_t destino;
	
	uint16_t w,z;

	byte entrada[256];
	byte straux[128];
	
	byte param1[64];
	byte param2[64];
	byte param3[128];
	byte param4[64];
	byte param5[64];


	param5[0] = 0;
	param4[0] = 0;
	param3[0] = 0;
	param2[0] = 0;
	param1[0] = 0;

	a = sscanf(buffer_in, "%s %s %s %s %s", param1, param2, param3, param4, param5);

	paux = buffer_in + strlen((void *)param1);

	if ((buffer_in[0] == '.') || (buffer_in[0] == '{'))
	{
		for (b=0;b<(strlen((void *)param1));b++) param1[b]=toupper(param1[b]);

		//-------------------------------------------------------------------------------------------------------
		// Comando .VERsion
		//-------------------------------------------------------------------------------------------------------

		if (!memcmp(param1, ".VER", 4))

		{
			Pinta_VERSION();
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .ALTa
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".ALT", 4))

		{
			if (EspejoFlash[0])		//== 0-> ya dado de Alta, !=0 -> no se ha dado de alta 
			{				
				EspejoFlash[0]=0;
				sprintf((void *)buffer_out, "^ALTA OK\r\n");	
				strcat((void *)buffer_out,MAC_BLE);
				strcat((void *)buffer_out,"\r\nBLE10\r\n");
				sprintf (param2,"%02d.%02d.%02d\r\n$", SW_VERSION, SW_REVISION, SW_COMPILATION);
				strcat((void *)buffer_out,param2);
				Escribe_en_flash (EspejoFlash,0,0x200);

				sprintf((void *)straux, "AT+BLEADVDATAEX=\"Ares %s BLE10 A\",\"A002\",\"31323334\",1\r\n", MAC_BLE);
				Send_RS232_ESP_ACK(500,(void *)straux);
				
				ESTADO_ALTA = true;
				sprintf((void *)straux,"AT+MQTTCLEAN=0\r\n"); 
				Send_RS232_ESP_ACK(500,(void *)straux);
				ConectarMQTT();	
			}
			else
			{
				strcpy((void *)buffer_out,"^ALTA KO: EL DISPOSITIVO YA ESTA DADO DE ALTA\r\n$"); 
			}

		}
		//-------------------------------------------------------------------------------------------------------
		// Comando BAJa
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".BAJ", 4))

		{
			if (EspejoFlash[0]==0)		//== 0-> ya dado de Alta, !=0 -> no se ha dado de alta 
			{				
				EspejoFlash[0]=0xFF;
				sprintf((void *)buffer_out, "^BAJA OK\r\n");	
				strcat((void *)buffer_out,MAC_BLE);
				strcat((void *)buffer_out,"\r\nBLE10\r\n");
				sprintf (param2,"%02d.%02d.%02d\r\n$", SW_VERSION, SW_REVISION, SW_COMPILATION);
				strcat((void *)buffer_out,param2);
				Escribe_en_flash (EspejoFlash,0,0x200);

				sprintf((void *)straux, "AT+BLEADVDATAEX=\"Ares %s BLE10 B\",\"A002\",\"31323334\",1\r\n", MAC_BLE);
				Send_RS232_ESP_ACK(500,(void *)straux);
				ESTADO_ALTA = false;
				sprintf((void *)straux,"AT+MQTTCLEAN=0\r\n"); 
				Send_RS232_ESP_ACK(500,(void *)straux);
				ConectarMQTT();
				TCP_RESTART_TIMER = 1;
			}
			else
			{
				strcpy((void *)buffer_out,"^BAJA KO: EL DISPOSITIVO YA ESTA DADO DE BAJA\r\n$"); 
			}
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando COP
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".COP", 4))
		{
			origen=0x801e000;
			
			for (w=0;w<0x1000;w++)
				{
				buffer_aligned[w] = *(__IO uint8_t *)origen;
				origen++;
				}			
	
			if (buffer_aligned[0x181]==0x7d) 
				{
				print ("Fallo arreglado\r\n");
				buffer_aligned[0x181]=0xfd;
				}
			prg_flash(0x801E000, 0x200, buffer_aligned);
			strcpy((void *)buffer_out,"Copiado\r\n"); 
			
		}
		else if (!memcmp(param1, ".TCP",4))
		{
			if(param2[0]=='O' || param2[0]=='o')
			{
				strcpy((void *)straux, "AT+CIPMUX=1\r\n"); //Enable multiple connections. Necessary to create the TCP server
				Send_RS232_ESP_ACK(1000,(void *)straux);
				strcpy((void *)straux, "AT+CIPSERVER=1,64500,\"TCP\"\r\n"); //Create the TCP server on the 64500 port
				Send_RS232_ESP_ACK(1000,(void *)straux);

				if(ErrorEnESP) {TCP_SERVER_ON=false; ; strcpy((void *)buffer_out, "TCP SERVER COULDN'T BE STARTED.\r\n");}
				else {TCP_SERVER_ON=true;  strcpy((void *)buffer_out, "TCP SERVER STARTED ON PORT 64500.\r\n");}
			}
			else if(param2[0]=='K' || param2[0]=='k')
			{
				strcpy((void *)straux, "AT+CIPSERVER=0,1\r\n"); //Delate the TCP server and close all conections
				Send_RS232_ESP_ACK(1000,(void *)straux);
				HAL_Delay(200);
				strcpy((void *)straux, "AT+CIPMUX=0\r\n");
				Send_RS232_ESP_ACK(1000,(void *)straux);

				if(ErrorEnESP) {TCP_SERVER_ON=true; ; strcpy((void *)buffer_out, "TCP SERVER COULDN'T BE CLOSED.\r\n");}
				else {TCP_SERVER_ON=false;  strcpy((void *)buffer_out, "TCP SERVER ON PORT 64500 CLOSED.\r\n");}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando CMP
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".CMP", 4))
		{

			origen=0x801e000;
			destino=0x801c000;
			
			for (w=0;w<0x1000;w++)
				{
				x = *(__IO uint8_t *)origen;
				i = *(__IO uint8_t *)destino;
				if (x!=i) 
					{
					sprintf((void *)buffer_out,"Diferencia encontrada en %X (%X,%X)\r\n",destino,x,i);
					print (buffer_out);
					break;
					}
				origen++;
				destino++;
				}
			
			sprintf((void *)buffer_out,"Fin de comparaci�n\r\n");
			
			
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando DEScarga
		//-------------------------------------------------------------------------------------------------------
   
		else if (!memcmp(param1, ".DESC", 5))
		{
			char numeroFichero,limite,n_packets,parts;
			uint16_t CRCSize,packets_size;
			uint32_t offset,laux;
			char nombreFichero[32];
			
			if (!memcmp(param2, "BOOT", 4))  
			{
				strcpy(nombreFichero,"BOOT_BLE10");
				parts=1;
				CRCSize=4094;
				packets_size = 4096;
			}
			else 
			{
				strcpy (nombreFichero,"FW_BLE10");
				parts=8;
				CRCSize=8190;
				packets_size = 8192; // This is measure as 4096 bytes units. For instance, FW packets are 8KB(8192 bytes) of size, so 8192/4096 = 2
			}
			
			crc_16 = 0xffff;
			offset=0;
			numeroFichero=0;
			n_packets = packets_size / 4096; //Being 4096 bytes the minimum segment to write on the flash.
			
			/*sprintf((void*)straux, "AT+FSMOUNT=1\r\n");
			Send_RS232_ESP_ACK(500,straux);
			HAL_Delay(250);*/
			
			for (x=0;x<parts;x++)
			{
					Indice_A_buffer_aligned=0;
				  //sprintf((void *)straux, "AT+HTTPCLIENT=2,0,\"https://archive.boxbosses.com/firmwares/ble10/prueba_512bytes.txt\",,,2\r\n");
				  //sprintf((void *)straux, "AT+HTTPCLIENT=2,0,\"https://archive.boxbosses.com/firmwares/ble10/%s_%02d.BIN\",,,2\r\n",nombreFichero, numeroFichero);
				  sprintf((void *)straux, "AT+HTTPCLIENT=2,0,\"https://archive.boxbosses.com/firmwares/ble10/%s_%02d.BIN?mac=%s\",,,2\r\n",nombreFichero, numeroFichero,MAC_BLE);
					Send_RS232_ESP_ACK_delayLargo((void *)straux);
 				  HAL_Delay(500);
				
				  if (ErrorEnESP == SI) {print(" ERROR DURING HTTPS DOWNLOAD\r\n"); break;}
					laux=0;
					
					for (a=0;a<n_packets;a++)
					{
							sprintf((void *)straux, "AT+FS=0,1,\"%s.BIN\",%d,4096\r\n",nombreFichero,offset);
							Recibido_OK_ESP = NO;
					  	Send_RS232_ESP_ACK(500,(void *)straux);
							HAL_Delay(250);
							print("Transmitiendo....\r\n");
							HAL_UART_Transmit(&huart1, buffer_aligned+laux, 4096, 1000);	//10 segundos 1000 * 0.01 
							tiempoEsperaRespuesta = 1000; 
              while ((Recibido_OK_ESP == NO) && (tiempoEsperaRespuesta!=0)){}
							if (tiempoEsperaRespuesta==0){ ErrorEnESP = SI; break;}
							laux+=4096;
							offset+=4096;
					}
					if (x==(parts-1))  //When reached last 8KB packet
					{
							crc_16 = crc16(buffer_aligned, CRCSize, crc_16);
							crc_16_LEIDO = (buffer_aligned[CRCSize+1]*256)+buffer_aligned[CRCSize]; //Multiply by 256 equals to >> 8 bits. TamanoCR + 1 = byte con la parte alta del CRC, tamanoCrc = byte parte baja
					}
					else crc_16 = crc16(buffer_aligned, CRCSize+2, crc_16); //Intermediate packets*/
					if (ErrorEnESP == SI) {sprintf(straux,"ESP ERROR DURING FS TRANSFERENCE NUMBER %d\r\nlaux = %d\r\n",a,laux); print(straux); break;}
					numeroFichero++;
					HAL_Delay(200);
			}
			//Suggested by Espressif after doing FileSystem operations
		  sprintf((void*)straux, "AT+FSMOUNT=0\r\n");
			Send_RS232_ESP_ACK(500,straux);
			if (ErrorEnESP == SI) strcat((void *)buffer_out, "ERROR en ESP32 al realizar la descarga.\r\n");
			else 
			{ 
				sprintf((void *)buffer_out, "Descarga realizada. (CRC calculado=%04X, CRC leido=%04X)",crc_16,crc_16_LEIDO);
				if (crc_16 == crc_16_LEIDO)
				{
						strcat ((void *)buffer_out," DESCARGA OK\r\n");
						descargaOK=SI;
				}
				else
				{
					strcat ((void *)buffer_out," ERROR EN DESCARGA, CRC NO COINCIDE !!!!!\r\n");
					descargaOK=NO;
				}				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando .SSId
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".SSI", 4))

		{
			strcpy (ssid,param2);
			sprintf((void *)buffer_out, "SSID .- %s\r\n",ssid);
			//GOT_SSID = true;
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando .PASsword
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".PAS", 4))

		{
			strcpy ((char*)password,(char*)param2);
			sprintf((void *)buffer_out, "Password .- %s\r\n",password);
			//GOT_PASS = true;
		}


		//-------------------------------------------------------------------------------------------------------
		// Comando .DESconectaAP
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".DES", 4))

		{
			sprintf((void *)straux, "AT+CWQAP\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			strcpy((void *)buffer_out, "Desconectado a la AP\r\n");
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .CONectaAP
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".CON", 4))

		{
			strcpy((void *)straux, "AT+CWMODE=1\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			tengo_IP_WIFI=NO;
			sprintf((void *)straux, "AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
			Send_RS232_ESP_ACK(500,(void *)straux);
			EsperaIPWifi();
			strcpy((void *)buffer_out, "Conectado a la AP\r\n");
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando FYH
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".FYH", 4))
		{
			sprintf((void *)buffer_out,"Hora Actual %02d:%02d:%02d: %02d-%02d-%04d\r\n",hora,minuto,segundo,dia,mes,year);		
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .WIFIAPROV
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".WIFIAPROV", 6))  //MODIFIED
    {
			if (tengo_IP_ETH  == SI )
			{
					strcpy((void *)straux, "AT+BLEINIT=0\r\n");
					Send_RS232_ESP_ACK(500,(void *)straux);
				
					connect2wifi(straux,(void*)param2,(void*)param3);						
					Activar_BLE();
			}
			else
			{   
					connect2wifi(straux,(void*)param2,(void*)param3);		
					HAL_Delay(500);
			}
				
			if(!conexionMqttAntesIntentada)
			{
				   pending_wifi_con = true; //Init_ESP32();	
			}
			
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando .FACTORY_RESET
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".FACTORY_RESET", 14))

		{
			FACTORY_RESET();
		}
		
		//-------------------------------------------------------------------------------------------------------
		// Comando .MQTT
		//-------------------------------------------------------------------------------------------------------
		else if (!memcmp(param1, ".MQT", 4))

		{
			if (estado_MQTT == CONECTADO) strcpy((void *)buffer_out, "Conectado al MQTT\r\n");
			else if (estado_MQTT == DESCONECTADO) strcpy((void *)buffer_out, "Desconectado al MQTT\r\n");
			else if (estado_MQTT == INTENTANDO_CONECTAR) strcpy((void *)buffer_out, "Conectando al MQTT....\r\n");
			else strcpy((void *)buffer_out, "Estado indeterminado !!!!!\r\n");
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .INFO
		//-------------------------------------------------------------------------------------------------------
		else if (!memcmp(param1, ".INFO", 6))

		{	
			strcpy((void *)buffer_out, "^BLE10\r\n");	
			sprintf (param2,"%02d.%02d.%02d\r\n", SW_VERSION, SW_REVISION, SW_COMPILATION);
			strcat((void *)buffer_out,param2);
			strcat((void *)buffer_out,MAC_BLE);
			strcat((void *)buffer_out,"\r\n");

			if (WIFI_conectado)
			{
				strcpy((void *)straux, "AT+CIPSTA?\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);
				HAL_Delay(100);
				strcat((void *)buffer_out,IP_WIFI);
			}
			else strcat((void *)buffer_out,"-");
			strcat((void *)buffer_out,"\r\n");
			if (ETH_conectado) 
			{
				strcpy((void *)straux, "AT+CIPETH?\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);	
				HAL_Delay(300);
				strcat((void *)buffer_out,IP_ETH);
			}
			else strcat((void *)buffer_out,"\r\n-");
			
			if (!WIFI_conectado && !ETH_conectado) strcat((void *)buffer_out,"\r\nNO CONEXION");
			else  strcat((void *)buffer_out,"\r\nCONEXION OK\r\n");
			if ((ssid[0] == 0xff || ssid[0] == 0x0) && nvs_vars.GOT_CREDENTIALS != SI ) strcat((void *)buffer_out,"- -");
			else 
			{
				sprintf (straux,"%s ",ssid);
				strcat((void *)buffer_out,straux);
				strcat((void *)buffer_out,password);
			}
			if(WIFI_conectado || ETH_conectado) 
			{
				HAL_Delay(100);
				Send_RS232_ESP_ACK(500,"AT+CWDHCP?\r\n"); 
				HAL_Delay(100);
				if (DHCP_WIFI && WIFI_conectado) strcat((void *)buffer_out,"\r\nWIFI: DHCP");
				else if (!DHCP_WIFI && WIFI_conectado) strcat((void *)buffer_out,"\r\nWIFI: STATIC IP");
				if (DHCP_ETH && ETH_conectado) strcat((void *)buffer_out,"\r\nETH: DHCP");
				else if(!DHCP_ETH && ETH_conectado) strcat((void *)buffer_out,"\r\nETH: STATIC IP");

				/*Send_RS232_ESP_ACK(800,"AT+PING=\"www.google.es\"\r\n");
				char strP[15];
				sprintf(strP, "\r\nPING: %d",pingData);
				strcat((void*)buffer_out,strP);  */
			}

			if(ESTADO_ALTA)strcat((void*)buffer_out,"\r\nESTADO: ALTA");
			else strcat((void*)buffer_out,"\r\nESTADO: BAJA");

			strcat((void *)buffer_out,"\r\n$");	
		}
		//-------------------------------------------------------------------------------------------------------
		// Comando .LIBerar
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".LIB", 4))

		{
			if (param2[0]=='0')
				{
				strcpy((void *)straux, "AT+BLEDISCONN=0\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);
				strcpy((void *)buffer_out, "Liberado CANAL 0\r\n");
				}
			else if (param2[0]=='1')
				{
				strcpy((void *)straux, "AT+BLEDISCONN=1\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);
				strcpy((void *)buffer_out, "Liberado CANAL 1\r\n");
				}
			else if (param2[0]=='2')
				{
				strcpy((void *)straux, "AT+BLEDISCONN=0\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);
				strcpy((void *)straux, "AT+BLEDISCONN=1\r\n");
				Send_RS232_ESP_ACK(500,(void *)straux);
				strcpy((void *)buffer_out, "Liberado CANAL 0 y 1\r\n");

				}
			else 	strcpy((void *)buffer_out, "ERROR de Sintaxis .LIBERA 0/1/2\r\n");
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .ABRe
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".ABR", 4))
		{
			if ((param2[0]=='1')&&(param2[1]!='0'))
			{		
					HAL_GPIO_WritePin(RL1_GPIO_Port, RL1_Pin, GPIO_PIN_SET);
					Last_RL1_Op = 0;  RL1_monitorTimer = 1;  RL1_opening_lock_timer = 1;		
					last_RL1_Event = timeAmongEvents + 1;
				  openedGate1Watcher = 0;
					strcpy ((void *)buffer_out, "^Cerradura 1, abriendo\r\n$");				
			}
			else if (param2[0]=='2')
		  {		
					HAL_GPIO_WritePin(RL2_GPIO_Port, RL2_Pin, GPIO_PIN_SET);
					Last_RL2_Op = 0;  RL2_monitorTimer = 1;  RL2_opening_lock_timer = 1;		
					last_RL2_Event = timeAmongEvents + 1;
				  openedGate2Watcher = 0;
				  strcpy((void *)buffer_out, "^Cerradura 2, abriendo\r\n$");	
				
		  }
			else if (param2[0]=='3')
			{		
					HAL_GPIO_WritePin(RL3_GPIO_Port, RL3_Pin, GPIO_PIN_SET);
					Last_RL3_Op = 0;  RL3_monitorTimer = 1;  RL3_opening_lock_timer = 1;		
					last_RL3_Event = timeAmongEvents + 1;
					openedGate3Watcher = 0;
					strcpy((void *)buffer_out, "^Cerradura 3, abriendo\r\n$");	
		
			}
			else if (param2[0]=='4')
			{		
					HAL_GPIO_WritePin(RL4_GPIO_Port, RL4_Pin, GPIO_PIN_SET);
					Last_RL4_Op = 0;  RL4_monitorTimer = 1;  RL4_opening_lock_timer = 1;		
					last_RL4_Event = timeAmongEvents + 1;
					openedGate4Watcher = 0;	
					strcpy((void *)buffer_out, "^Cerradura 4, abriendo\r\n$");	
			}
			else if (param2[0]=='5')
			{		
					HAL_GPIO_WritePin(RL5_GPIO_Port, RL5_Pin, GPIO_PIN_SET);
					Last_RL5_Op = 0;  RL5_monitorTimer = 1;  RL5_opening_lock_timer = 1;		
					last_RL5_Event = timeAmongEvents + 1;
					openedGate5Watcher = 0;	
					strcpy((void *)buffer_out, "^Cerradura 5, abriendo\r\n$");
					
			}
			else if (param2[0]=='6')
			{		
					HAL_GPIO_WritePin(RL6_GPIO_Port, RL6_Pin, GPIO_PIN_SET);
					Last_RL6_Op = 0;  RL6_monitorTimer = 1;  RL6_opening_lock_timer = 1;		
					last_RL6_Event = timeAmongEvents + 1;
					openedGate6Watcher = 0;	
					strcpy((void *)buffer_out, "^Cerradura 6, abriendo\r\n$");	
				
			}
			else if (param2[0]=='7')
			{		
					HAL_GPIO_WritePin(RL7_GPIO_Port, RL7_Pin, GPIO_PIN_SET);
					Last_RL7_Op = 0;  RL7_monitorTimer = 1;  RL7_opening_lock_timer = 1;		
					last_RL7_Event = timeAmongEvents + 1;
				  openedGate7Watcher = 0;	 
					strcpy((void *)buffer_out, "^Cerradura 7, abriendo\r\n$");	
			}
			else if (param2[0]=='8')
			{		
					HAL_GPIO_WritePin(RL8_GPIO_Port, RL8_Pin, GPIO_PIN_SET);
					Last_RL8_Op = 0;  RL8_monitorTimer = 1;  RL8_opening_lock_timer = 1;		
					last_RL8_Event = timeAmongEvents + 1;
				  openedGate8Watcher = 0;	
					strcpy((void *)buffer_out, "^Cerradura 8, abriendo\r\n$");	
			}
			else if (param2[0]=='9')
			{		
					HAL_GPIO_WritePin(RL9_GPIO_Port, RL9_Pin, GPIO_PIN_SET);
					Last_RL9_Op = 0;  
					RL9_monitorTimer = 1;  RL9_opening_lock_timer = 1; last_RL9_Event = timeAmongEvents + 1;
				  openedGate9Watcher = 0;	
					strcpy((void *)buffer_out, "^Cerradura 9, abriendo\r\n$");	
			}
			else if ((param2[0]=='1') && (param2[1]=='0'))
			{		
					HAL_GPIO_WritePin(RL10_GPIO_Port, RL10_Pin, GPIO_PIN_SET);
					Last_RL10_Op = 0;  RL10_monitorTimer = 1;  RL10_opening_lock_timer = 1;		
					last_RL10_Event = timeAmongEvents + 1;
				  openedGate10Watcher = 0;	
					strcpy((void *)buffer_out, "^Cerradura 10, abriendo\r\n$");	
			}
			else strcpy((void *)buffer_out, "^ERROR nivel 3 (.ABRe 1..10\r\n$");
					
			param2[1]=0;

		}
		
		//-------------------------------------------------------------------------------------------------------
		// Comando .ESTado (pulsador,ct1,ct2
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".EST", 4))

		{
			if (!memcmp(param2, "PUL", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOB,PULSADOR_Pin)==0) strcpy((void *)buffer_out, "^PULSADOR.- pulsado\r\n$");
				else strcpy((void *)buffer_out, "^PULSADOR.- NO pulsado\r\n$");
				}
			else if (!memcmp(param2, "CT1", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOB,CT1_Pin)==0) strcpy((void *)buffer_out, "^CT1.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT1.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT2", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOB,CT2_Pin)==0) strcpy((void *)buffer_out, "^CT2.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT2.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT3", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOA,CT3_Pin)==0) strcpy((void *)buffer_out, "^CT3.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT3.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT4", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOB,CT4_Pin)==0) strcpy((void *)buffer_out, "^CT4.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT4.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT5", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOA,CT5_Pin)==0) strcpy((void *)buffer_out, "^CT5.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT5.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT6", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOA,CT6_Pin)==0) strcpy((void *)buffer_out, "^CT6.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT6.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT7", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOA,CT7_Pin)==0) strcpy((void *)buffer_out, "^CT7.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT7.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT8", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOC,CT8_Pin)==0) strcpy((void *)buffer_out, "^CT8.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT8.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT9", 3))
				{		
				if (HAL_GPIO_ReadPin(GPIOB,CT9_Pin)==0) strcpy((void *)buffer_out, "^CT9.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT9.- Abierto\r\n$");
				}
			else if (!memcmp(param2, "CT10", 4))
				{		
				if (HAL_GPIO_ReadPin(GPIOD,CT10_Pin)==0) strcpy((void *)buffer_out, "^CT10.- Cerrado\r\n$");
				else strcpy((void *)buffer_out, "^CT10.- Abierto\r\n$");
				}
			else strcpy((void *)buffer_out, "^ERROR nivel 3 (.ESTado PULSADOR/CT1/.../CT10\r\n$");

		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .ACTUALIZA
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".ACTUALIZA", 10))

		{
			pending_download = true;
			strcpy((void *)buffer_out, "Update request recieved.\r\n");
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .UPGRADE
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".UPGRADE", 8))
		{
			strcpy((void *)straux, "ATE0\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			HAL_Delay(1000);

			AA_Hazupgrade((uint32_t)&huart2,(uint32_t)&huart1,(uint32_t)&hiwdg);
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .RESet
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".RES", 4))

		{
			NVIC_SystemReset();
		}

		else if(!memcmp(param1, ".TCPSEND",8))
		{
			
			sprintf((void *)straux, "AT+CIPSEND=%u\r\n",strlen(param2));
			uint8_t ans = Send_RS232_ESP_ACK(1200,(void *)straux);
			//strcat(param3,"\r\n");
			if(ans==OK) 
			{
				Send_RS232_ESP_ACK(500,param2); 
				sprintf((void *)buffer_out,"DATA SENT TO TCP SERVER: %s\r\n", param2);
			}
			else sprintf((void *)buffer_out,"Error sending TCP data\r\n");
		}

		//-------------------------------------------------------------------------------------------------------
		// Comando .ESP SEND
		//-------------------------------------------------------------------------------------------------------

		else if (!memcmp(param1, ".ESP", 4))
		{

			if (!memcmp(param2, "SEND", 4))
			{
				paux = buffer_in + strlen((void *)param1) + strlen((void *)param2) + 2;

				for (i = 0;; i++)
				{
					entrada[i] = *paux;
					paux++;
					if (entrada[i] == 0xd)
					{
						entrada[i + 1] = 0xA;
						entrada[i + 2] = 0;
						break;
					}
				}

				strcpy((void *)BufferTX_ESP, (void *)entrada);
				mqtt_log = true;
				pInd = 0;
				memset(pData,0,sizeof(pData));

				Send_RS232_ESP_ACK(4000,(void *)BufferTX_ESP);
				
				HAL_Delay(500);

				sprintf((void *)buffer_out, "\r\nEnviado al ESP: %s\r\n", BufferTX_ESP);
				strcat(buffer_out, "Recibido del ESP: ");
				strcat((void *)buffer_out, pData);

				mqtt_log = false;
			}
			else if (!memcmp(param2, ".RST", 4))
			{
				Init_ESP32();
				strcpy((void *)buffer_out, "ESP32 Reset OK\r\n");
			}
			else
			{
				strcpy((void *)buffer_out, "ERROR nivel 2 \r\n");
			}
		}

		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------

		else
		{
			strcpy((void *)buffer_out, "ERROR Nivel 2\r\n");
		}
	}
	else
	{
		strcpy((void *)buffer_out, "ERROR Nivel 1\r\n");
	}

	a = strlen((void *)buffer_out);
	return (a);
}


/*************************************************************************
                 WIFI CONNECTION
*************************************************************************/

void connect2wifi(uint8_t *straux, char *param2, char *param3)//Created
{  
	strcpy((void *)straux, "AT+CWINIT=1\r\n");
	Send_RS232_ESP_ACK(1000,(void *)straux);
	HAL_Delay(150);
	strcpy((void *)straux, "AT+CWMODE=1\r\n");
	Send_RS232_ESP_ACK(500,(void *)straux);
	HAL_Delay(250);								
	WIFI_conectado = NO;
	error_WIFI = NO;
	tengo_IP_WIFI=NO;
	
 //Comando: AT+CWJAP=[<ssid>],[<pwd>][,<bssid>][,<pci_en>][,<reconn_interval>][,<listen_interval>][,<scan_mode>][,<jap_timeout>][,<pmf>]
	sprintf((void *)straux, "AT+CWJAP=\"%s\",\"%s\",,,,,,%u,\r\n",param2,param3,AP_CONN_TIMEOUT);
	Send_RS232_ESP_ACK(500,(void *)straux);
	EsperaIPWifi();
	
	//while ((WIFI_conectado == NO) && (error_WIFI == NO)){}
	if (WIFI_conectado == SI)
	{
		//sprintf((void *)straux, "AT+CIPSTA=\"192.168.1.98\",\"192.168.1.1\",\"255.255.255.0\"\r\n");
		//Send_RS232_ESP_ACK(500,(void *)straux);
		strcpy((void *)buffer_out,"^WIFIAPROV OK\r\n$");
	
		nvs_vars.GOT_CREDENTIALS = SI;
		uint8_t vectaux[2];
		vectaux[0] = nvs_vars.GOT_CREDENTIALS; 
		print("Las credenciales son válidas.GOT_CRED=SI grabado.\r\n");
		strcpy (EspejoFlash + 0x90 + offsetof(NVS_variables, GOT_CREDENTIALS),(void*)vectaux);
		Escribe_en_flash (EspejoFlash,0,0x200);       //Store in STM32's NVS that ESP32 has WIFI credentials saved.
		
		print("Grabando credenciales en flash.\r\n");
		strcpy (EspejoFlash+0x10,param2);		//SSID en posicion 0x10
		strcpy (EspejoFlash+0x50,param3);		//PASSWORD en posicion 0x50
		Escribe_en_flash (EspejoFlash,0,0x200);
		strcpy (password,param3);
		strcpy (ssid,param2);
		
	}
	else 
	{
		sprintf((void*)buffer_out,"^WIFIAPROV KO: %c\r\n$",causaError_WIFI);

		if(nvs_vars.GOT_CREDENTIALS == SI)  // If the previous AP was giving IP then reconnect to it.
		{
			print("Las nuevas credenciales no son válidas. Retomando conexión con el AP previo.\r\n");
			HAL_Delay(500);
			strcpy((void *)straux, "AT+CWINIT=0\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			
			strcpy((void *)straux, "AT+CWINIT=1\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			 
			strcpy((void *)straux, "AT+CWMODE=1\r\n");
			Send_RS232_ESP_ACK(500,(void *)straux);
			
			sprintf((void *)straux, "AT+CWJAP=\"%s\",\"%s\",,,,,,%u,\r\n",ssid,password,AP_CONN_TIMEOUT);
			Send_RS232_ESP_ACK(500,(void *)straux);
    	EsperaIPWifi();
			if (WIFI_conectado == SI) { nvs_vars.GOT_CREDENTIALS = SI; Escribe_en_flash ((uint8_t*)&nvs_vars.GOT_CREDENTIALS,0x90 + offsetof(NVS_variables, GOT_CREDENTIALS),1); }
			else {nvs_vars.GOT_CREDENTIALS = NO;  Escribe_en_flash ((uint8_t*)&nvs_vars.GOT_CREDENTIALS,0x90 + offsetof(NVS_variables, GOT_CREDENTIALS),1); }
		}			      
	}
}
/*************************************************************************

	Pinta_VERSION()
 Carga en el "buffer_out" la leyenda de VERSION, utilizada en CONFIGURA.C
 y MAIN.C

*************************************************************************/
void Pinta_VERSION()
{

	sprintf((void *)buffer_out, "^\r\n[VERSION:%02d.%02d.%02d BLE10]\r\n", SW_VERSION, SW_REVISION, SW_COMPILATION);
	strcat((void *)buffer_out, "CONFIGURATION BY TCP SUPPORT.\r\n");
	strcat((void *)buffer_out, "(@Ares Seguridad '25) \r\n");
	strcat((void *)buffer_out, __DATE__);
	strcat((void *)buffer_out, " ");
	strcat((void *)buffer_out, __TIME__);
	strcat((void *)buffer_out, " \r\n$");
}