/**
 ****************************************************************************************
 *
 * @file        app.c
 *
 * @brief       rBLE Sample Application
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 *
 * $Rev: 6128 $
 *
 ****************************************************************************************
 */

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    "r_uart_api.h"
#include    "rble.h"
#include    "rble_app.h"
#include    "rble_api_custom.h"
#include    "rscip_ext.h"

#include    "logger.h"
#include    "app.h"

#include    "db_handle.h"
#include    "sam/sams.h"

#include    "hal_data.h"

//#define ENABLE_SET_NAME

#ifdef ENABLE_SET_NAME
#define DEVICE_NAME "DK-S124 BL/HR/HT Demo"
#endif

#define APP_Disp_Clear()       //printf("clear")
#define APP_Disp(x, s)         //printf("%s", s)
#define APP_Disp_Status(x)     //printf("%d", x)
#define APP_Disp_RW(c)         //printf("%c", c)

/**************************** Define Declaration **************************************/
#define     RUN_COMMAND_QUEUE_SIZE          10
#define     ILLEGAL_CONNECTION_HANDLE       0xffff

#ifndef char_t
typedef char            char_t;
#endif

typedef enum {
    APP_DISP_EVT,
    APP_DISP_CMD,
    APP_DISP_STATUS,
    APP_DISP_FATAL,
} APP_DISP_TYPE;

typedef struct
{
    uint8_t ES_DESCRIPTOR_VALUE_CHANGED_VAL[1];
    uint8_t ES_APPARENT_WIND_DIRECTION_VAL[2];
    uint8_t ES_APPARENT_WIND_SPEED_VAL[2];
    uint8_t ES_DEW_POINT_VAL[1];
    uint8_t ES_ELEVATION_VAL[3];
    uint8_t ES_GUST_FACTOR_VAL[1];
    uint8_t ES_HEAT_INDEX_VAL[1];
    uint8_t ES_HUMIDITY_VAL[2];
    uint8_t ES_IRRADIANCE_VAL[2];
    uint8_t ES_POLLEN_CONCENTRATION_VAL[3];
    uint8_t ES_RAINFALL_VAL[2];
    uint8_t ES_PRESSURE_VAL[4];
    uint8_t ES_TEMPERATURE_VAL[2];
    uint8_t ES_TRUE_WIND_DIRECTION_VAL[2];
    uint8_t ES_TRUE_WIND_SPEED_VAL[2];
    uint8_t ES_UV_INDEX_VAL[1];
    uint8_t ES_WIND_CHILL_VAL[1];
    uint8_t ES_BAROMETRIC_PRESSURE_TREND_VAL[1];
    uint8_t ES_MAGNETIC_DECLINATION_VAL[2];
    uint8_t ES_MAGNETIC_FLUX_DENSITY___2D_VAL[4];
    uint8_t ES_MAGNETIC_FLUX_DENSITY___3D_VAL[6];
    uint8_t DI_MANUFACTURER_NAME_STRING_VAL[7];
    uint8_t DI_MODEL_NUMBER_STRING_VAL[7];
    uint8_t DI_SERIAL_NUMBER_STRING_VAL[7];
    uint8_t DI_HARDWARE_REVISION_STRING_VAL[7];
    uint8_t DI_FIRMWARE_REVISION_STRING_VAL[7];
    uint8_t DI_SOFTWARE_REVISION_STRING_VAL[7];
    uint8_t DI_SYSTEM_ID_VAL[8];
    uint8_t DI_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST_VAL[6];
    uint8_t DI_PNP_ID_VAL[7];
    uint8_t BS_BATTERY_LEVEL_VAL[1];
}environmental_sensor_t;

/******************************* PRIVATE PROTOTYPE ************************************/
static void APP_RBLE_CallBack( RBLE_MODE mode );
static bool APP_RBLE_Active_CallBack( void );
static bool APP_RBLE_Exit_CallBack( void );

/* --------------------------- Generic Access Profile --------------------------------*/
static RBLE_STATUS APP_GAP_Reset_Command();
#ifdef ENABLE_SET_NAME
static RBLE_STATUS APP_GAP_Set_Name_Command( void );
#endif
static RBLE_STATUS APP_GAP_Broadcast_Enable_Command();
static RBLE_STATUS APP_GAP_Set_Bonding_Mode_Command();
static RBLE_STATUS APP_GAP_Set_Security_Request_Command();
static RBLE_STATUS APP_GAP_Bonding_Response_Command();
static RBLE_STATUS APP_GAP_Disconnection_Command( void );
static RBLE_STATUS APP_GAP_Get_Device_Info_Command( void );

static void APP_GAP_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Reset_CallBack( RBLE_GAP_EVENT *event );
#ifdef ENABLE_SET_NAME
static BOOL APP_GAP_Set_Name_CallBack( RBLE_GAP_EVENT *event );
#endif
static bool APP_GAP_Broadcast_Enable_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Connection_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Disconnection_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Bonding_Callback( RBLE_GAP_EVENT *event );
static bool APP_GAP_Set_Bonding_Mode_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Set_Security_Request_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_RPA_Resolved_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Bonding_Request_CallBack( RBLE_GAP_EVENT *event );
static bool APP_GAP_Get_Device_Info_CallBack(RBLE_GAP_EVENT *event );

static RBLE_STATUS APP_ESS_Enable_Command(void);
static RBLE_STATUS APP_ESS_Disable_Command(void);
static RBLE_STATUS APP_ESS_Set_Value_Command(uint16_t char_hdl, uint8_t * p_data);


/* --------------------------------- Utility -----------------------------------------*/
static void  APP_RBLE_Exit_Command( void );
void update_BdAddress(uint8_t *ptr);


/**************************** PRIVATE Memory Declaration ******************************/
static uint16_t         RunCmd[RUN_COMMAND_QUEUE_SIZE];    // RBLE_APP_CMD_NO_enum value
static uint8_t          WrPtr     ;
static uint8_t          RdPtr     ;
static bsp_lock_t rdwr_lock;

