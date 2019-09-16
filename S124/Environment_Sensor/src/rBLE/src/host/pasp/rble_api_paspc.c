/**
 ****************************************************************************************
 *
 * @file		rble_api_paspc.c
 *
 * @brief	rBLE-Host Phone Alert Status Client I/F API
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: v3.00.00 $
 *
 ****************************************************************************************
 */

#include	<string.h>

#include	"rble_api.h"
#include	"rble_host.h"
#include	"rble_trans.h"

#if (PRF_SEL_PASC)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Phone Alert Status Client xxx 
 *
 *  Phone Alert Status Client Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Store Call Back
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	con_type		
 *  @param[in]	*pass			
 *  @param[in]	call_back		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PASP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_PASS_CONTENT *pass, RBLE_PASPC_EVENT_HANDLER call_back)
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
		if ( NULL == RBLE_PASPC_Info.Pasc_Handler ) {
			RBLE_PASPC_Info.conhdl		= conhdl;
			RBLE_PASPC_Info.Pasc_Handler	= call_back;
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
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_PASP_CLIENT_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, con_type );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		
		RBLE_DATA_SET16( Data_p, pass->shdl );
		RBLE_DATA_SET16( Data_p, pass->ehdl );
		RBLE_DATA_SET16( Data_p, pass->alert_status_char_hdl );
		RBLE_DATA_SET16( Data_p, pass->alert_status_val_hdl );
		RBLE_DATA_SET16( Data_p, pass->alert_status_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, pass->alert_status_prop );
		RBLE_DATA_SET8(  Data_p, pass->reserved );
		RBLE_DATA_SET16( Data_p, pass->ringer_setting_char_hdl );
		RBLE_DATA_SET16( Data_p, pass->ringer_setting_val_hdl );
		RBLE_DATA_SET16( Data_p, pass->ringer_setting_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, pass->ringer_setting_prop );
		RBLE_DATA_SET8(  Data_p, pass->reserved2 );
		RBLE_DATA_SET16( Data_p, pass->ringer_cp_char_hdl );
		RBLE_DATA_SET16( Data_p, pass->ringer_cp_val_hdl );
		RBLE_DATA_SET8(  Data_p, pass->ringer_cp_prop );
		RBLE_DATA_SET8(  Data_p, pass->reserved3 );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Phone Alert Status Client xxx 
 *
 *  Phone Alert Status Client Role xxx @n
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
RBLE_STATUS RBLE_PASP_Client_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_PASP_CLIENT_DISABLE;

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
 *  @param[in]	char_code		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PASP_Client_Read_Char(uint16_t conhdl, uint8_t char_code)
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
		head_pnt->opcode		= RBLE_CMD_PASP_CLIENT_READ_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Phone Alert Status Client xxx 
 *
 *  Phone Alert Status Client Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	cp_val		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PASP_Client_Write_Ringer_Control_Point(uint16_t conhdl, uint8_t cp_val)
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
		head_pnt->opcode		= RBLE_CMD_PASP_CLIENT_WRITE_RINGER_CONTROL_POINT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, cp_val );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Characteristic Info
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
 *  @param[in]	char_code		
 *  @param[in]	cfg_val			
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PASP_Client_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )		/* reserved */
								   + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_PASP_CLIENT_WRITE_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		RBLE_DATA_SET16( Data_p, cfg_val );
	}

	return ret;
}

#endif//#if (PRF_SEL_PASC)
