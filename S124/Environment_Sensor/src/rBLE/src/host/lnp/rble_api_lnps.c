/**
 ****************************************************************************************
 *
 * @file		rble_api_lnps.c
 *
 * @brief	rBLE-Host LNP Sensor I/F API
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

#if (PRF_SEL_LNPS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
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
RBLE_STATUS RBLE_LNP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_LNP_SENSOR_PARAM *param, RBLE_LNPS_EVENT_HANDLER call_back)
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
		if ( NULL == RBLE_LNPS_Info.Lns_Handler ) {
			RBLE_LNPS_Info.conhdl		= conhdl;
			RBLE_LNPS_Info.Lns_Handler	= call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, sec_lvl );
		RBLE_DATA_SET8(  Data_p, con_type );
		
		RBLE_DATA_SET16( Data_p, param->location_speed_ntf_en );
		RBLE_DATA_SET16( Data_p, param->ln_cp_ind_en );
		RBLE_DATA_SET16( Data_p, param->navigation_ntf_en );
		RBLE_DATA_SET16( Data_p, param->battery_level_ntf_en );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
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
RBLE_STATUS RBLE_LNP_Sensor_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*location_speed_info	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Sensor_Send_Location_Speed(uint16_t conhdl, RBLE_LNP_LOCATION_SPEED_INFO *location_speed_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint32_t ) + sizeof( int32_t )
								   + sizeof( int32_t ) + sizeof( int32_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_SEND_LOCATION_SPEED;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET16( Data_p, location_speed_info->flags );
		RBLE_DATA_SET16( Data_p, location_speed_info->instant_speed );
		RBLE_DATA_SET32( Data_p, location_speed_info->total_distance );
		RBLE_DATA_SET32( Data_p, location_speed_info->latitude );
		RBLE_DATA_SET32( Data_p, location_speed_info->longitude );
		RBLE_DATA_SET32( Data_p, location_speed_info->elevation );
		RBLE_DATA_SET16( Data_p, location_speed_info->heading );
		RBLE_DATA_SET8(  Data_p, location_speed_info->rolling_time );
		RBLE_DATA_SET8(  Data_p, location_speed_info->reserved );
		
		RBLE_DATA_SET16( Data_p, location_speed_info->utc_time.year );
		RBLE_DATA_SET8(  Data_p, location_speed_info->utc_time.month );
		RBLE_DATA_SET8(  Data_p, location_speed_info->utc_time.day );
		RBLE_DATA_SET8(  Data_p, location_speed_info->utc_time.hour );
		RBLE_DATA_SET8(  Data_p, location_speed_info->utc_time.min );
		RBLE_DATA_SET8(  Data_p, location_speed_info->utc_time.sec );
		RBLE_DATA_SET8(  Data_p, location_speed_info->utc_time.reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	*quality_info	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Sensor_Set_Position_Quality(RBLE_LNP_POSITION_QUALITY_INFO *quality_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint32_t )
								  + sizeof( uint32_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_SET_POSITION_QUALITY;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		
		RBLE_DATA_SET16( Data_p, quality_info->flags );
		RBLE_DATA_SET8(  Data_p, quality_info->beacon_solution_num );
		RBLE_DATA_SET8(  Data_p, quality_info->beacon_view_num );
		RBLE_DATA_SET16( Data_p, quality_info->first_fix_time );
		RBLE_DATA_SET32( Data_p, quality_info->ehpe );
		RBLE_DATA_SET32( Data_p, quality_info->evpe );
		RBLE_DATA_SET8(  Data_p, quality_info->hdop );
		RBLE_DATA_SET8(  Data_p, quality_info->vdop );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*ind_cp_info	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Sensor_Send_LN_Control_Point(uint16_t conhdl, RBLE_LNP_IND_CONTROL_POINT_INFO *ind_cp_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint8_t ) + (sizeof( uint8_t ) * RBLE_LNP_SENSOR_ROUTE_NAME_MAX);
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_SEND_LN_CONTROL_POINT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, ind_cp_info->OpCode );
		RBLE_DATA_SET8(  Data_p, ind_cp_info->request_op_code );
		RBLE_DATA_SET8(  Data_p, ind_cp_info->response_value );
		RBLE_DATA_SET8(  Data_p, ind_cp_info->reserved );
		RBLE_DATA_SET16( Data_p, ind_cp_info->route_num );
		RBLE_DATA_SET8(  Data_p, ind_cp_info->name_size );
		memcpy(Data_p, &ind_cp_info->route_name[0], RBLE_LNP_SENSOR_ROUTE_NAME_MAX);
		Data_p += RBLE_LNP_SENSOR_ROUTE_NAME_MAX;
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*navigation_info	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Sensor_Send_Navigation(uint16_t conhdl, RBLE_LNP_NAVIGATION_INFO *navigation_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint32_t )
								   + sizeof( int32_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_SEND_NAVIGATION;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET16( Data_p, navigation_info->flags );
		RBLE_DATA_SET16( Data_p, navigation_info->bearing );
		RBLE_DATA_SET16( Data_p, navigation_info->heading );
		RBLE_DATA_SET32( Data_p, navigation_info->remain_dis );
		RBLE_DATA_SET32( Data_p, navigation_info->remain_v_dis );
		
		RBLE_DATA_SET16( Data_p, navigation_info->estimate_time.year );
		RBLE_DATA_SET8(  Data_p, navigation_info->estimate_time.month );
		RBLE_DATA_SET8(  Data_p, navigation_info->estimate_time.day );
		RBLE_DATA_SET8(  Data_p, navigation_info->estimate_time.hour );
		RBLE_DATA_SET8(  Data_p, navigation_info->estimate_time.min );
		RBLE_DATA_SET8(  Data_p, navigation_info->estimate_time.sec );
		RBLE_DATA_SET8(  Data_p, navigation_info->estimate_time.reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Sensor xxx 
 *
 *  LNP Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	battery_level	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Sensor_Send_Battery_Level(uint16_t conhdl, uint8_t battery_level)
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
		head_pnt->opcode		= RBLE_CMD_LNP_SENSOR_SEND_BATTERY_LEVEL;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, battery_level );
	}

	return ret;
}

#endif//#if (PRF_SEL_LNPS)
