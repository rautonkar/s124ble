/**
 ****************************************************************************************
 *
 * @file	rble_api_hghd.c
 *
 * @brief	rBLE-Host HOGP Device Role I/F API
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

#if (PRF_SEL_HGHD)

/**************************** Define Declaration ****************************************/
/**************************** Structure Declaration *************************************/
/**************************** PUBLIC Memory Declaration *********************************/
/**************************** PRIVATE Memory Declaration ********************************/
/**************************** PRIVATE PROTOTYPE Declaration *****************************/
/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief HOGP Device Role Enable Command
 *
 *  HOGP Device Role Enable @n
 *  
 *  @attention		It's necessary to call this function before perform other functions
 *  
 *  @b Contents of management:
 *    -# check parameter
 *    -# register callback function
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl				Connection Handle
 *  @param[in]  sec_lvl				Security level
 *  @param[in]	con_type			Connection Type
 *  @param[in]	hids_inst_num		Number of HID Service Instance
 *  @param[in]	bas_inst_num		Number of Battery Service Instance
 *  @param[in]	param				HID Device Parameter
 *  @param[in]	call_back			call_back Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_HDevice_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_HGP_DEVICE_PARAM *param, RBLE_HGHD_EVENT_HANDLER call_back)
{
	RBLE_STATUS			ret = RBLE_OK;
	RBLE_HEADER_t		*head_pnt;
	int_t				cnt;

	/* Parameter Check */
	if (NULL != call_back) {
		/* Status Check */
		if (RBLE_MODE_ACTIVE != rble_mode) {
			ret = RBLE_STATUS_ERROR;
		} else {
			/* Search Free Call Back Handler Memory */
			if ( NULL != RBLE_HGHD_Info.Hghd_Handler ) {
				/* Free Memory None */
				ret = RBLE_ERR;
			} else {
				RBLE_HGHD_Info.conhdl      = conhdl;
				RBLE_HGHD_Info.Hghd_Handler = call_back;
				
				/* Create Command Packet Header */
				head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
				head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
				head_pnt->packet_length =  sizeof( uint16_t ) + sizeof( uint8_t ) * 4
										 + (sizeof( uint16_t ) * RBLE_HIDS_INST_MAX) * 3 + sizeof( uint8_t ) * RBLE_HIDS_INST_MAX
										 + sizeof( uint16_t ) * RBLE_BAS_INST_MAX;
#if ((RBLE_HIDS_INST_MAX % 2) != 0)
				head_pnt->packet_length += sizeof( uint8_t );
#endif
				head_pnt->opcode		= RBLE_CMD_HGP_HDEVICE_ENABLE;

				/* Create Command Payloard */
				RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
				
				RBLE_Data_Set16( conhdl );
				RBLE_Data_Set8( sec_lvl );
				RBLE_Data_Set8( con_type );
				RBLE_Data_Set8( param->hids_inst_num );
				RBLE_Data_Set8( param->bas_inst_num );
				for (cnt = 0; cnt < RBLE_HIDS_INST_MAX; cnt++) {
					RBLE_Data_Set16( param->report_input_ntf_en[cnt] );
				}
				for (cnt = 0; cnt < RBLE_HIDS_INST_MAX; cnt++) {
					RBLE_Data_Set16( param->kb_report_ntf_en[cnt] );
				}
				for (cnt = 0; cnt < RBLE_HIDS_INST_MAX; cnt++) {
					RBLE_Data_Set16( param->mo_report_ntf_en[cnt] );
				}
				for (cnt = 0; cnt < RBLE_HIDS_INST_MAX; cnt++) {
					RBLE_Data_Set8( param->protocol_mode_val[cnt] );
				}
#if ((RBLE_HIDS_INST_MAX % 2) != 0)
				RBLE_Data_Set8( 0 );		/* skip to pad */
#endif
				for (cnt = 0; cnt < RBLE_BAS_INST_MAX; cnt++) {
					RBLE_Data_Set16( param->battery_level_ntf_en[cnt] );
				}
				
				RBLE_Data_Set_End( );		/* End Packet Data Setting */
			}
		}
	} else {
		ret = RBLE_PARAM_ERR;
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief HOGP Device Role Disable Command
 *
 *  HOGP Device Role Disable @n
 *  
 *  @attention		It's necessary to call this function before perform other functions
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl				Connection Hnadle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_HDevice_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_HGP_HDEVICE_DISABLE;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Report Value Command
 *
 *  Send Report Value @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	inst_idx	Service Instance Index
 *  @param[in]	*report	    Pointer to Report Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_HDevice_Send_Report(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report)
{
	RBLE_STATUS 			ret = RBLE_OK;
	RBLE_HEADER_t			*head_pnt;
	uint8_t					cnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 4 + sizeof( uint8_t ) * RBLE_HIDS_REPORT_MAX + sizeof( uint16_t );
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
		head_pnt->packet_length += sizeof( uint8_t );
#endif
		head_pnt->opcode		= RBLE_CMD_HGP_HDEVICE_SEND_REPORT;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( 0 );
		RBLE_Data_Set8( report->device_type );
		RBLE_Data_Set8( report->report_type );
		for (cnt = 0; cnt < RBLE_HIDS_REPORT_MAX; cnt++) {
			RBLE_Data_Set8( report->value[cnt] );
		}
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
		RBLE_Data_Set8( report->reserved );
#endif
		RBLE_Data_Set16( report->value_size );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Battery Level Command
 *
 *  Send Battery Level @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		    Connection Handle
 *  @param[in]	inst_idx	    Service Instance Index
 *  @param[in]	battery_level   Battery Level
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_HDevice_Send_Battery_Level(uint16_t conhdl, uint8_t inst_idx, uint8_t battery_level)
{
	RBLE_STATUS 			ret = RBLE_OK;
	RBLE_HEADER_t			*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_HGP_HDEVICE_SEND_BATTERY_LEVEL;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( battery_level );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

#endif//#if (PRF_SEL_HGHD)
