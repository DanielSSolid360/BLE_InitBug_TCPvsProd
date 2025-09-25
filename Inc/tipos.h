#ifndef TIPOS_H
#define TIPOS_H

#define si 2
#define SI 1
#define NO 0 

#define EN_DEBUG	NO

// Define el modelo de dispositivo

#define MODEMV1	SI
//#define MODEMV2	SI
//#define MODEMV3	SI
//#define MODEMV4	SI

#define DIRECCION_IDF	0x1fff7590 

#define si 2 
#define SI 1
#define NO 0 
#define DESCONOCIDO 3

#define MAX_SEGUNDOS_FUERA_MAIN 240 
 
#define DESCONECTADO		0
#define CONECTADO		1
#define INTENTANDO_CONECTAR	2


#define ERROR 1
#define OK 0

#define HORA_UPGRADE	4 
#define MINUTO_UPGRADE	00 

#define HORA_RESET	2 
#define MINUTO_RESET	00 

#define MAX_TIME_WITHOUT_ADV 80 
#define LastBLECommand_MAX 60 
#define TIEMPO_MAX_SIN_MQTT 300 // 
#define SYNC_WATCH_MAX_TIME 1
 
#define timeAmongEvents 2
#define ForcedDoorTime 125U
#define OPENING_WAIT_TIME 30U //Maximum time that system will wait since the command .ABRE is executed untill calling that the locker wasn't actually opened.
#define GATE_LEFT_OPENED_MAX_TIME 120U
#define OPENING_LOCK_TIME 25U

#define SW_VERSION	0
#define SW_REVISION	3
#define	SW_COMPILATION  146

#define ARRANCANDO	0
#define IDLE		1
#define CONECTADO_BLE	2
#define ACTUALIZANDO    4 
      
typedef unsigned char byte ;
typedef unsigned char uint8_t ;
typedef unsigned short uint16_t ;
typedef unsigned int uint32_t ;

#endif