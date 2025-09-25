#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>
#include "stdio.h"
#include <stdlib.h>
#include "string.h"

#include "main.h"
#include "varios.h"
#include "tipos.h"
#include "esp32.h"
#include "configura.h"
#include "upgrade.h"
#include "nvs.h"

extern IWDG_HandleTypeDef hiwdg;
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


extern volatile uint8_t timerMinuto;
extern volatile uint8_t timerMinuto;
extern volatile uint8_t timerminuto;
extern volatile uint32_t timerSegundo;
extern volatile uint32_t timersegundo;
extern volatile uint32_t timerMilis;//
extern volatile uint32_t timermilis;//
extern volatile uint32_t timer10MiliSeg;
extern volatile uint32_t timer10milis;
extern volatile uint32_t timer100Milis;
extern volatile uint32_t timer100milis;
extern volatile uint16_t timerLEDstRojo;
extern volatile uint16_t timerON_LED;
extern volatile uint16_t timerOFF_LED;
extern volatile uint16_t crc_16;
extern volatile uint16_t crc_16_LEIDO;
extern volatile uint8_t descargaOK;
extern volatile uint8_t statusSistema;
extern volatile uint8_t estado_LED;
extern volatile uint8_t perroMain;
extern volatile uint8_t pendienteSMTP;
extern volatile uint8_t horaActualizada;
extern volatile uint8_t dia;
extern volatile uint8_t mes;
extern volatile uint16_t year;
extern volatile uint8_t hora;
extern volatile uint8_t minuto;
extern volatile uint8_t segundo;
extern volatile uint8_t pulsadorActivado;

//Pending variables
extern volatile uint8_t actualizacionPendiente;
extern volatile bool pendingMQTTmsg; //
extern volatile bool pending_download;//
extern volatile bool pending_wifi_dis;//
extern volatile bool pending_wifi_con;//

extern volatile uint16_t tiempoEsperaRespuesta;
extern volatile uint8_t tiempoEsperaRespuestaLargo;

extern volatile uint8_t estadoPULSADOR;
extern volatile uint8_t tiempoPulsador;
 
extern volatile uint8_t tiempoCT1;
extern volatile uint8_t tiempoCT2;
extern volatile uint8_t tiempoCT3;
extern volatile uint8_t tiempoCT4;
extern volatile uint8_t tiempoCT5;
extern volatile uint8_t tiempoCT6;
extern volatile uint8_t tiempoCT7;
extern volatile uint8_t tiempoCT8;
extern volatile uint8_t tiempoCT9;
extern volatile uint8_t tiempoCT10;
extern volatile uint8_t tiempoTAMPER;
extern volatile bool TAMPER_TRIGGERED;

extern volatile uint16_t BLECONN_checkTim; //
extern volatile uint16_t BLESyncTimer;
extern volatile uint8_t LastBLECommand;    //

extern volatile uint32_t Last_RL1_Op;
extern volatile uint32_t Last_RL2_Op;
extern volatile uint32_t Last_RL3_Op;
extern volatile uint32_t Last_RL4_Op;
extern volatile uint32_t Last_RL5_Op;
extern volatile uint32_t Last_RL6_Op;
extern volatile uint32_t Last_RL7_Op;
extern volatile uint32_t Last_RL8_Op;
extern volatile uint32_t Last_RL9_Op;
extern volatile uint32_t Last_RL10_Op;

extern volatile uint16_t RL1_monitorTimer;	
extern volatile uint16_t RL2_monitorTimer;	
extern volatile uint16_t RL3_monitorTimer;	
extern volatile uint16_t RL4_monitorTimer;	
extern volatile uint16_t RL5_monitorTimer;	
extern volatile uint16_t RL6_monitorTimer;	
extern volatile uint16_t RL7_monitorTimer;	
extern volatile uint16_t RL8_monitorTimer;	
extern volatile uint16_t RL9_monitorTimer;	
extern volatile uint16_t RL10_monitorTimer;	

extern volatile uint16_t last_RL1_Event;
extern volatile uint16_t last_RL2_Event;
extern volatile uint16_t last_RL3_Event;
extern volatile uint16_t last_RL4_Event;
extern volatile uint16_t last_RL5_Event;
extern volatile uint16_t last_RL6_Event;
extern volatile uint16_t last_RL7_Event;
extern volatile uint16_t last_RL8_Event;
extern volatile uint16_t last_RL9_Event;
extern volatile uint16_t last_RL10_Event;

extern volatile uint8_t RL1_opening_lock_timer;
extern volatile uint8_t RL2_opening_lock_timer;
extern volatile uint8_t RL3_opening_lock_timer;
extern volatile uint8_t RL4_opening_lock_timer;
extern volatile uint8_t RL5_opening_lock_timer;
extern volatile uint8_t RL6_opening_lock_timer;
extern volatile uint8_t RL7_opening_lock_timer;
extern volatile uint8_t RL8_opening_lock_timer;
extern volatile uint8_t RL9_opening_lock_timer;
extern volatile uint8_t RL10_opening_lock_timer;

