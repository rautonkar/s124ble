/**
 ****************************************************************************************
 *
 * @file	rble_api_spps.c
 *
 * @brief	rBLE-Host ScPP Server Role I/F API
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 2156 $
 *
 ****************************************************************************************
 */

#include	<string.h>

#include	"rble_api.h"
#include	"rble_host.h"
#include	"rble_trans.h"

#if (PRF_SEL_SPPS)

/**************************** Define Declaration ****************************************/
/**************************** Structure Declaration *************************************/
/**************************** PUBLIC Memory Declaration *********************************/
/**************************** PRIVATE Memory Declaration ********************************/
/**************************** PRIVATE PROTOTYPE Declaration *****************************/
/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Scan Server Role Enable Command
 *
 *  Sensor Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]  sec_lvl		Security level
 *  @param[in]	con_type	Connection Type
 *  @param[in]	param		Scan Server Parameter
 *  @param[in]	call_back	call_back Address
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_SPP_Server_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_SPP_SERVER_PARAM *param, RBLE_SPPS_EVENT_HANDLER call_back)
{
	RBLE_STATUS			ret = RBLE_OK;
	RBLE_HEADER_t		*head_pnt;
	uint8_t				*Data_p;

	/* Parameter Check */
	if (NULL != call_back) {
		/* Status Check */
		if (RBLE_MODE_ACTIVE != rble_mode) {
			ret = RBLE_STATUS_ERROR;
		} else {
			/* Search Free Call Back Handler Memory */
			if ( NULL != RBLE_SPPS_Info.Scans_Handler ) {
				/* Free Memory None */
				ret = RBLE_ERR;
			} else {
				RBLE_SPPS_Info.conhdl      = conhdl;
				RBLE_SPPS_Info.Scans_Handler = call_back;

				/* Create Command Packet Header */
				head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
				head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
				head_pnt->packet_length = sizeof( uint16_t ) * 2 + sizeof( uint8_t ) * 2;

				head_pnt->opcode		= RBLE_CMD_SPP_SERVER_ENABLE;

				/* Create Command Payloard */
				Data_p = &head_pnt->data[ 0 ];
				RBLE_DATA_SET16( Data_p, conhdl );
				RBLE_DATA_SET8( Data_p, sec_lvl );
				RBLE_DATA_SET8( Data_p, con_type );

				RBLE_DATA_SET16( Data_p, param->s_refresh_ntf_en );
			}
		}
	} else {
		ret = RBLE_PARAM_ERR;
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Scan Server Role Disable Command
 *
 *  Thermometer Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_SPP_Server_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_SPP_SERVER_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Scan Refresh Command
 *
 *  Send Scan Refresh @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		    Connection Handle
 *  @param[in]	s_refresh_val	Scan Refresh request
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_SPP_Server_Send_Refresh(uint16_t conhdl, uint8_t s_refresh_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_SPP_SERVER_SEND_REFRESH;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, s_refresh_val );
	}
	return ret;
}

#endif//#if (PRF_SEL_SPPS)
