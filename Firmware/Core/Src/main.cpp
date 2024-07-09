/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Joseph-Ladino.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <cstdarg>
#include <cstdio>

#include "usbd_cdc_if.h"
#include "globals.h"

#define BUILD_WITH_SECRETS
#ifdef BUILD_WITH_SECRETS
#include "secrets.hpp"
#endif

#include "W5500HC.hpp"
//#include "MQTTInterface.hpp"
#include "CountdownTimer.hpp"
#include "PAHOClient.hpp"
//#include <WS2815Strip.hpp>
//#include "DMAControllerLED.hpp"
#include "DMAStrip.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
using namespace JETHERNET;
//using namespace JMQTT;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// defined in HAL_TIM_Base_MspInit() in stm32g0xx_hal_msp.c
extern DMA_HandleTypeDef hdma_tim1_ch1;

extern "C" {
int _write(int file, char *ptr, int len) {

#ifdef USE_USB_DEBUG
	uint8_t rc = USBD_FAIL;
	CountdownTimer timeoutTimer(2000);

	do {
		rc = CDC_Transmit_FS((uint8_t*) ptr, len);
	} while (USBD_OK != rc && !timeoutTimer.expired());

	if (timeoutTimer.expired())
		return 0;
//
//	if (USBD_FAIL == rc) {
//		HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_SET);
//		return 0;
//	}
#endif
	return len;
}

}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim1_ch1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

W5500HC eth;
JMQTT::PAHOClient mqtt;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//static unsigned int arrivedcount = 0;
//void messageArrived(MQTT::MessageData &md) {
//	MQTT::Message &message = md.message;
//
//	USB_Printf("Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n",
//			++arrivedcount, message.qos, message.retained, message.dup,
//			message.id);
//	USB_Printf("Payload %.*s\n", (int ) message.payloadlen,
//			(char* ) message.payload);
//}
//
//void messageArrivedDefault(MQTT::MessageData &md) {
//	USB_Printf("\nDefault handler called:\n");
//	messageArrived(md);
//}
void setLedPower(bool on) {
	auto state = on ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(LED_PWR_EN_GPIO_Port, LED_PWR_EN_Pin, state);
}

bool ledPowerReady() {
	return HAL_GPIO_ReadPin(LED_PWR_EN_GPIO_Port, LED_PWR_EN_Pin)
			== GPIO_PIN_SET;
}

void messageReceived(JMQTT::Client &client, JMQTT::Message msg) {
	USB_Printf("%s: %s\n", msg.topic.data(), msg.payload.data());
	if (msg.topic == "test/switch") {
		setLedPower(msg.payload == "ON");
		printf("turning LED_PWR_EN %s\n", msg.payload == "ON" ? "on" : "off");
	}
}

void mqttOnConnected(JMQTT::Client &client) {
	client.publish( { "test", "test" });
	client.subscribe("test/#");
}

bool connectEth() {
	if (!eth.waitForLink(5000)) {
		USB_Printf("Unable to get PHY link connection\n");
		return false;
	}

	if (!eth.enableDHCP(5000)) {
		USB_Printf("Unable to initiate DHCP\n");
		return false;
	}

	if (!eth.enableDNS()) {
		USB_Printf("Unable to initiate DNS\n");
		return false;
	}

	return true;
}
using buftype = uint32_t;
using LEDStrip = JLED::DMAStrip<300, 15, 79, buftype>;
LEDStrip strip;

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	strip.onDMAInterrupt(true);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	strip.onDMAInterrupt(false);
}

void startDMA(buftype *buf, uint16_t len) {
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, buf, len);
}

void stopDMA() {
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
}

void LED_Init(TIM_HandleTypeDef *tim) {
	using namespace JLED;

	CountdownTimer pwrTimeout(100);

	setLedPower(true);

	strip.setAll(0xFF00FF);
	strip.setStartDMACallback(startDMA);
	strip.setStopDMACallback(stopDMA);

	while (!(ledPowerReady() || pwrTimeout.expired()))
		;

	if (pwrTimeout.expired()) {
		USB_Printf("\n ERROR ENABLING LED POWER\n");
		return;
	}
}

void LED_Update() {
	static int led = 0, delayMs = 100, frameMs = 7;
	static CountdownTimer delay(delayMs);
	static CountdownTimer frameTimer(frameMs);

	static JLED::Color cols[] = { 0xff0000, 0x00ff00, 0x0000ff };
	if (delay.expired()) {
		delay.countdown_ms(delayMs);
		strip.set(led, cols[(led / 3) % (sizeof(cols) / sizeof(JLED::Color))]);

		led = (led + 1) % LEDStrip::NUM_PIXELS;
	}

	if(frameTimer.expired() && !strip.displayInProgress()) {
		frameTimer.countdown_ms(frameMs);
		strip.display();
	}
}

//#define TEST_LED_ONLY

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

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
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM1_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init();
	MX_USB_Device_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start_IT(&htim3);
