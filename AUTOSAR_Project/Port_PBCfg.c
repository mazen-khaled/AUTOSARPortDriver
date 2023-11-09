 /******************************************************************************
 *
 * Module: Port Configuration
 *
 * File Name: Port_Cfg.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Configuration.
 *
 * Author: Mazen Khaled
 ******************************************************************************/
#include "Port.h"

/*******************************************************************************
 *                           Software Version 1.0.0                            *
 *******************************************************************************/
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

 /* Software Version checking between Dio_PBcfg.c and Dio.h files */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif

/*******************************************************************************
 *                           AUTOSAR Version 4.0.3                             *
 *******************************************************************************/
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif
   
/*******************************************************************************
 *                            Post-Build Variables                             *
 *******************************************************************************/  
 const Port_ConfigType Port_Configuration = {
   PORTA,PA0,INPUT,PULL_UP,STD_LOW,DIO,
   PORTA,PA1,INPUT,PULL_DOWN,STD_LOW,ADC,
   PORTA,PA2,OUTPUT,OFF,STD_HIGH,ADC,
   PORTA,PA3,OUTPUT,OFF,STD_LOW,DIO,
   PORTA,PA4,OUTPUT,OFF,STD_LOW,Mode1,
   PORTA,PA5,OUTPUT,OFF,STD_LOW,Mode1,
   PORTA,PA6,OUTPUT,OFF,STD_LOW,Mode2,
   PORTA,PA7,OUTPUT,OFF,STD_LOW,Mode2,
   
   PORTB,PB0,OUTPUT,OFF,STD_LOW,Mode3,
   PORTB,PB1,OUTPUT,OFF,STD_LOW,Mode3,
   PORTB,PB2,INPUT,PULL_UP,STD_LOW,Mode4,
   PORTB,PB3,INPUT,PULL_DOWN,STD_LOW,Mode4,
   PORTB,PB4,OUTPUT,OFF,STD_HIGH,Mode5,
   PORTB,PB5,OUTPUT,OFF,STD_LOW,Mode5,
   PORTB,PB6,OUTPUT,OFF,STD_LOW,Mode6,
   PORTB,PB7,OUTPUT,OFF,STD_LOW,Mode6,
   
   PORTC,PC0,OUTPUT,OFF,STD_LOW,Mode3,
   PORTC,PC1,OUTPUT,OFF,STD_LOW,Mode3,
   PORTC,PC2,INPUT,PULL_UP,STD_LOW,Mode4,
   PORTC,PC3,INPUT,PULL_DOWN,STD_LOW,Mode4,
   PORTC,PC4,INPUT,PULL_UP,STD_LOW,Mode7,
   PORTC,PC5,INPUT,PULL_DOWN,STD_LOW,Mode7,
   PORTC,PC6,OUTPUT,OFF,STD_HIGH,Mode8,
   PORTC,PC7,OUTPUT,OFF,STD_LOW,Mode8,
   
   PORTD,PD0,INPUT,PULL_UP,STD_LOW,Mode9,
   PORTD,PD1,INPUT,PULL_DOWN,STD_LOW,Mode9,
   PORTD,PD2,INPUT,PULL_UP,STD_LOW,Mode10,
   PORTD,PD3,OUTPUT,OFF,STD_LOW,Mode10,
   PORTD,PD4,OUTPUT,OFF,STD_LOW,Mode11,
   PORTD,PD5,OUTPUT,OFF,STD_LOW,Mode11,
   PORTD,PD6,OUTPUT,OFF,STD_LOW,Mode12,
   
   PORTE,PE0,INPUT,OFF,STD_LOW,Mode12,
   PORTE,PE1,INPUT,OFF,STD_LOW,Mode13,
   PORTE,PE2,INPUT,PULL_UP,STD_LOW,Mode13,
   PORTE,PE3,INPUT,PULL_DOWN,STD_LOW,Mode14,
   PORTE,PE4,INPUT,OFF,STD_HIGH,Mode14,
   PORTE,PE5,INPUT,OFF,STD_LOW,Mode1,
   
   PORTF,PF0,OUTPUT,OFF,STD_LOW,Mode1,
   PORTF,PF1,OUTPUT,OFF,STD_LOW,Mode2,
   PORTF,PF2,OUTPUT,OFF,STD_LOW,Mode3,
   PORTF,PF3,OUTPUT,OFF,STD_LOW,Mode4,
   PORTF,PF4,INPUT,PULL_UP,STD_LOW,Mode5,
 };