extern volatile uint32_t openedGate1Watcher;
extern volatile uint32_t openedGate2Watcher;
extern volatile uint32_t openedGate3Watcher;
extern volatile uint32_t openedGate4Watcher;
extern volatile uint32_t openedGate5Watcher;
extern volatile uint32_t openedGate6Watcher;
extern volatile uint32_t openedGate7Watcher;
extern volatile uint32_t openedGate8Watcher;
extern volatile uint32_t openedGate9Watcher;
extern volatile uint32_t openedGate10Watcher;
extern bool alarm_topic;

extern volatile uint8_t ssid[64];
extern volatile uint8_t password[64];
//extern volatile uint8_t GOT_CREDENTIALS[1];

extern volatile uint8_t BLE_desconectado;
extern volatile uint8_t BLE_reconectar;
extern volatile uint8_t envia_por_BLE;
extern uint8_t buffer_copia_TCP_en_BLE[256];
extern uint8_t Indice_a_buffer_RX_desde_BLE;
extern uint8_t Indice_a_buffer_RX_desde_MQTT;
extern uint8_t buffer_RX_desde_BLE[256];
extern uint8_t buffer_RX_desde_MQTT[256];
extern volatile uint8_t comando_BLE;
extern volatile uint8_t comando_MQTT;
extern char MQTTpendingMsg[400]; //
extern volatile uint8_t recibido_de_conexion_BLE;
extern volatile uint8_t numero_conexiones_BLE;
extern volatile unsigned char ForceBLEDISCONN; //
extern uint8_t buffer_eventos_BLE[256];
extern uint8_t eventos_BLE_nuevo;
extern uint8_t BLE0;
extern uint8_t BLE1;


extern volatile uint8_t error_WIFI;
extern volatile uint8_t causaError_WIFI;
extern volatile uint8_t WIFI_conectado;
extern volatile uint8_t conexion_TCP;
extern uint8_t buffer_RX_desde_TCP[256];
extern volatile bool TCP_COMMAND;
extern volatile uint8_t abre_sck_TCP;
extern volatile uint8_t envia_msg_bienvenida_TCP;
extern uint8_t enviar_por_conexion_BLE;
extern volatile bool pending_TCP_conn;

extern volatile uint8_t ETH_conectado;
extern uint8_t buffer_RX_desde_TCP_ETH[256];
extern volatile uint8_t comando_TCP_ETH;
extern volatile uint16_t TCP_BUFF_INDEX;
extern volatile uint8_t abre_sck_TCP_ETH;

extern uint32_t Indice_A_buffer_RX_MODEM;
extern uint8_t buffer_RX_MODEM[560];
extern uint8_t capturaLog;
extern volatile uint8_t Recibido_Prompt_ESP;
extern volatile uint8_t ErrorEnESP;
extern uint8_t BufferTX_ESP[256];
extern char buffer_in[256];
extern uint8_t buffer_out[356];
extern uint8_t strmodem[4];
extern uint8_t strconsole[4];
extern uint8_t Indice_A_buffer_RS232_TEMP;
extern uint8_t buffer_in_RS232_TEMP[270];
extern uint8_t comando_consola;
extern uint8_t MonitorizaRS232_ESP32;
extern volatile uint8_t Recibido_OK_ESP;
extern volatile uint8_t Recibido_READY_ESP32;
extern volatile uint16_t testW;
extern volatile uint8_t test_Conexion_ok;
extern volatile unsigned char falloEnPing;

extern volatile unsigned char estado_MQTT;

extern unsigned char UniqueIDF[14];
extern unsigned char StrUniqueIDF[28];
extern unsigned char MAC_BLE[18];
extern unsigned char IP_ETH[16]; 
extern unsigned char IP_WIFI[16]; 
extern volatile bool queryIP; 

extern volatile uint8_t AP_CONN_TIMEOUT;
extern volatile uint8_t tengo_IP_ETH;
extern volatile uint8_t tengo_IP_WIFI;
extern volatile bool DHCP_WIFI;
extern volatile bool DHCP_ETH;

extern volatile uint8_t pubMqttOK;
extern bool conexionMqttAntesIntentada;
extern unsigned char copiaUltimoCmd[64];
extern uint8_t Inicializando;

extern volatile uint16_t tiempoSinMQTT;
extern volatile uint16_t mqttReconTimer; //
extern volatile uint16_t tiempoSinInternet;
extern volatile uint8_t acaboDeRecibirIP_ETH;
extern volatile uint8_t acaboDeRecibirIP_WIFI;

extern int posicionFichero;
extern uint16_t aTx;
extern char numeroEnAscii[9];
extern char aTransmitir[50];
extern byte recibido[32];
extern byte transmitido[2];
extern char suceso;

//Buffer de 8KB usado para almacenar los paquetes con los binarios del nuevo firmware cargado del servidor.
extern char buffer_aligned[0X2000] __attribute__((aligned(16))); //"attribute((aligned(16)))"" instruye al compilador a alinear la variable en una dirección que sea un múltiplo de 16.
extern volatile uint32_t Indice_A_buffer_aligned;

void print(char* string); //
void reset_variables();//
extern bool system_init_done; //

extern volatile char pData[256];
extern volatile uint8_t pInd;
extern bool mqtt_log;
extern volatile int32_t pingData;

extern volatile bool ESTADO_ALTA;
extern volatile bool TCP_SERVER_ON; 
extern volatile uint16_t TCP_RESTART_TIMER;
extern volatile uint8_t TCP_connID;

extern uint8_t BLE_bug_counter;
extern uint8_t ERROR_bug_counter;

#endif