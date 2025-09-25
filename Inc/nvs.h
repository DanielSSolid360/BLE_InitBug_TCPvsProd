// nvs_variables.h

#ifndef NVS_VARIABLES_H
#define NVS_VARIABLES_H

#include <stdint.h>

/*********** NVS memory **********/            
extern uint8_t EspejoFlash[0x200];		//Located en 0x0801F800

/*********** NVS variables **********/   
typedef struct{                 //offset
	
	uint8_t GOT_CREDENTIALS;      //+0x90
	//uint8_t VAARIABLE2          //+0x91
  //....
	
}NVS_variables;

// Declaración de la estructura global y volátil
extern volatile NVS_variables nvs_vars;

#endif // NVS_VARIABLES_H
