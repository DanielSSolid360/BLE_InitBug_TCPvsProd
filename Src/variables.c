#include "variables.h"

volatile uint16_t last_RL1_Event;
volatile uint16_t last_RL2_Event;
volatile uint16_t last_RL3_Event;
volatile uint16_t last_RL4_Event;
volatile uint16_t last_RL5_Event;
volatile uint16_t last_RL6_Event;
volatile uint16_t last_RL7_Event;
volatile uint16_t last_RL8_Event;
volatile uint16_t last_RL9_Event;
volatile uint16_t last_RL10_Event;

volatile uint32_t Last_RL1_Op;
volatile uint32_t Last_RL2_Op;
volatile uint32_t Last_RL3_Op;
volatile uint32_t Last_RL4_Op;
volatile uint32_t Last_RL5_Op;
volatile uint32_t Last_RL6_Op;
volatile uint32_t Last_RL7_Op;
volatile uint32_t Last_RL8_Op;
volatile uint32_t Last_RL9_Op;
volatile uint32_t Last_RL10_Op;

volatile uint16_t RL1_monitorTimer;	
volatile uint16_t RL2_monitorTimer;	
volatile uint16_t RL3_monitorTimer;	
volatile uint16_t RL4_monitorTimer;	
volatile uint16_t RL5_monitorTimer;	
volatile uint16_t RL6_monitorTimer;	
volatile uint16_t RL7_monitorTimer;	
volatile uint16_t RL8_monitorTimer;	
volatile uint16_t RL9_monitorTimer;	
volatile uint16_t RL10_monitorTimer;	

volatile uint8_t RL1_opening_lock_timer;
volatile uint8_t RL2_opening_lock_timer;
volatile uint8_t RL3_opening_lock_timer;
volatile uint8_t RL4_opening_lock_timer;
volatile uint8_t RL5_opening_lock_timer;
volatile uint8_t RL6_opening_lock_timer;
volatile uint8_t RL7_opening_lock_timer;
volatile uint8_t RL8_opening_lock_timer;
volatile uint8_t RL9_opening_lock_timer;
volatile uint8_t RL10_opening_lock_timer;

volatile uint32_t openedGate1Watcher;
volatile uint32_t openedGate2Watcher;
volatile uint32_t openedGate3Watcher;
volatile uint32_t openedGate4Watcher;
volatile uint32_t openedGate5Watcher;
volatile uint32_t openedGate6Watcher;
volatile uint32_t openedGate7Watcher;
volatile uint32_t openedGate8Watcher;
volatile uint32_t openedGate9Watcher;
volatile uint32_t openedGate10Watcher;

bool alarm_topic;

volatile char pData[256];
volatile uint8_t pInd;
bool mqtt_log;

volatile int32_t pingData;

volatile bool ESTADO_ALTA; 
volatile bool TCP_SERVER_ON; 
volatile uint16_t TCP_RESTART_TIMER;
volatile uint8_t TCP_connID;

uint8_t BLE_bug_counter;
uint8_t ERROR_bug_counter;

void reset_variables()
{
	// Inicializar todas las variables a cero
		Last_RL1_Op = 0;
		Last_RL2_Op = 0;
		Last_RL3_Op = 0;
		Last_RL4_Op = 0;
		Last_RL5_Op = 0;
		Last_RL6_Op = 0;
		Last_RL7_Op = 0;
		Last_RL8_Op = 0;
		Last_RL9_Op = 0;
		Last_RL10_Op = 0;

		last_RL1_Event = 0;
		last_RL2_Event = 0;
		last_RL3_Event = 0;
		last_RL4_Event = 0;
		last_RL5_Event = 0;
		last_RL6_Event = 0;
		last_RL7_Event = 0;
		last_RL8_Event = 0;
		last_RL9_Event = 0;
		last_RL10_Event = 0;

	if (!ESTADO_ALTA)TCP_RESTART_TIMER = 1;
}