#ifndef TEST_LED_ONLY
#ifdef DEBUG
	// allow time for terminal to connect to USB
	uint32_t timeout = 1000;
	CountdownTimer usbTimeout(timeout);
	while (!usbTimeout.expired()
			&& CDC_Get_Device_State() != USBD_STATE_CONFIGURED)
		;
	USB_Printf("\n\n");
	USB_Printf("%lums to initialize USB\n", timeout - usbTimeout.left_ms());

#endif

	W5500Config conf { &hspi1, ETH_SCSn_GPIO_Port,
	ETH_SCSn_Pin, ETH_RSTn_GPIO_Port, ETH_RSTn_Pin };

	if (!eth.init(&conf)) {
		USB_Printf("Unable to initiate chip\n");
		return false;
	}

	connectEth();

	JMQTT::ClientConfig mqttConf;
	auto &sock = eth.getFreeSocket();

#ifdef BUILD_WITH_SECRETS

	using namespace JSECRETS;
	mqttConf.clientName = MQTT_CLIENT_ID;
	mqttConf.username = MQTT_SERVER_USERNAME;
	mqttConf.password = MQTT_SERVER_PASSWORD;
	mqttConf.brokerIP = MQTT_SERVER_IP;
	mqttConf.brokerPort = MQTT_SERVER_PORT;

#endif

	mqtt.setConnectCallback(mqttOnConnected);
	mqtt.setMessageCallback(messageReceived);

	if (mqtt.connect(sock, mqttConf)) {
		USB_Printf("Connected client to MQTT server!\n");
	} else {
		USB_Printf("Error connecting client to MQTT server!\n");
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

//	int oldRc = rc;
	uint32_t reconnectTimeMs = 2500;
	CountdownTimer reconnectTimer(reconnectTimeMs);
#endif
	LED_Init(&htim1);

	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		LED_Update();
#ifndef TEST_LED_ONLY
		auto connected = mqtt.update(1);
		if (!connected && reconnectTimer.expired()) {
			USB_Printf("\nClient disconnected!\n");

			bool success = false;
			// if no physical connection, try to establish one
			if (/* !eth.phyLinkStatus() */true) {
				eth.softReset();
				success = connectEth(); // try to wait for connection
				if (!success)
					goto RECONNECT_EXIT;
			}

			mqtt.disconnect();
			// issue has to be related to client/server connection
			success = mqtt.reconnect(eth.getFreeSocket());

			RECONNECT_EXIT: if (success) {
				USB_Printf("Connected client to MQTT server!\n");
			} else {
				USB_Printf("Error connecting client to MQTT server!\n");
			}

			reconnectTimer.countdown_ms(reconnectTimeMs);
		}
#endif
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 80 - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 1023;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 62499;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

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
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, ETH_RSTn_Pin | ETH_SCSn_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			TEST_LED_Pin | LED_PWR_EN_Pin | ESP_IO2_Pin | ESP_IO0_Pin
					| ESP_EN_Pin | ESP_RSTn_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : ETH_RSTn_Pin ETH_SCSn_Pin */
	GPIO_InitStruct.Pin = ETH_RSTn_Pin | ETH_SCSn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : ETH_INTn_Pin */
	GPIO_InitStruct.Pin = ETH_INTn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ETH_INTn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : TEST_LED_Pin LED_PWR_EN_Pin ESP_IO2_Pin ESP_IO0_Pin
	 ESP_EN_Pin ESP_RSTn_Pin */
	GPIO_InitStruct.Pin = TEST_LED_Pin | LED_PWR_EN_Pin | ESP_IO2_Pin
			| ESP_IO0_Pin | ESP_EN_Pin | ESP_RSTn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_PWR_OK_Pin */
	GPIO_InitStruct.Pin = LED_PWR_OK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(LED_PWR_OK_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//void USB_Printf(const char *fmt, ...) {
//#ifdef DEBUG_SB_OLD
//	static char buff[2048];
//	va_list args;
//	va_start(args, fmt);
//	vsnprintf(buff, sizeof(buff), fmt, args);
////    HAL_UART_Transmit(&huart1, (uint8_t*)buff, strlen(buff),
////                      HAL_MAX_DELAY);
//
//	uint8_t result, tries = 0;
//	do {
//		result = CDC_Transmit_FS((unsigned char*) buff,
//				strlen((const char*) buff));
//		if (result != USBD_OK)
//			HAL_Delay(1);
//	} while (result != USBD_OK && ++tries < 3);
//
//	va_end(args);
//#endif
//}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	if (htim == &htim3) {
		//		USB_Printf("1 sec int\n"); // NOTE: CDC uses interrupts so this will never return
		HAL_GPIO_TogglePin(TEST_LED_GPIO_Port, TEST_LED_Pin);
	}

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	if (htim->Instance == TIM6) {
		CountdownTimer::msTick();
		eth.msTick();
	}
	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	USB_Printf("\n\n\tERROR OCCURED\n\n");
	while (1) {

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
