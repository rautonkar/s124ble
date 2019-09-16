/**
 ****************************************************************************************
 *
 * @file		rble_api_tipc.c
 *
 * @brief	rBLE-Host Time Client I/F API
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: v2.00.00 $
 *
 ****************************************************************************************
 */

#include	<string.h>

#include	"rble_api.h"
#include	"rble_host.h"
#include	"rble_trans.h"

#if (PRF_SEL_TIPC)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Time Client Enable
 *
 *  Time Client Role enable@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Store Call Back
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl				Connection Handle
 *  @param[in]	con_type			Connection Type
 *  @param[in]	*cts				Current Time Service Information
 *  @param[in]	*ndcs				Next DST Change Service Information
 *  @param[in]	*rtus				Reference Time Update Service Information
 *  @param[in]	call_back			Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_CTS_CONTENT *cts, RBLE_NDCS_CONTENT *ndcs, RBLE_RTUS_CONTENT *rtus, RBLE_TIPC_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		if ( NULL == RBLE_TIPC_Info.Tic_Handler ) {
			RBLE_TIPC_Info.conhdl		= conhdl;
			RBLE_TIPC_Info.Tic_Handler	= call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )		/* reserved */
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_TIP_CLIENT_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, con_type );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		
		RBLE_DATA_SET16( Data_p, cts->shdl );
		RBLE_DATA_SET16( Data_p, cts->ehdl );
		RBLE_DATA_SET16( Data_p, cts->current_time_char_hdl );
		RBLE_DATA_SET16( Data_p, cts->current_time_val_hdl );
		RBLE_DATA_SET16( Data_p, cts->current_time_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, cts->current_time_prop );
		RBLE_DATA_SET8(  Data_p, cts->reserved1 );
		RBLE_DATA_SET16( Data_p, cts->local_time_info_char_hdl );
		RBLE_DATA_SET16( Data_p, cts->local_time_info_val_hdl );
		RBLE_DATA_SET8(  Data_p, cts->local_time_info_prop );
		RBLE_DATA_SET8(  Data_p, cts->reserved2 );
		RBLE_DATA_SET16( Data_p, cts->ref_time_info_char_hdl );
		RBLE_DATA_SET16( Data_p, cts->ref_time_info_val_hdl );
		RBLE_DATA_SET8(  Data_p, cts->ref_time_info_prop );
		RBLE_DATA_SET8(  Data_p, cts->reserved3 );
		
		RBLE_DATA_SET16( Data_p, ndcs->shdl );
		RBLE_DATA_SET16( Data_p, ndcs->ehdl );
		RBLE_DATA_SET16( Data_p, ndcs->time_dst_char_hdl );
		RBLE_DATA_SET16( Data_p, ndcs->time_dst_val_hdl );
		RBLE_DATA_SET8(  Data_p, ndcs->time_dst_prop );
		RBLE_DATA_SET8(  Data_p, ndcs->reserved1 );
		
		RBLE_DATA_SET16( Data_p, rtus->shdl );
		RBLE_DATA_SET16( Data_p, rtus->ehdl );
		RBLE_DATA_SET16( Data_p, rtus->update_cp_char_hdl );
		RBLE_DATA_SET16( Data_p, rtus->update_cp_val_hdl );
		RBLE_DATA_SET8(  Data_p, rtus->update_cp_prop );
		RBLE_DATA_SET8(  Data_p, rtus->reserved1 );
		RBLE_DATA_SET16( Data_p, rtus->update_state_char_hdl );
		RBLE_DATA_SET16( Data_p, rtus->update_state_val_hdl );
		RBLE_DATA_SET8(  Data_p, rtus->update_state_prop );
		RBLE_DATA_SET8(  Data_p, rtus->reserved2 );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Time Client Disable
 *
 *  Time Client Role disable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Client_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_TIP_CLIENT_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Read Characteristic Info
 *
 *  Read Characteristic Information @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	char_code		Characteristic Code
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Client_Read_Char(uint16_t conhdl, uint8_t char_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_TIP_CLIENT_READ_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Time Update Control Point
 *
 *  @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	time_update_cp	Time update control point value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Client_Write_Time_Update_CP(uint16_t conhdl, uint8_t time_update_cp)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_TIP_CLIENT_WRITE_TIME_UPDATE_CP;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, time_update_cp );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Characteristic info
 *
 *  Write Characteristic Information @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	cfg_val			Configure Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Client_Write_Char(uint16_t conhdl, uint16_t cfg_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_TIP_CLIENT_WRITE_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET16( Data_p, cfg_val );
	}

	return ret;
}

#endif//#if (PRF_SEL_TIPC)
