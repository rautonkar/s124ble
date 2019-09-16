/**
 ****************************************************************************************
 *
 * @file		rble_api_rscps.c
 *
 * @brief	rBLE-Host Running Speed and Cadence Sensor I/F API
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

#if (PRF_SEL_RSCS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Running Speed and Cadence Sensor xxx 
 *
 *  Running Speed and Cadence Sensor Role xxx @n
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
RBLE_STATUS RBLE_RSCP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_RSCP_SENSOR_PARAM *param, RBLE_RSCPS_EVENT_HANDLER call_back)
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
		if ( NULL == RBLE_RSCPS_Info.Rscs_Handler ) {
			RBLE_RSCPS_Info.conhdl		= conhdl;
			RBLE_RSCPS_Info.Rscs_Handler	= call_back;
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
								   + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_RSCP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, sec_lvl );
		RBLE_DATA_SET8(  Data_p, con_type );
		
		RBLE_DATA_SET16( Data_p, param->rsc_meas_ntf_en );
		RBLE_DATA_SET16( Data_p, param->sc_cp_ind_en );
		RBLE_DATA_SET8(  Data_p, param->sensor_location );
		RBLE_DATA_SET8(  Data_p, param->reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Running Speed and Cadence Sensor xxx 
 *
 *  Running Speed and Cadence Sensor Role xxx @n
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
RBLE_STATUS RBLE_RSCP_Sensor_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_RSCP_SENSOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Running Speed and Cadence Sensor xxx 
 *
 *  Running Speed and Cadence Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*measurements_info	
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_RSCP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_RSCP_MEASUREMENTS_INFO *measurements_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint32_t );
		head_pnt->opcode		= RBLE_CMD_RSCP_SENSOR_SEND_MEASUREMENTS;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, measurements_info->flags );
		RBLE_DATA_SET8(  Data_p, measurements_info->instant_cadence );
		RBLE_DATA_SET16( Data_p, measurements_info->instant_speed );
		RBLE_DATA_SET16( Data_p, measurements_info->instant_stride_len );
		RBLE_DATA_SET32( Data_p, measurements_info->total_distance );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Running Speed and Cadence Sensor xxx 
 *
 *  Running Speed and Cadence Sensor Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*sc_cp_info		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_RSCP_Sensor_Send_SC_Control_Point(uint16_t conhdl, RBLE_RSCP_SC_CONTROL_POINT_INFO *sc_cp_info)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint32_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_RSCP_SENSOR_SEND_SC_CONTROL_POINT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, sc_cp_info->OpCode );
		RBLE_DATA_SET8(  Data_p, sc_cp_info->reserved1 );
		RBLE_DATA_SET32( Data_p, sc_cp_info->cumulative_value );
		RBLE_DATA_SET8(  Data_p, sc_cp_info->sensor_location );
		RBLE_DATA_SET8(  Data_p, sc_cp_info->request_op_code );
		RBLE_DATA_SET8(  Data_p, sc_cp_info->response_value );
		RBLE_DATA_SET8(  Data_p, sc_cp_info->reserved );
	}

	return ret;
}

#endif//#if (PRF_SEL_RSCS)