/**************************** PUBLIC Memory Declaration *******************************/
uint16_t         ConnectionHdl = ILLEGAL_CONNECTION_HANDLE;
bool             InBonding     = false;
RBLE_BD_ADDR     BondedAddr    ;
uint8_t          SecStatus     ;
APP_KEY_DATA     LocalKey      ;
APP_KEY_DATA     RemoteKey     ;
uint16_t         RcvEdiv       ;
RBLE_RAND_NB     RcvNb         ;
uint8_t          ConnectionIdx ;
uint8_t          ConnectionType;
RBLE_BD_ADDR     RemoteDevice  ;
bool             FoundFlg      ;
bool             CheckedIrkFlg ;
uint8_t          Flash_cmd = NULL;
RBLE_BD_ADDR BdAddress = {{ 0x45, 0x01, 0xDE, 0xFA, 0xFE, 0xCA }};
environmental_sensor_t  es_ble;


#if (USE_LCD==true)
extern RBLE_MODE		rble_mode;
#endif
/******************************* Program Area *****************************************/

/******************************* Program Area *****************************************/

/******************************************************************************************/
/**
 *  @brief      Application RBLE Exit Command Execute
 *
 *  @retval     None
 */
/******************************************************************************************/
/*
 * Updating the Unique ID of the Chip for the Board ID. This Gives the Unique ID for the RL78/G1D BLE
 */
void update_BdAddress(uint8_t *ptr)
{
    uint8_t i=0;

    for( i=0; i < sizeof(RBLE_BD_ADDR); i++){
        BdAddress.addr[i]= *ptr++;
    }
}
static void  APP_RBLE_Exit_Command( void )
{
    // trap when exit
    while (1)
    {
        /* show message to power cycle or reset to RL78/G1D device */

    }
}

/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Device Info
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Get_Device_Info_Command( void )
{
    RBLE_STATUS ret_status;

    /* API Call */
    ret_status = RBLE_GAP_Get_Device_Info();

    return( ret_status );
}


/**************************************************************************************/
/**
 *  @brief      GAP Get Device Info Callback function
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Get_Device_Info_CallBack(RBLE_GAP_EVENT *event )
{
    bool        ret = false;
    static RBLE_BD_ADDR BdAddress_Read;

    if(RBLE_OK == event->param.get_dev_ver.status)
    {
        ret = true;
        BdAddress_Read = event->param.get_dev_ver.addr;
        /* compare BD Address */
        if (APP_Compare_BDAddr (&BdAddress_Read, &BdAddress)){
            /* API Call */
            APP_Set_RunCmd(GAP_SET_BONDING_MODE_CMD);
        }
        else{   // write new BD address

            APP_Set_RunCmd(VS_Enable_CMD);
            /* Write BD Address */
            Flash_cmd = RBLE_VS_FLASH_CMD_START;
            APP_Set_RunCmd(VS_Flash_Management_CMD);        // start calling for BD Address write
        }
    }
    return ( ret );
}

/**************************************************************************************/
/**
 *  @brief      Application Update Environment Parameter
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/


/**************************************************************************************/
/**
 *  @brief      Application Initialize
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
bool APP_Init( void * p_uart )
{
    bool        ret = true;
    RBLE_STATUS ret_status;

    /* Initialize rBLE */
    APP_Disp(APP_DISP_CMD, "BLE Initialization");
    ret_status = RBLE_Init(p_uart, &APP_RBLE_CallBack);
    if ( RBLE_OK != ret_status ) {
        ret = false;
    }
    
    /* Initialize Bonding Information Buffer */
    SecStatus      = RBLE_SMP_SEC_NONE;
    memset(&BondedAddr, 0, sizeof(RBLE_BD_ADDR));
    memset(&LocalKey  , 0, sizeof(APP_KEY_DATA));
    memset(&RemoteKey , 0, sizeof(APP_KEY_DATA));

    return( ret );
}

/**************************************************************************************/
/**
 *  @brief      Application Command Execute
 *
 *  @retval     None
 */
