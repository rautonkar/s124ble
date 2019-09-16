/**
 ****************************************************************************************
 *
 * @file	rble_api_sppc.c
 *
 * @brief	rBLE-Host ScPP Client Role I/F API
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

#if (PRF_SEL_SPPC)

/**************************** Define Declaration ****************************************/
/**************************** Structure Declaration *************************************/
/**************************** PUBLIC Memory Declaration *********************************/
/**************************** PRIVATE Memory Declaration ********************************/
/**************************** PRIVATE PROTOTYPE Declaration *****************************/
/******************************* Program Erea *************************************/

/******************************************************************************************/
/**
 *  @brief Scan Client Role Enable Command
 *
 *  Blood Scan Client Role Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	con_type	Connection Type
 *  @param[in]	*bls		Blood Pressure Service Infomation
 *  @param[in]	*dis		Device Information Service Infomation
 *  @param[in]	call_back	call_back Address
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_SPP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_SPS_CONTENT *sps, RBLE_SCANS_INTV_WINDOW_PARAM *s_intv_window, RBLE_SPPC_EVENT_HANDLER call_back)
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
			if ( NULL != RBLE_SPPC_Info.Scanc_Handler ) {
				/* Free Memory None */
				ret = RBLE_ERR;
			} else {
				RBLE_SPPC_Info.conhdl      = conhdl;
				RBLE_SPPC_Info.Scanc_Handler = call_back;

				/* Create Command Packet Header */
				head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
				head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
				head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 2
										 + sizeof( uint16_t ) * 7 + sizeof( uint8_t ) * 4
										 + sizeof( uint16_t ) * 2;

				head_pnt->opcode		= RBLE_CMD_SPP_CLIENT_ENABLE;

				/* Create Command Payloard */
				Data_p = &head_pnt->data[ 0 ];
				RBLE_DATA_SET16( Data_p, conhdl );
				RBLE_DATA_SET8( Data_p, con_type );
				RBLE_DATA_SET8( Data_p, 0 );

				RBLE_DATA_SET16( Data_p, sps->shdl );
				RBLE_DATA_SET16( Data_p, sps->ehdl );
				RBLE_DATA_SET16( Data_p, sps->intv_window_char_hdl );
				RBLE_DATA_SET16( Data_p, sps->intv_window_val_hdl );
				RBLE_DATA_SET8( Data_p, sps->intv_window_prop );
				RBLE_DATA_SET8( Data_p, sps->reserved1 );
				RBLE_DATA_SET16( Data_p, sps->refresh_char_hdl );
				RBLE_DATA_SET16( Data_p, sps->refresh_val_hdl );
				RBLE_DATA_SET16( Data_p, sps->refresh_cfg_hdl );
				RBLE_DATA_SET8( Data_p, sps->refresh_prop );
				RBLE_DATA_SET8( Data_p, sps->reserved2 );

				RBLE_DATA_SET16( Data_p, s_intv_window->le_scan_interval );
				RBLE_DATA_SET16( Data_p, s_intv_window->le_scan_window );
			}
		}
	} else {
		ret = RBLE_PARAM_ERR;
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Scan Client Role Disable Command
 *
 *  Blood Scan Client Role Disable @n
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
RBLE_STATUS RBLE_SPP_Client_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_SPP_CLIENT_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Characteristic Command
 *
 *  Write Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	cfg_val		Configure Value
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_SPP_Client_Write_Char(uint16_t conhdl, uint16_t cfg_val)
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
		head_pnt->packet_length = sizeof( uint16_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_SPP_CLIENT_WRITE_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET16( Data_p, cfg_val );
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Set Interval Window Value Command
 *
 *  Read Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	char_code	Characteristic Code
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_SPP_Client_Write_Interval(uint16_t conhdl, RBLE_SCANS_INTV_WINDOW_PARAM *s_intv_window)
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
		head_pnt->packet_length = sizeof( uint16_t )  + sizeof( uint16_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_SPP_CLIENT_SET_INTERVAL;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );

		RBLE_DATA_SET16( Data_p, s_intv_window->le_scan_interval );
		RBLE_DATA_SET16( Data_p, s_intv_window->le_scan_window );
	}
	return ret;
}

#endif//#if (PRF_SEL_SPPC)
