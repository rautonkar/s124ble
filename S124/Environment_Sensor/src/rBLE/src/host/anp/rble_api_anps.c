/**
 ****************************************************************************************
 *
 * @file		rble_api_anps.c
 *
 * @brief	rBLE-Host ANP Server I/F API
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

#if (PRF_SEL_ANPS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief ANP Server xxx 
 *
 *  ANP Server Role xxx @n
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
 *  @param[in]	sec_lvl			
 *  @param[in]	con_type		
 *  @param[in]	*param			
 *  @param[in]	call_back		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_ANP_Server_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_ANP_SERVER_PARAM *param, RBLE_ANPS_EVENT_HANDLER call_back)
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
		if ( NULL == RBLE_ANPS_Info.Ans_Handler ) {
			RBLE_ANPS_Info.conhdl		= conhdl;
			RBLE_ANPS_Info.Ans_Handler	= call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_ANP_SERVER_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, sec_lvl );
		RBLE_DATA_SET8(  Data_p, con_type );
		
		RBLE_DATA_SET16( Data_p, param->new_alert_ntf_en );
		RBLE_DATA_SET16( Data_p, param->unread_alert_ntf_en );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief ANP Server xxx 
 *
 *  ANP Server Role xxx @n
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
RBLE_STATUS RBLE_ANP_Server_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_ANP_SERVER_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief ANP Server xxx 
 *
 *  ANP Server Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*alert_info		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_ANP_Server_Send_New_Alert(uint16_t conhdl, RBLE_ANP_NEW_ALERT_INFO *alert_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + (sizeof( uint8_t ) * RBLE_ANP_ALERT_TEXT_MAX);
		head_pnt->opcode		= RBLE_CMD_ANP_SERVER_SEND_NEW_ALERT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, alert_info->category_id );
		RBLE_DATA_SET8(  Data_p, alert_info->alert_num );
		RBLE_DATA_SET8(  Data_p, alert_info->text_size );
		memcpy( Data_p, &alert_info->text[0], RBLE_ANP_ALERT_TEXT_MAX);
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief ANP Server xxx 
 *
 *  ANP Server Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*alert_status	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_ANP_Server_Send_Unread_Alert(uint16_t conhdl, RBLE_ANP_UNREAD_ALERT_INFO *alert_status)
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
		head_pnt->opcode		= RBLE_CMD_ANP_SERVER_SEND_UNREAD_ALERT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, alert_status->category_id );
		RBLE_DATA_SET8(  Data_p, alert_status->unread_count );
	}

	return ret;
}

#endif//#if (PRF_SEL_ANPS)
