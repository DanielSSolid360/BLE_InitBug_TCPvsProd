// nvs_variables.h

#ifndef NVS_VARIABLES_H
#define NVS_VARIABLES_H

#include <stdint.h>

/*********** NVS memory **********/            
extern uint8_t EspejoFlash[0x200];		//Located en 0x0801F800
/*
[ADDRESS]  | USE
[0x0]      | Byte used for the bool variable ESTADO_ALTA
[0x02]     | Byte used for the BLE bug counter variable
[0x10]     | Start of SSID string
[0x50]     | Start of PASSWORD string
[0x90]     | Byte used for the bool variable GOT_CREDENTIALS	
*/

/*********** NVS variables **********/   
typedef struct{                 //offset
	
	uint8_t GOT_CREDENTIALS;      //+0x90
	//uint8_t VAARIABLE2          //+0x91
  //....
	
}NVS_variables;

// Declaración de la estructura global y volátil
extern volatile NVS_variables nvs_vars;

#endif // NVS_VARIABLES_H
