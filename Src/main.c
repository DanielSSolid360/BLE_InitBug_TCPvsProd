/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "variables.h"


/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM14_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t timerMinuto;
volatile uint8_t timerminuto;
volatile uint32_t timerSegundo;
volatile uint32_t timersegundo;
volatile uint32_t timerMilis;
volatile uint32_t timermilis;
volatile uint32_t timer10MiliSeg;
volatile uint32_t timer10milis;
volatile uint32_t timer100Milis;
volatile uint32_t timer100milis;
volatile uint16_t timerLEDstRojo;
volatile uint16_t timerON_LED;
volatile uint16_t timerOFF_LED;
volatile uint16_t crc_16;
volatile uint16_t crc_16_LEIDO;
volatile uint8_t descargaOK;
volatile uint8_t statusSistema;
volatile uint8_t estado_LED;
volatile uint8_t perroMain;
volatile uint8_t pendienteSMTP;
volatile uint8_t horaActualizada;
volatile uint8_t dia;
volatile uint8_t mes;
volatile uint16_t year;
volatile uint8_t hora;
volatile uint8_t minuto;
volatile uint8_t segundo;
volatile uint8_t pulsadorActivado;

//Pending variables
volatile uint8_t actualizacionPendiente;
volatile bool pendingMQTTmsg; //
volatile bool pending_download;//
volatile bool pending_wifi_dis;//
volatile bool pending_wifi_con;//

volatile uint16_t tiempoEsperaRespuesta;
volatile uint8_t tiempoEsperaRespuestaLargo;

volatile uint8_t estadoPULSADOR;
volatile uint8_t tiempoPulsador;

volatile uint16_t BLECONN_checkTim; 
volatile uint16_t BLESyncTimer;
volatile uint8_t LastBLECommand;

volatile bool TAMPER_TRIGGERED;


volatile uint8_t ssid[64];
volatile bool GOT_SSID;
volatile uint8_t password[64];
volatile bool GOT_PASS;
//volatile uint8_t GOT_CREDENTIALS[1];

volatile uint8_t BLE_desconectado;
volatile uint8_t BLE_reconectar;

volatile uint8_t error_WIFI;
volatile uint8_t causaError_WIFI;
volatile uint8_t WIFI_conectado;
volatile uint8_t conexion_TCP;
uint8_t buffer_RX_desde_TCP[256];
volatile uint8_t comando_TCP;
volatile uint8_t abre_sck_TCP;
volatile uint8_t envia_msg_bienvenida_TCP;

volatile uint8_t ETH_conectado;
uint8_t buffer_RX_desde_TCP_ETH[256];
volatile uint8_t comando_TCP_ETH;
volatile uint8_t abre_sck_TCP_ETH;


volatile uint8_t envia_por_BLE;
uint8_t buffer_copia_TCP_en_BLE[256];
uint8_t Indice_a_buffer_RX_desde_BLE;
uint8_t Indice_a_buffer_RX_desde_MQTT;
uint8_t buffer_RX_desde_BLE[256];
uint8_t buffer_RX_desde_MQTT[256];
volatile uint8_t comando_BLE;
volatile uint8_t comando_MQTT;
char MQTTpendingMsg[400]; //
volatile uint8_t recibido_de_conexion_BLE;
volatile uint8_t numero_conexiones_BLE;
volatile unsigned char ForceBLEDISCONN;
uint8_t buffer_eventos_BLE[256];
uint8_t eventos_BLE_nuevo;
uint8_t enviar_por_conexion_BLE;
uint8_t BLE0;
uint8_t BLE1;

uint32_t Indice_A_buffer_RX_MODEM;
uint8_t buffer_RX_MODEM[560];
uint8_t capturaLog;
volatile uint8_t Recibido_Prompt_ESP;
volatile uint8_t ErrorEnESP;
uint8_t BufferTX_ESP[256];
char buffer_in[256];
uint8_t buffer_out[356];
uint8_t strmodem[4];
uint8_t strconsole[4];
uint8_t Indice_A_buffer_RS232_TEMP;
uint8_t buffer_in_RS232_TEMP[270];
uint8_t comando_consola;
uint8_t MonitorizaRS232_ESP32;
volatile uint8_t Recibido_OK_ESP;
volatile uint8_t Recibido_READY_ESP32;
volatile uint16_t testW;
volatile uint8_t test_Conexion_ok;
volatile unsigned char falloEnPing;

volatile unsigned char estado_MQTT;

unsigned char UniqueIDF[14];
unsigned char StrUniqueIDF[28];
unsigned char MAC_BLE[18]; 
unsigned char IP_ETH[16]; 
unsigned char IP_WIFI[16]; 
volatile bool queryIP;  

volatile uint8_t AP_CONN_TIMEOUT;
volatile uint8_t tengo_IP_ETH;
volatile uint8_t tengo_IP_WIFI;
volatile bool DHCP_WIFI;
volatile bool DHCP_ETH;

volatile uint8_t pubMqttOK;
bool conexionMqttAntesIntentada;
unsigned char copiaUltimoCmd[64]; 
uint8_t Inicializando;

volatile uint16_t tiempoSinMQTT;
volatile uint16_t mqttReconTimer;
volatile uint16_t tiempoSinInternet;
volatile uint8_t acaboDeRecibirIP_WIFI;
volatile uint8_t acaboDeRecibirIP_ETH;

/*********** FLASH UPGRADE **********/
int posicionFichero;
uint16_t aTx;
char numeroEnAscii[9];
char aTransmitir[50];
byte recibido[32];
byte transmitido[2];
char suceso;

