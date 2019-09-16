/**
 ****************************************************************************************
 *
 * @file		rble_api_anpc.c
 *
 * @brief	rBLE-Host ANP Client I/F API
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

#if (PRF_SEL_ANPC)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief ANP Client xxx 
 *
 *  ANP Client Role xxx @n
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
 *  @param[in]	*ans			
 *  @param[in]	call_back		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_ANP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_ANS_CONTENT *ans, RBLE_ANPC_EVENT_HANDLER call_back)
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
		if ( NULL == RBLE_ANPC_Info.Anc_Handler ) {
			RBLE_ANPC_Info.conhdl		= conhdl;
			RBLE_ANPC_Info.Anc_Handler	= call_back;
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
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_ANP_CLIENT_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, con_type );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		
		RBLE_DATA_SET16( Data_p, ans->shdl );
		RBLE_DATA_SET16( Data_p, ans->ehdl );
		RBLE_DATA_SET16( Data_p, ans->supp_new_alert_char_hdl );
		RBLE_DATA_SET16( Data_p, ans->supp_new_alert_val_hdl );
		RBLE_DATA_SET8(  Data_p, ans->supp_new_alert_prop );
		RBLE_DATA_SET8(  Data_p, ans->reserved1 );
		RBLE_DATA_SET16( Data_p, ans->new_alert_char_hdl );
		RBLE_DATA_SET16( Data_p, ans->new_alert_val_hdl );
		RBLE_DATA_SET16( Data_p, ans->new_alert_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, ans->new_alert_prop );
		RBLE_DATA_SET8(  Data_p, ans->reserved2 );
		RBLE_DATA_SET16( Data_p, ans->supp_unread_alert_char_hdl );
		RBLE_DATA_SET16( Data_p, ans->supp_unread_alert_val_hdl );
		RBLE_DATA_SET8(  Data_p, ans->supp_unread_alert_prop );
		RBLE_DATA_SET8(  Data_p, ans->reserved3 );
		RBLE_DATA_SET16( Data_p, ans->unread_alert_char_hdl );
		RBLE_DATA_SET16( Data_p, ans->unread_alert_val_hdl );
		RBLE_DATA_SET16( Data_p, ans->unread_alert_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, ans->unread_alert_prop );
		RBLE_DATA_SET8(  Data_p, ans->reserved4 );
		RBLE_DATA_SET16( Data_p, ans->alert_ntf_cp_char_hdl );
		RBLE_DATA_SET16( Data_p, ans->alert_ntf_cp_val_hdl );
		RBLE_DATA_SET8(  Data_p, ans->alert_ntf_cp_prop );
		RBLE_DATA_SET8(  Data_p, ans->reserved5 );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief ANP Client xxx 
 *
 *  ANP Client Role xxx @n
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
RBLE_STATUS RBLE_ANP_Client_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_ANP_CLIENT_DISABLE;

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
RBLE_STATUS RBLE_ANP_Client_Read_Char(uint16_t conhdl, uint8_t char_code)
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
		head_pnt->opcode		= RBLE_CMD_ANP_CLIENT_READ_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief ANP Client xxx 
 *
 *  ANP Client Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*alert_ntf_cp	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_ANP_Client_Write_Alert_Notification_CP(uint16_t conhdl, RBLE_ANP_ALERT_NTF_CP *alert_ntf_cp)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_ANP_CLIENT_WRITE_ALERT_NOTIFICATION_CP;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, alert_ntf_cp->command_id );
		RBLE_DATA_SET8(  Data_p, alert_ntf_cp->category_id );
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
RBLE_STATUS RBLE_ANP_Client_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val)
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
		head_pnt->opcode		= RBLE_CMD_ANP_CLIENT_WRITE_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		RBLE_DATA_SET16( Data_p, cfg_val );
	}

	return ret;
}

#endif//#if (PRF_SEL_ANPC)
