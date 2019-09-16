/**
 ****************************************************************************************
 *
 * @file		rble_api_tips.c
 *
 * @brief	rBLE-Host Time Server I/F API
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

#if (PRF_SEL_TIPS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Time Server Enable
 *
 *  Time Server Role enable @n
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
 *  @param[in]	sec_lvl				Security level
 *  @param[in]	con_type			Connection Type
 *  @param[in]	current_time_ntf_en	Notification Config Value
 *  @param[in]	call_back			Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Server_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, uint16_t current_time_ntf_en, RBLE_TIPS_EVENT_HANDLER call_back)
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
		if ( NULL == RBLE_TIPS_Info.Tis_Handler ) {
			RBLE_TIPS_Info.conhdl		= conhdl;
			RBLE_TIPS_Info.Tis_Handler	= call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_TIP_SERVER_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, sec_lvl );
		RBLE_DATA_SET8(  Data_p, con_type );
		RBLE_DATA_SET16( Data_p, current_time_ntf_en );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Time Server Disable
 *
 *  Time Server Role disable @n
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
RBLE_STATUS RBLE_TIP_Server_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_TIP_SERVER_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Time Server Send Current Time
 *
 *  Time Server Role Send Current Time@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*current_time	Current time information
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Server_Send_Current_Time(uint16_t conhdl, RBLE_TIP_CURRENT_TIME *current_time)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_TIP_SERVER_SEND_CURRENT_TIME;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		
		RBLE_DATA_SET16( Data_p, current_time->stamp.year );
		RBLE_DATA_SET8(  Data_p, current_time->stamp.month );
		RBLE_DATA_SET8(  Data_p, current_time->stamp.day );
		RBLE_DATA_SET8(  Data_p, current_time->stamp.hour );
		RBLE_DATA_SET8(  Data_p, current_time->stamp.min );
		RBLE_DATA_SET8(  Data_p, current_time->stamp.sec );
		RBLE_DATA_SET8(  Data_p, current_time->stamp.reserved );
		RBLE_DATA_SET8(  Data_p, current_time->day_of_week );
		RBLE_DATA_SET8(  Data_p, current_time->fractions256 );
		RBLE_DATA_SET8(  Data_p, current_time->adjust_reason );
		RBLE_DATA_SET8(  Data_p, current_time->reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Time Server Write Data
 *
 *  Time Server Role Write Characteristic Data@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	char_code		Characteristic Code
 *  @param[in]	*wr_data		Writing data
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_TIP_Server_Write_Data(uint8_t char_code, RBLE_TIPS_WR_DATA *wr_data)
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
		head_pnt->packet_length = sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_TIP_SERVER_WRITE_DATA;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET8(  Data_p, char_code );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		
		switch(char_code) {
			case RBLE_TIPS_WR_CTS_CRNT_TIME:
				RBLE_DATA_SET16( Data_p, wr_data->param.current_time.stamp.year );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.stamp.month );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.stamp.day );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.stamp.hour );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.stamp.min );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.stamp.sec );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.stamp.reserved );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.day_of_week );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.fractions256 );
				RBLE_DATA_SET8(  Data_p, wr_data->param.current_time.adjust_reason );
				
				head_pnt->packet_length += sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
										 + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
				break;
			case RBLE_TIPS_WR_CTS_LOCAL_TIME:
				RBLE_DATA_SET8( Data_p, wr_data->param.local_time.time_zone );
				RBLE_DATA_SET8( Data_p, wr_data->param.local_time.dst_offset );
				head_pnt->packet_length += sizeof( uint8_t ) + sizeof( uint8_t );
				break;
			case RBLE_TIPS_WR_CTS_REF_TIME:
				RBLE_DATA_SET8( Data_p, wr_data->param.ref_time.time_source );
				RBLE_DATA_SET8( Data_p, wr_data->param.ref_time.accuracy );
				RBLE_DATA_SET8( Data_p, wr_data->param.ref_time.days_since_update );
				RBLE_DATA_SET8( Data_p, wr_data->param.ref_time.hours_since_update );
				head_pnt->packet_length += sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
				break;
			case RBLE_TIPS_WR_NDCS_TIME_DST:
				RBLE_DATA_SET16( Data_p, wr_data->param.next_dst.stamp.year );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.stamp.month );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.stamp.day );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.stamp.hour );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.stamp.min );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.stamp.sec );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.stamp.reserved );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.dst_offset );
				RBLE_DATA_SET8(  Data_p, wr_data->param.next_dst.reserved );
				head_pnt->packet_length += sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
										 + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
				break;
			case RBLE_TIPS_WR_RTUS_UPDATE_STATUS:
			default:
				RBLE_DATA_SET8( Data_p, wr_data->param.update_state.current_state );
				RBLE_DATA_SET8( Data_p, wr_data->param.update_state.update_result );
				head_pnt->packet_length += sizeof( uint8_t ) + sizeof( uint8_t );
				break;
		}
	}

	return ret;
}

#endif//#if (PRF_SEL_TIPS)