/*********** FIRMWARE DOWNLOAD **********/
char buffer_aligned[0X2000] __attribute__((aligned(16))); //Buffer to store the new binaries files coming from server
volatile uint32_t Indice_A_buffer_aligned;

/*********** NVS memory **********/            
uint8_t EspejoFlash[0x200];		//Located en 0x0801F800

volatile NVS_variables nvs_vars;

void print(char* string);

bool system_init_done; //

/*
EspejoFlash[0].- Libre
*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


int main(void)
 { 
  /* USER CODE BEGIN 1 */
	volatile uint8_t esperaArranque;	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM14_Init();	
	MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
#ifndef EN_DEBUG
  MX_IWDG_Init();
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  
  BLE_desconectado=SI;
  timerLEDstRojo=2000;
  MonitorizaRS232_ESP32=SI;
  pendienteSMTP=SI;
  
  print("\n\r\n\r*********************************************** \n\r");
  print("*********************************************** \n\r");
  print("*                                             * \n\r");
  print("*                    BLE10                    * \n\r");
  print("*                                             * \n\r");
  print("*                        @Ares Seguridad '25  * \n\r");
  print("*********************************************** \n\r");
  print("*********************************************** \n\r\n\r");

  LeeIDF(UniqueIDF,(unsigned char *)DIRECCION_IDF,12);
  UniqueIDF[12]=0;
  sprintf (buffer_out,"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",UniqueIDF[0],UniqueIDF[1],UniqueIDF[2],UniqueIDF[3],UniqueIDF[4],UniqueIDF[5],
  										UniqueIDF[6],UniqueIDF[7],UniqueIDF[8],UniqueIDF[9],UniqueIDF[10],UniqueIDF[11]);
  HAL_GPIO_WritePin(GPIOD, RST_ESP32_Pin, GPIO_PIN_RESET); 
  
  strcpy (StrUniqueIDF,buffer_out);
  print ((void *)StrUniqueIDF);  
 
  Lee_en_flash	(EspejoFlash,0,0x200);
 
  Copia_SSDI();
  Copia_PASSWORD();
	
  Pinta_VERSION();  	
  print ((void *)buffer_out);  
  print ("\r\nSystem....OK\r\n\r\n");  

  HAL_GPIO_WritePin(GPIOD, LED_Pin, GPIO_PIN_RESET);		//LED ROJO ON
  HAL_TIM_Base_Start_IT(&htim14);
  HAL_UART_Receive_IT(&huart2, (void *)strconsole, 1);
  HAL_UART_Receive_IT(&huart1, (void *)strmodem, 1);
  
  esperaArranque = 1; /*UniqueIDF[0];*/
  
 /* while (esperaArranque) 
  {
  	HAL_Delay(100);
		sprintf (buffer_out,"Arrancando ESP32 en: %03d\r",esperaArranque);
		print ((void *)buffer_out); 
		esperaArranque--;
	}*/
  //print ("\r\nEspera aleatoria finalizada, OK\r\n\r\n"); 
	
  char comment[30];   
  Lee_en_flash((void*)&nvs_vars.GOT_CREDENTIALS,0x90 + offsetof(NVS_variables,GOT_CREDENTIALS),1);
  sprintf(comment,"\r\nGOT CREDENTIALS: %u\r\n",nvs_vars.GOT_CREDENTIALS);
	print(comment);
  
	Init_ESP32();
  
  estadoPULSADOR=HAL_GPIO_ReadPin(GPIOB,PULSADOR_Pin);
  
	mqttReconTimer = 1;
	reset_variables();
	
  while (1)
  { 
		pending_wifi_handler();//
		pending_MQTT_messages();  //
  	Procesa_consola();
  	check_BLE_status();//
		mqtt_recon_handler();//
  	Test_conexion_TCP();
  	GPIO_STATE();
  	Test_pedir_IP_WIFI();
  	Reenvia_TCP_por_BLE();
  	Pinta_nuevos_eventos_BLE();
  	ActualizaHora();
  	Actualizacion_Programada();
		check_pending_download();//
  	perroMain=0;
		system_init_done = true;
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 125;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 64 - 1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 100-1;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 256000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RL3_Pin|RL4_Pin|RL8_Pin|RL6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RL1_Pin|RL2_Pin|RL7_Pin|RL10_Pin
                          |RL9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RL5_GPIO_Port, RL5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RST_ESP32_GPIO_Port, RST_ESP32_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : RL3_Pin RL4_Pin RL8_Pin RL6_Pin */
  GPIO_InitStruct.Pin = RL3_Pin|RL4_Pin|RL8_Pin|RL6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CT3_Pin CT7_Pin CT5_Pin CT6_Pin */
  GPIO_InitStruct.Pin = CT3_Pin|CT7_Pin|CT5_Pin|CT6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CT4_Pin CT1_Pin CT2_Pin PULSADOR_Pin
                           CT9_Pin */
  GPIO_InitStruct.Pin = CT4_Pin|CT1_Pin|CT2_Pin|PULSADOR_Pin
                          |CT9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RL1_Pin RL2_Pin RL7_Pin RL10_Pin
                           RL9_Pin */
  GPIO_InitStruct.Pin = RL1_Pin|RL2_Pin|RL7_Pin|RL10_Pin
                          |RL9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : CT8_Pin */
  GPIO_InitStruct.Pin = CT8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CT8_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RL5_Pin */
  GPIO_InitStruct.Pin = RL5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RL5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin RST_ESP32_Pin */
  GPIO_InitStruct.Pin = LED_Pin|RST_ESP32_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : CT10_Pin */
  GPIO_InitStruct.Pin = CT10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

volatile bool error_h;

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  error_h = true;
	__disable_irq();
	
  while (1)
  {
		NVIC_SystemReset();  //Emergency reset
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