/******************************************************************************************/
RBLE_STATUS APP_Run( void )
{
    RBLE_STATUS ret_status = RBLE_OK;
    /* Loop until Run Command Queue is empty */
    while(RdPtr != WrPtr)
    {
        switch(RunCmd[RdPtr])
        {
            case RBLE_APP_CMD_NON:
                break;

            /* --------------------------- Generic Access Profile --------------------------------*/
            case GAP_RESET_CMD:
                ret_status = APP_GAP_Reset_Command();
                LOGGER(APP_GAP_Reset_Command);
                break;
#ifdef ENABLE_SET_NAME
            case GAP_SET_NAME_CMD:
                ret_status = APP_GAP_Set_Name_Command();
                LOGGER(APP_GAP_Set_Name_Command);
                break;
#endif
            case GAP_BROADCAST_ENABLE_CMD:
                ret_status = APP_GAP_Broadcast_Enable_Command();
                LOGGER(APP_GAP_Broadcast_Enable_Command);
                break;
            case GAP_SET_BONDING_MODE_CMD:
                ret_status = APP_GAP_Set_Bonding_Mode_Command();
                LOGGER(APP_GAP_Set_Bonding_Mode_Command);
                break;
            case GAP_SET_SECURITY_REQUEST_CMD:
                ret_status = APP_GAP_Set_Security_Request_Command();
                LOGGER(APP_GAP_Set_Security_Request_Command);
                break;
            case GAP_BONDING_RESPONSE_CMD:
                ret_status = APP_GAP_Bonding_Response_Command();
                LOGGER(APP_GAP_Bonding_Response_Command);
                break;
            case GAP_DISCONNECT_CMD:
                ret_status = APP_GAP_Disconnection_Command();
                LOGGER(APP_GAP_Disconnection_Command);
                break;
            case GAP_GET_DEVICE_INFO_CMD:
                ret_status = APP_GAP_Get_Device_Info_Command();
                LOGGER(APP_GAP_Get_Device_Info_Command);
                break;
            case GAP_AUTHORIZED_IND_CMD:
                ret_status = RBLE_GAP_Authorized_Ind(ConnectionHdl);
                LOGGER(RBLE_GAP_Authorized_Ind);
                break;

            /* --------------------------- Security Manager -------------------------------------*/
            case SM_START_ENC_CMD:
                ret_status = APP_SM_Encryption_Start_Command();
                LOGGER(APP_SM_Encryption_Start_Command);
                break;

            case SM_CHK_BD_ADDR_REQ_RESP_CMD:
                ret_status = APP_SM_Check_BDAddr_Response_Command();
                LOGGER(APP_GAP_Bonding_Response_Command);
                break;
            case SM_TK_REQ_RESP_CMD:
                ret_status = APP_SM_TK_Response_Command();
                LOGGER(APP_SM_TK_Response_Command);
                break;
            case SM_LTK_REQ_RESP_CMD:
                ret_status = APP_SM_LTK_Response_Command();
                LOGGER(APP_SM_LTK_Response_Command);
                break;
            case SM_LTK_REQ_FOR_ENC_RESP_CMD:
                ret_status = APP_SM_LTK_Response_For_Encryption_Command();
                LOGGER(APP_SM_LTK_Response_For_Encryption_Command);
                break;
            case SM_IRK_REQ_RESP_CMD:
                ret_status = APP_SM_IRK_Response_Command();
                LOGGER(APP_SM_IRK_Response_Command);
                break;

           /* ------------------------- Vendor Specific Profile-------------------------------*/
            case VS_Enable_CMD:
                 ret_status = APP_VS_Enable_Command();
                 LOGGER(APP_VS_Enable_Command);
                 break;
            case VS_Write_BdAddress_CMD:
                 ret_status = APP_VS_Write_BdAddress_Command();
                 LOGGER(APP_VS_Write_BdAddress_Command);
                 break;
            case VS_Flash_Management_CMD:
                 ret_status = APP_VS_Flash_Management_Command();
                 LOGGER(APP_VS_Flash_Management_Command);
                 break;
            case VS_Flash_Access_CMD:
                 ret_status = APP_VS_Flash_Management_Command();
                 LOGGER(APP_VS_Flash_Management_Command);
                 break;

            case RBLE_APP_ENVIRONMENT_SENSING_SERVER_Enable_CMD:
                 ret_status = APP_ESS_Enable_Command();
                 LOGGER(APP_ESS_Enable_Command);
            break;

            case RBLE_APP_ENVIRONMENT_SENSING_SERVER_Disable_CMD:
                ret_status = APP_ESS_Disable_Command ();
                LOGGER(APP_ESS_Disable_Command);
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_APPARENT_WIND_DIRECTION_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_APPARENT_WIND_DIRECTION_VAL,
                                           &es_ble.ES_APPARENT_WIND_DIRECTION_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_APPARENT_WIND_SPEED_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_APPARENT_WIND_SPEED_VAL,
                                           &es_ble.ES_APPARENT_WIND_SPEED_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_DEW_POINT_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_DEW_POINT_VAL, &es_ble.ES_DEW_POINT_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_ELEVATION_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_ELEVATION_VAL, &es_ble.ES_ELEVATION_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_GUST_FACTOR_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_GUST_FACTOR_VAL, &es_ble.ES_GUST_FACTOR_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_HEAT_INDEX_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_HEAT_INDEX_VAL, &es_ble.ES_HEAT_INDEX_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_HUMIDITY_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_HUMIDITY_VAL, &es_ble.ES_HUMIDITY_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_IRRADIANCE_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_IRRADIANCE_VAL, &es_ble.ES_IRRADIANCE_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_POLLEN_CONCENTRATION_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_POLLEN_CONCENTRATION_VAL,
                                           &es_ble.ES_POLLEN_CONCENTRATION_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_RAINFALL_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_RAINFALL_VAL, &es_ble.ES_RAINFALL_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_PRESSURE_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_PRESSURE_VAL, &es_ble.ES_PRESSURE_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_TEMPERATURE_CMD:
            {
                es_ble.ES_TEMPERATURE_VAL[0]++;
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_TEMPERATURE_VAL, &es_ble.ES_TEMPERATURE_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_TRUE_WIND_DIRECTION_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_TRUE_WIND_DIRECTION_VAL,
                                           &es_ble.ES_TRUE_WIND_DIRECTION_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_TRUE_WIND_SPEED_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_TRUE_WIND_SPEED_VAL,
                                           &es_ble.ES_TRUE_WIND_SPEED_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_UV_INDEX_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_UV_INDEX_VAL, &es_ble.ES_UV_INDEX_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_WIND_CHILL_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_WIND_CHILL_VAL, &es_ble.ES_WIND_CHILL_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_BAROMETRIC_PRESSURE_TREND_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_BAROMETRIC_PRESSURE_TREND_VAL,
                                           &es_ble.ES_BAROMETRIC_PRESSURE_TREND_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_MAGNETIC_DECLINATION_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_DECLINATION_VAL,
                                           &es_ble.ES_MAGNETIC_DECLINATION_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_MAGNETIC_FLUX_DENSITY___2D_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_FLUX_DENSITY___2D_VAL,
                                           &es_ble.ES_MAGNETIC_FLUX_DENSITY___2D_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_MAGNETIC_FLUX_DENSITY___3D_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_FLUX_DENSITY___3D_VAL,
                                           &es_ble.ES_MAGNETIC_FLUX_DENSITY___3D_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_MANUFACTURER_NAME_STRING_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_MANUFACTURER_NAME_STRING_VAL,
                                           &es_ble.DI_MANUFACTURER_NAME_STRING_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_MODEL_NUMBER_STRING_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_MODEL_NUMBER_STRING_VAL,
                                           &es_ble.DI_MODEL_NUMBER_STRING_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_SERIAL_NUMBER_STRING_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_SERIAL_NUMBER_STRING_VAL,
                                           &es_ble.DI_SERIAL_NUMBER_STRING_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_HARDWARE_REVISION_STRING_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_HARDWARE_REVISION_STRING_VAL,
                                           &es_ble.DI_HARDWARE_REVISION_STRING_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_FIRMWARE_REVISION_STRING_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_FIRMWARE_REVISION_STRING_VAL,
                                           &es_ble.DI_FIRMWARE_REVISION_STRING_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_SOFTWARE_REVISION_STRING_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_SOFTWARE_REVISION_STRING_VAL,
                                           &es_ble.DI_SOFTWARE_REVISION_STRING_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_SYSTEM_ID_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_SYSTEM_ID_VAL, &es_ble.DI_SYSTEM_ID_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST_CMD:
            {
                APP_ESS_Set_Value_Command (
                        ENVIRONMENTAL_SENSING_HDL_DI_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST_VAL,
                        &es_ble.DI_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_PNP_ID_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_DI_PNP_ID_VAL, &es_ble.DI_PNP_ID_VAL[0]);
            }
            break;

            case RBLE_APP_ENVIRONMENTAL_SENSING_Set_Param_BATTERY_LEVEL_CMD:
            {
                APP_ESS_Set_Value_Command (ENVIRONMENTAL_SENSING_HDL_BS_BATTERY_LEVEL_VAL,
                                           &es_ble.BS_BATTERY_LEVEL_VAL[0]);
            }
            break;

            case EXIT_CMD:
                 APP_RBLE_Exit_Command();
                 LOGGER(APP_RBLE_Exit_Command);
                 ret_status = true;
                 break;

            default:
                ret_status = RBLE_UNSPECIFIED_ERROR;
                break;
        }

        /* Operate Run Command Queue Pointer as a Ring Buffer */
        while(R_BSP_SoftwareLock(&rdwr_lock)!=SSP_SUCCESS);
        RdPtr = (uint8_t)(((RUN_COMMAND_QUEUE_SIZE - 1U) == RdPtr) ? (0) : (RdPtr + 1U));
        R_BSP_SoftwareUnlock(&rdwr_lock);
    }

    return ret_status;
}

/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Reset
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Reset_Command( void )
{
    RBLE_STATUS ret_status;

    ConnectionHdl = ILLEGAL_CONNECTION_HANDLE;
    APP_Disp(APP_DISP_CMD, "Reset");
    ret_status = RBLE_GAP_Reset( &APP_GAP_CallBack, &APP_SM_CallBack );

    return( ret_status );
}
#ifdef ENABLE_SET_NAME
static RBLE_STATUS APP_GAP_Set_Name_Command( void )
{
    RBLE_STATUS ret_status;
    RBLE_BD_NAME dev_name = {.name = {0}, .namelen = 0};

    strncpy((char *)dev_name.name, DEVICE_NAME, sizeof(dev_name.name) - 1);
    dev_name.namelen = (uint8_t)strlen((char *)dev_name.name);

    ret_status = RBLE_GAP_Set_Name(&dev_name);

    return( ret_status );
}
#endif
/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Broadcast Enable
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Broadcast_Enable_Command( void )
{
    RBLE_STATUS ret_status;

    APP_Disp(APP_DISP_CMD, "Broadcast Enable");
    ret_status = RBLE_GAP_Broadcast_Enable( RBLE_Broadcast_Enable_Param[0].disc_mode, RBLE_Broadcast_Enable_Param[0].conn_mode, (RBLE_ADV_INFO *)&(RBLE_Broadcast_Enable_Param[0].adv_info) );

    return( ret_status );
}

/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Set Bonding Mode
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Set_Bonding_Mode_Command( void )
{
    RBLE_STATUS ret_status;

    APP_Disp(APP_DISP_CMD, "Set Bonding Mode");
    ret_status = RBLE_GAP_Set_Bonding_Mode(RBLE_GAP_BONDABLE);

    return( ret_status );
}

/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Set Security Request
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Set_Security_Request_Command( void )
{
    RBLE_STATUS ret_status;

    APP_Disp(APP_DISP_CMD, "Set Security Request");
    ret_status = RBLE_GAP_Set_Security_Request( RBLE_GAP_SEC1_NOAUTH_PAIR_ENC );

    return( ret_status );
}

/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Bonding Response
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Bonding_Response_Command( void )
{
    RBLE_STATUS ret_status;
    static RBLE_BOND_RESP_PARAM Param = {
        0x0000,                                         /* Connection handle           */
        RBLE_OK,                                        /* accept or reject bonding    */
        #ifdef USE_PAIRING_JUSTWORKS
        RBLE_IO_CAP_NO_INPUT_NO_OUTPUT,                 /* IO capabilities             */
        #else
        RBLE_IO_CAP_DISPLAY_ONLY,                       /* IO capabilities             */
        #endif
        RBLE_OOB_AUTH_DATA_NOT_PRESENT,                 /* OOB flag                    */
        #ifdef USE_PAIRING_JUSTWORKS
        RBLE_AUTH_REQ_NO_MITM_BOND,                     /* Authentication Requirements */
        #else
        RBLE_AUTH_REQ_MITM_BOND,                        /* Authentication Requirements */
        #endif
        RBLE_SMP_MAX_ENC_SIZE_LEN,                      /* Encryption key size         */
        RBLE_KEY_DIST_ENCKEY | RBLE_KEY_DIST_IDKEY,     /* Initiator key distribution  */
        RBLE_KEY_DIST_ENCKEY,                           /* Responder key distribution  */
        0x00                                            /* Reserved                    */
    };
#ifdef USE_PAIRING_JUSTWORKS
    memcpy(&Param, &RBLE_Bond_Resp_Param[1], sizeof(Param));
#else

#endif

    Param.conhdl = ConnectionHdl;
    APP_Disp(APP_DISP_CMD, "Bonding Response");
    ret_status = RBLE_GAP_Bonding_Response( &Param );

    return( ret_status );
}

/**************************************************************************************/
/**
 *  @brief      rBLE API Call : GAP Disconnection
 *
 *  @retval     rBLE API Return Status
 */
/******************************************************************************************/
static RBLE_STATUS APP_GAP_Disconnection_Command( void )
{
    RBLE_STATUS ret_status = false;

    if(ConnectionHdl != ILLEGAL_CONNECTION_HANDLE)
    {
        APP_Disp(APP_DISP_CMD, "BLE Disconnect");
        ret_status = RBLE_GAP_Disconnect( ConnectionHdl );
    }

    return( ret_status );
}



/********************  Call Back API ********************/

/******************************************************************************************/
/**
 *  @brief      Event Handle : rBLE
 *
 *  @param      mode    rBLE Mode , defined by RBLE_MODE_enum
 *
 *  @retval     None
 */
/******************************************************************************************/
static void APP_RBLE_CallBack( RBLE_MODE mode )
{
    switch(mode){
        case RBLE_MODE_INITIALIZE:
            break;
        case RBLE_MODE_ACTIVE:
            APP_RBLE_Active_CallBack();
            break;
        case RBLE_MODE_RESET:
            break;
        case RBLE_MODE_SLEEP:
            break;
        case RBLE_MODE_ERROR:
            APP_RBLE_Exit_CallBack();
            break;
        default:
            break;
    }
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : rBLE Active Event
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_RBLE_Active_CallBack( void )
{
    APP_Disp(APP_DISP_EVT, "BLE Active");
    APP_Set_RunCmd(GAP_RESET_CMD);
    return( true );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : rBLE Exit Event
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_RBLE_Exit_CallBack( void )
{
    APP_Disp(APP_DISP_FATAL, "Fatal");
    return( true );
}

/* --------------------------- Generic Access Profile --------------------------------*/

/******************************************************************************************/
/**
 *  @brief      Event Handler : GAP Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     None
 */
/******************************************************************************************/
static void APP_GAP_CallBack( RBLE_GAP_EVENT *event )
{
    switch( event->type ) {
        case RBLE_GAP_EVENT_RESET_RESULT:
            APP_GAP_Reset_CallBack( event );
            LOGGER(APP_GAP_Reset_CallBack);
            break;
#ifdef ENABLE_SET_NAME
        case RBLE_GAP_EVENT_SET_NAME_COMP:
            APP_GAP_Set_Name_CallBack( event );
            LOGGER(APP_GAP_Set_Name_CallBack);
            break;
#endif
        case RBLE_GAP_EVENT_OBSERVATION_ENABLE_COMP:
            LOGGER(RBLE_GAP_EVENT_OBSERVATION_ENABLE_COMP);
            break;
        case RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP:
            LOGGER(RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP);
            break;
        case RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP:
            APP_GAP_Broadcast_Enable_CallBack( event );
            LOGGER(APP_GAP_Broadcast_Enable_CallBack);
            break;
        case RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP:
            LOGGER(RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP);
            break;
        case RBLE_GAP_EVENT_SET_BONDING_MODE_COMP:
            APP_GAP_Set_Bonding_Mode_CallBack( event );
            LOGGER(APP_GAP_Set_Bonding_Mode_CallBack);
            break;
        case RBLE_GAP_EVENT_SET_SECURITY_REQUEST_COMP:
            APP_GAP_Set_Security_Request_CallBack( event );
            LOGGER(APP_GAP_Set_Security_Request_CallBack);
            break;
        case RBLE_GAP_EVENT_GET_DEVICE_INFO_COMP:
            APP_GAP_Get_Device_Info_CallBack( event );
            LOGGER(APP_GAP_Get_Device_Info_CallBack);
            break;
        case RBLE_GAP_EVENT_RPA_RESOLVED:
            APP_GAP_RPA_Resolved_CallBack( event );
            LOGGER(APP_GAP_RPA_Resolved_CallBack);
            break;
        case RBLE_GAP_EVENT_GET_WHITE_LIST_SIZE_COMP:
            LOGGER(RBLE_GAP_EVENT_GET_WHITE_LIST_SIZE_COMP);
            break;
        case RBLE_GAP_EVENT_ADD_TO_WHITE_LIST_COMP:
            LOGGER(RBLE_GAP_EVENT_ADD_TO_WHITE_LIST_COMP);
            break;
        case RBLE_GAP_EVENT_DEL_FROM_WHITE_LIST_COMP:
            LOGGER(RBLE_GAP_EVENT_DEL_FROM_WHITE_LIST_COMP);
            break;
        case RBLE_GAP_EVENT_GET_REMOTE_DEVICE_NAME_COMP:
            LOGGER(RBLE_GAP_EVENT_GET_REMOTE_DEVICE_NAME_COMP);
            break;
        case RBLE_GAP_EVENT_GET_REMOTE_DEVICE_INFO_COMP:
            LOGGER(RBLE_GAP_EVENT_GET_REMOTE_DEVICE_INFO_COMP);
            break;
        case RBLE_GAP_EVENT_DEVICE_SEARCH_COMP:
            LOGGER(RBLE_GAP_EVENT_DEVICE_SEARCH_COMP);
            break;
        case RBLE_GAP_EVENT_DEVICE_SEARCH_RESULT_IND:
            LOGGER(RBLE_GAP_EVENT_DEVICE_SEARCH_RESULT_IND);
            break;
        case RBLE_GAP_EVENT_SET_RANDOM_ADDRESS_COMP:
            LOGGER(RBLE_GAP_EVENT_SET_RANDOM_ADDRESS_COMP);
            break;
        case RBLE_GAP_EVENT_SET_PRIVACY_FEATURE_COMP:
            LOGGER(RBLE_GAP_EVENT_SET_PRIVACY_FEATURE_COMP);
            break;
        case RBLE_GAP_EVENT_CONNECTION_COMP:
            APP_GAP_Connection_CallBack( event );
            LOGGER(APP_GAP_Connection_CallBack);
            break;
        case RBLE_GAP_EVENT_CONNECTION_CANCEL_COMP:
            LOGGER(RBLE_GAP_EVENT_CONNECTION_CANCEL_COMP);
            break;
        case RBLE_GAP_EVENT_DISCONNECT_COMP:
            APP_GAP_Disconnection_CallBack( event );
            LOGGER(APP_GAP_Disconnection_CallBack);
            break;
        case RBLE_GAP_EVENT_ADVERTISING_REPORT_IND:
            LOGGER(RBLE_GAP_EVENT_ADVERTISING_REPORT_IND);
            break;
        case RBLE_GAP_EVENT_BONDING_COMP:
            APP_GAP_Bonding_Callback( event );
            LOGGER(APP_GAP_Bonding_Callback);
            break;
        case RBLE_GAP_EVENT_BONDING_REQ_IND:
            APP_GAP_Bonding_Request_CallBack( event );
            LOGGER(APP_GAP_Bonding_Request_CallBack);
            break;
        case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_REQ_IND:
            LOGGER(RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_REQ_IND);
            break;
        case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_COMP:
            LOGGER(RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_COMP);
            break;
        case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_RESPONSE:
            LOGGER(RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_RESPONSE);
            break;
        case RBLE_GAP_EVENT_CHANNEL_MAP_REQ_COMP:
            LOGGER(RBLE_GAP_EVENT_CHANNEL_MAP_REQ_COMP);
            break;
        case RBLE_GAP_EVENT_READ_RSSI_COMP:
            LOGGER(RBLE_GAP_EVENT_READ_RSSI_COMP);
            break;
        case RBLE_GAP_EVENT_WR_CHAR_IND:
            LOGGER(RBLE_GAP_EVENT_WR_CHAR_IND);
            break;
        case RBLE_GAP_EVENT_COMMAND_DISALLOWED_IND:
            LOGGER(RBLE_GAP_EVENT_COMMAND_DISALLOWED_IND);
            break;
        default:
            break;
    }
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Reset Complete Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Reset_CallBack( RBLE_GAP_EVENT *event )
{
    RBLE_STATUS     ret_status = false;

    if(RBLE_OK == event->param.status) {
        ret_status = true;

        if((event->param.reset_result.rBLE_major_ver < RBLE_MAJOR_VERSION) &&
                (event->param.reset_result.rBLE_minor_ver < RBLE_MINOR_VERSION)     )
        {
            /** Improper version numbers */
            ret_status = RBLE_VERSION_FAIL;
            ret_status = false;
        }

        /* Get Device Info */
        APP_Set_RunCmd(GAP_GET_DEVICE_INFO_CMD);
    }

    APP_Disp(APP_DISP_EVT, "Reset Complete");
    APP_Disp_Status(event->param.status);
    return( ret_status );
}
#ifdef ENABLE_SET_NAME
static BOOL APP_GAP_Set_Name_CallBack( RBLE_GAP_EVENT *event )
{
    RBLE_STATUS     ret_status = FALSE;

    if(RBLE_OK == event->param.status) {
        ret_status = TRUE;

        /* Enable Broadcast */
        APP_Set_RunCmd(GAP_BROADCAST_ENABLE_CMD);
    }

    return( ret_status );
}
#endif
/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Broadcast Enable Complete Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Broadcast_Enable_CallBack( RBLE_GAP_EVENT *event )
{
    RBLE_STATUS     ret_status = false;

    if(RBLE_OK == event->param.status) {
        ret_status = true;
    }

    APP_Disp(APP_DISP_EVT, "Broadcast Enable");
    APP_Disp_Status(event->param.status);
    return( ret_status );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Connection Complete Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Connection_CallBack( RBLE_GAP_EVENT *event )
{
    extern void RBLE_Client_Connection(uint32_t status);
    RBLE_STATUS ret_status = false;

    if(RBLE_OK == event->param.status)
    {
        ConnectionHdl  = event->param.conn_comp.connect_info.conhdl;
        RemoteDevice   = event->param.conn_comp.connect_info.peer_addr;
        FoundFlg       = false;
        CheckedIrkFlg  = false;
        ret_status = true;

        /* Call the Enable Function Generated by Bluetooth Developer Studio below */
        APP_Set_RunCmd(RBLE_APP_ENVIRONMENT_SENSING_SERVER_Enable_CMD);

        /* Indicate successful connection */
        RBLE_Client_Connection(1);
    }

    APP_Disp(APP_DISP_EVT, "Connection Complete");
    APP_Disp_Status(event->param.status);

    return( ret_status );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Disconnection Complete Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Disconnection_CallBack( RBLE_GAP_EVENT *event )
{
    extern void RBLE_Client_Connection(uint32_t status);
    ConnectionHdl = ILLEGAL_CONNECTION_HANDLE;
    SSP_PARAMETER_NOT_USED(event);

    APP_Disp(APP_DISP_EVT, "Disconnection Complete");
    APP_Disp_Status(event->param.disconnect.status);
    APP_Disp_Clear();

    /* Indicate disconnection */
    RBLE_Client_Connection(0);

    /* Restart Broadcast for the Next Connection */
    APP_Set_RunCmd(GAP_BROADCAST_ENABLE_CMD);

    return( true );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Set Bonding Mode Complete Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Set_Bonding_Mode_CallBack( RBLE_GAP_EVENT *event )
{
    RBLE_STATUS ret_status = false;

    if(RBLE_OK == event->param.status) {
        ret_status = true;

        /* Set Security Request */
        APP_Set_RunCmd(GAP_SET_SECURITY_REQUEST_CMD);

    }

    APP_Disp(APP_DISP_EVT, "Bonding Mode Complete");
    APP_Disp_Status(event->param.status);
    return( ret_status );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Set Security Request Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Set_Security_Request_CallBack( RBLE_GAP_EVENT *event )
{
    RBLE_STATUS ret_status = false;

    if(RBLE_OK == event->param.set_sec_req.status) {
        ret_status = true;
#ifdef ENABLE_SET_NAME
        /* Set Name */
        APP_Set_RunCmd(GAP_SET_NAME_CMD);
#else
        /* Start Broadcast for the First Connection */
        APP_Set_RunCmd(GAP_BROADCAST_ENABLE_CMD);
#endif
    }

    APP_Disp(APP_DISP_EVT, "Set Security Request");
    APP_Disp_Status(event->param.set_sec_req.status);
    return( ret_status );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP RPA Resolved Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_RPA_Resolved_CallBack( RBLE_GAP_EVENT *event )
{
    if(event != NULL){
        APP_Disp(APP_DISP_EVT, "RPA Resolved");
        return( true );
    }
    return( false);
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Bonding Complete Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Bonding_Callback( RBLE_GAP_EVENT *event )
{
    InBonding      = false;
    SecStatus      = event->param.bonding_comp.sec_prop | RBLE_SMP_BONDED;
    BondedAddr     = RemoteDevice  ;
    if(event != NULL){
        APP_Disp(APP_DISP_EVT, "Bonding Complete");
        APP_Disp_Status(RBLE_OK);
        return( true );
    }
    return( false );
}

/******************************************************************************************/
/**
 *  @brief      Event Respond : GAP Bonding Request Event
 *
 *  @param      event   GAP Event Parameters
 *
 *  @retval     true if succeeded , false if failed
 */
/******************************************************************************************/
static bool APP_GAP_Bonding_Request_CallBack( RBLE_GAP_EVENT *event )
{
    InBonding = true;
    if(event != NULL){
    /* Reset Remote Device Property */
    SecStatus      = RBLE_SMP_SEC_NONE;
    memset(&BondedAddr, 0, sizeof(RBLE_BD_ADDR));
    memset(&LocalKey  , 0, sizeof(APP_KEY_DATA));
    memset(&RemoteKey , 0, sizeof(APP_KEY_DATA));

    /* Reply Bonding Response */
    APP_Set_RunCmd(GAP_BONDING_RESPONSE_CMD);
    APP_Disp(APP_DISP_EVT, "Bonding Request");
    return( true );
    }
    return( false );
}

static ENVIRONMENTAL_SENSING_SERVER_PARAM environmental_sensing_server_param =
{
  .es_descriptor_value_changed_char_cccd = 0,
  .es_apparent_wind_direction_char_cccd = 0,
  .es_apparent_wind_speed_char_cccd = 0,
  .es_dew_point_char_cccd = 0,
  .es_elevation_char_cccd = 0,
  .es_gust_factor_char_cccd = 0,
  .es_heat_index_char_cccd = 0,
  .es_humidity_char_cccd = 0,
  .es_irradiance_char_cccd = 0,
  .es_pollen_concentration_char_cccd = 0,
  .es_rainfall_char_cccd = 0,
  .es_pressure_char_cccd = 0,
  .es_temperature_char_cccd = 0,
  .es_true_wind_direction_char_cccd = 0,
  .es_true_wind_speed_char_cccd = 0,
  .es_uv_index_char_cccd = 0,
  .es_wind_chill_char_cccd = 0,
  .es_barometric_pressure_trend_char_cccd = 0,
  .es_magnetic_declination_char_cccd = 0,
  .es_magnetic_flux_density___2d_char_cccd = 0,
  .es_magnetic_flux_density___3d_char_cccd = 0,
  .bs_battery_level_char_cccd = 0,
};

static void environmental_sensing_server_cccd_write_service(uint16_t char_hdl, uint16_t value)
{
    switch(char_hdl)
    {
        case ENVIRONMENTAL_SENSING_HDL_ES_DESCRIPTOR_VALUE_CHANGED_CCCD:
        {
            environmental_sensing_server_param.es_descriptor_value_changed_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_APPARENT_WIND_DIRECTION_CCCD:
        {
            environmental_sensing_server_param.es_apparent_wind_direction_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_APPARENT_WIND_SPEED_CCCD:
        {
            environmental_sensing_server_param.es_apparent_wind_speed_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_DEW_POINT_CCCD:
        {
            environmental_sensing_server_param.es_dew_point_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_ELEVATION_CCCD:
        {
            environmental_sensing_server_param.es_elevation_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_GUST_FACTOR_CCCD:
        {
            environmental_sensing_server_param.es_gust_factor_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_HEAT_INDEX_CCCD:
        {
            environmental_sensing_server_param.es_heat_index_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_HUMIDITY_CCCD:
        {
            environmental_sensing_server_param.es_humidity_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_IRRADIANCE_CCCD:
        {
            environmental_sensing_server_param.es_irradiance_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_POLLEN_CONCENTRATION_CCCD:
        {
            environmental_sensing_server_param.es_pollen_concentration_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_RAINFALL_CCCD:
        {
            environmental_sensing_server_param.es_rainfall_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_PRESSURE_CCCD:
        {
            environmental_sensing_server_param.es_pressure_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_TEMPERATURE_CCCD:
        {
            environmental_sensing_server_param.es_temperature_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_TRUE_WIND_DIRECTION_CCCD:
        {
            environmental_sensing_server_param.es_true_wind_direction_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_TRUE_WIND_SPEED_CCCD:
        {
            environmental_sensing_server_param.es_true_wind_speed_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_UV_INDEX_CCCD:
        {
            environmental_sensing_server_param.es_uv_index_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_WIND_CHILL_CCCD:
        {
            environmental_sensing_server_param.es_wind_chill_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_BAROMETRIC_PRESSURE_TREND_CCCD:
        {
            environmental_sensing_server_param.es_barometric_pressure_trend_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_DECLINATION_CCCD:
        {
            environmental_sensing_server_param.es_magnetic_declination_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_FLUX_DENSITY___2D_CCCD:
        {
            environmental_sensing_server_param.es_magnetic_flux_density___2d_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_FLUX_DENSITY___3D_CCCD:
        {
            environmental_sensing_server_param.es_magnetic_flux_density___3d_char_cccd = value;
        }
        break;
        case ENVIRONMENTAL_SENSING_HDL_BS_BATTERY_LEVEL_CCCD:
        {
            environmental_sensing_server_param.bs_battery_level_char_cccd = value;
        }
        break;

        default:
        {
            ;
        }
        break;
    }
}

static void ENVIRONMENTAL_SENSING_SERVER_EVENT_CALLBACK(ENVIRONMENTAL_SENSING_SERVER_EVENT *event)
{
    if(ConnectionHdl == event->conhdl)
    {
        if(RBLE_OK == event->status)
        {
            switch(event->type)
            {
                case ENVIRONMENTAL_SENSING_SERVER_EVENT_ENABLE_COMP:
                {
                    /* Load new values into the BLE Copy */
                }
                break;

                case ENVIRONMENTAL_SENSING_SERVER_EVENT_DISABLE_COMP:
                {
                    ENVIRONMENTAL_SENSING_SERVER_PARAM *p1 = &environmental_sensing_server_param;
                    ENVIRONMENTAL_SENSING_SERVER_PARAM *p2 = &event->param.disable_comp_evt.param;
                    *p1 = *p2;
                }
                break;

                case ENVIRONMENTAL_SENSING_SERVER_EVENT_CCCD_WRITE:
                {
                    environmental_sensing_server_cccd_write_service(event->param.cccd_write_evt.char_hdl,
                                                                    event->param.cccd_write_evt.value);
                }
                break;

                case ENVIRONMENTAL_SENSING_SERVER_EVENT_NOTIFY_COMP:
                case ENVIRONMENTAL_SENSING_SERVER_EVENT_INDICATE_COMP:
                case ENVIRONMENTAL_SENSING_SERVER_EVENT_SET_VALUE_COMP:
                {
                    ;
                }
                break;

                default:
                {
                    ;
                }
                break;
            }

        }

    }
}

static RBLE_STATUS APP_ESS_Enable_Command(void)
{
    RBLE_STATUS rble_err = RBLE_OK;

    if(ConnectionHdl != ILLEGAL_CONNECTION_HANDLE)
    {
        rble_err = ENVIRONMENTAL_SENSING_Server_Enable(ConnectionHdl, ConnectionType,
                                                       &environmental_sensing_server_param, ENVIRONMENTAL_SENSING_SERVER_EVENT_CALLBACK);
    }
    return rble_err;
}

static RBLE_STATUS APP_ESS_Disable_Command(void)
{
    RBLE_STATUS rble_err = RBLE_OK;

    if(ConnectionHdl != ILLEGAL_CONNECTION_HANDLE)
    {
        rble_err = ENVIRONMENTAL_SENSING_Server_Disable(ConnectionHdl);
    }

    return rble_err;
}

static RBLE_STATUS APP_ESS_Set_Value_Command(uint16_t char_hdl, uint8_t * p_data)
{
    RBLE_STATUS rble_err = RBLE_OK;
    uint16_t cccd = 0;

    if(ConnectionHdl != ILLEGAL_CONNECTION_HANDLE)
    {
        switch( char_hdl )
        {
            case ENVIRONMENTAL_SENSING_HDL_ES_DESCRIPTOR_VALUE_CHANGED_VAL:
                cccd = environmental_sensing_server_param.es_descriptor_value_changed_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_APPARENT_WIND_DIRECTION_VAL:
                cccd = environmental_sensing_server_param.es_apparent_wind_direction_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_APPARENT_WIND_SPEED_VAL:
                cccd = environmental_sensing_server_param.es_apparent_wind_speed_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_DEW_POINT_VAL:
                cccd = environmental_sensing_server_param.es_dew_point_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_ELEVATION_VAL:
                cccd = environmental_sensing_server_param.es_elevation_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_GUST_FACTOR_VAL:
                cccd = environmental_sensing_server_param.es_gust_factor_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_HEAT_INDEX_VAL:
                cccd = environmental_sensing_server_param.es_heat_index_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_HUMIDITY_VAL:
                cccd = environmental_sensing_server_param.es_humidity_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_IRRADIANCE_VAL:
                cccd = environmental_sensing_server_param.es_irradiance_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_POLLEN_CONCENTRATION_VAL:
                cccd = environmental_sensing_server_param.es_pollen_concentration_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_RAINFALL_VAL:
                cccd = environmental_sensing_server_param.es_rainfall_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_PRESSURE_VAL:
                cccd = environmental_sensing_server_param.es_pressure_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_TEMPERATURE_VAL:
                cccd = environmental_sensing_server_param.es_temperature_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_TRUE_WIND_DIRECTION_VAL:
                cccd = environmental_sensing_server_param.es_true_wind_direction_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_TRUE_WIND_SPEED_VAL:
                cccd = environmental_sensing_server_param.es_true_wind_speed_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_UV_INDEX_VAL:
                cccd = environmental_sensing_server_param.es_uv_index_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_WIND_CHILL_VAL:
                cccd = environmental_sensing_server_param.es_wind_chill_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_BAROMETRIC_PRESSURE_TREND_VAL:
                cccd = environmental_sensing_server_param.es_barometric_pressure_trend_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_DECLINATION_VAL:
                cccd = environmental_sensing_server_param.es_magnetic_declination_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_FLUX_DENSITY___2D_VAL:
                cccd = environmental_sensing_server_param.es_magnetic_flux_density___2d_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_ES_MAGNETIC_FLUX_DENSITY___3D_VAL:
                cccd = environmental_sensing_server_param.es_magnetic_flux_density___2d_char_cccd;
                break;

            case ENVIRONMENTAL_SENSING_HDL_BS_BATTERY_LEVEL_VAL:
                cccd = environmental_sensing_server_param.es_magnetic_flux_density___2d_char_cccd;
                break;

            default:
                return RBLE_PARAM_ERR;
        }

        if(ENVIRONMENTAL_SENSING_SERVER_DESC_ALL_DISABLE == cccd)
        {
            rble_err = ENVIRONMENTAL_SENSING_Server_Set_Value(char_hdl, p_data);
        }
        else
        {
            rble_err = ENVIRONMENTAL_SENSING_Server_Char_NtfInd(ConnectionHdl, char_hdl, p_data, cccd);
        }
    }

    return rble_err;
}

/* --------------------------- Utility -------------------------------------*/

/******************************************************************************************/
/**
 *  @brief      Disconnection
 *
 *  @retval     None
 */
/******************************************************************************************/
void APP_Disconnect(void)
{
    /* Disconnection */
    APP_Set_RunCmd(GAP_DISCONNECT_CMD);
}

/******************************************************************************************/
/**
 *  @brief      Queueing Command
 *
 *  @param      cmd     Command Value , defined by RBLE_APP_CMD_NO_enum
 *
 *  @retval     None
 */
/******************************************************************************************/
void APP_Set_RunCmd( uint16_t cmd )
{
    RunCmd[WrPtr] = cmd;

    /* Operate Run Command Queue Pointer as a Ring Buffer */
    while(R_BSP_SoftwareLock(&rdwr_lock)!=SSP_SUCCESS);
    WrPtr = (uint8_t)((WrPtr == (RUN_COMMAND_QUEUE_SIZE - 1)) ? (0) : (WrPtr + 1));
    R_BSP_SoftwareUnlock(&rdwr_lock);
}

/******************************************************************************************/
/**
 *  @brief      Compare BD Addresses
 *
 *  @param      a       BD Address
 *  @param      b       BD Address
 *
 *  @retval     true if same , false if different
 */
/******************************************************************************************/
bool APP_Compare_BDAddr(RBLE_BD_ADDR* a, RBLE_BD_ADDR* b)
{
    uint8_t i;

    for(i = 0; i < RBLE_BD_ADDR_LEN; i++)
    {
        if(a->addr[i] != b->addr[i])
        {
            return false;
        }
    }
    return true;
}

void timer_iterator(timer_callback_args_t * p_args)
{
    if(ILLEGAL_CONNECTION_HANDLE != ConnectionHdl)
    {
        bsp_leds_t leds_dks124;
        ssp_err_t err = R_BSP_LedsGet(&leds_dks124);
        static ioport_level_t level = IOPORT_LEVEL_HIGH;
        g_ioport_on_ioport.pinWrite(leds_dks124.p_leds[BSP_LED_LED3], level = (level == IOPORT_LEVEL_HIGH) ? IOPORT_LEVEL_LOW:IOPORT_LEVEL_HIGH);
        APP_Set_RunCmd(RBLE_APP_ENVIRONMENTAL_SENSING_SERVER_Set_Param_TEMPERATURE_CMD);
        SSP_PARAMETER_NOT_USED(err);
    }
    SSP_PARAMETER_NOT_USED(p_args);
}
