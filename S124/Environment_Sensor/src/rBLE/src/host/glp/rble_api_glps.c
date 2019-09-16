/**
 ****************************************************************************************
 *
 * @file		rble_api_glps.c
 *
 * @brief	rBLE-Host  Glucose Profile Sensor I/F API
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

#if (PRF_SEL_GLPS)

/**************************** Define Declaration **************************************/


/**************************** Structure Declaration ***********************************/

/**************************** PUBLIC Memory Declaration *******************************/


/**************************** PRIVATE Memory Declaration ******************************/

/**************************** PRIVATE PROTOTYPE Declaration ***************************/

/******************************* Program Erea *********************************************/

/******************************************************************************************/
/**
 *  @brief Glucose Sensor Enable
 *
 *  Glucose Profile Sensor Role Enable@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Store Call Back
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *  @param[in] sec_lvl		Security level
 *  @param[in] con_type		Connection Type
 *  @param[in] *param		Sensor Parameter
 *  @param[in] *call_back	Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_GLP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_GLP_SENSOR_PARAM *param, RBLE_GLPS_EVENT_HANDLER call_back)
{
	RBLE_STATUS 					ret = RBLE_OK;
	RBLE_HEADER_t 					*head_pnt;
	uint8_t							*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	if(RBLE_OK == ret){
		/* Parameter Check */
		if(NULL == call_back){
			ret = RBLE_PARAM_ERR;
		}else{
			/* Search Free Call Back Handler Memory */
			if ( NULL == RBLE_GLPS_Info.Gls_Handler ) {
				RBLE_GLPS_Info.conhdl      = conhdl;
				RBLE_GLPS_Info.Gls_Handler = call_back;
			} else {
				/* Free Memory None */
				ret = RBLE_ERR;
			}
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_GLP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, sec_lvl );
		RBLE_DATA_SET8( Data_p, con_type );

		RBLE_DATA_SET16( Data_p, param->glp_meas_ntf_en );
		RBLE_DATA_SET16( Data_p, param->glp_meas_context_ntf_en );
		RBLE_DATA_SET16( Data_p, param->ra_cp_ind_en );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Glucose Sensor Disable
 *
 *  Glucose Profile Sensor Role disable@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_GLP_Sensor_Disable(uint16_t conhdl)
{
	RBLE_STATUS 					ret = RBLE_OK;
	RBLE_HEADER_t 					*head_pnt;
	uint8_t							*Data_p;

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
		head_pnt->opcode		= RBLE_CMD_GLP_SENSOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Glucose Send Measurements Info
 *
 *  Glucose Send Measurements Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *  @param[in] measurements_info	Measurements Infomation
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_GLP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_GLP_MEASUREMENTS_INFO *measurements_info)
{
	RBLE_STATUS								ret = RBLE_OK;
	RBLE_HEADER_t							*head_pnt;
	uint8_t									*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								 + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								 + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								 + sizeof( uint16_t ) );
		head_pnt->opcode		= RBLE_CMD_GLP_SENSOR_SEND_MEASUREMENTS;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, measurements_info->flags );
		RBLE_DATA_SET8( Data_p, 0 );				/* Reserved */
		RBLE_DATA_SET16( Data_p, measurements_info->seq_num );
		RBLE_DATA_SET16( Data_p, measurements_info->stamp.year );
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.month );
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.day );
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.hour );
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.min );
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.sec );
		RBLE_DATA_SET8( Data_p, 0 );				/* Reserved */
		RBLE_DATA_SET16( Data_p, measurements_info->time_offset );
		RBLE_DATA_SET16( Data_p, measurements_info->concentration );
		RBLE_DATA_SET8( Data_p, measurements_info->type );
		RBLE_DATA_SET8( Data_p, measurements_info->sample_location );
		RBLE_DATA_SET16( Data_p, measurements_info->sensor_status_annun );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Measurements Context Info
 *
 *  Send Measurements Context Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl				Connection Handle
 *  @param[in]	*measurements_context_info	Measurements Context Infomation
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_GLP_Sensor_Send_Measurements_Context(uint16_t conhdl, RBLE_GLP_MEASUREMENTS_CONTEXT_INFO *measurements_context_info)
{
	RBLE_STATUS 					ret = RBLE_OK;
	RBLE_HEADER_t 					*head_pnt;
	uint8_t							*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								 + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								 + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t )
								 + sizeof( uint16_t ) );
		head_pnt->opcode		= RBLE_CMD_GLP_SENSOR_SEND_MEASUREMENTS_CONTEXT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, measurements_context_info->flags );
		RBLE_DATA_SET8( Data_p, measurements_context_info->reserved1 );
		RBLE_DATA_SET16( Data_p, measurements_context_info->seq_num );
		RBLE_DATA_SET8( Data_p, measurements_context_info->ex_flags );
		RBLE_DATA_SET8( Data_p, measurements_context_info->carbohydrate_id );
		RBLE_DATA_SET16( Data_p, measurements_context_info->carbohydrate_kg );
		RBLE_DATA_SET8( Data_p, measurements_context_info->meal );
		RBLE_DATA_SET8( Data_p, measurements_context_info->tester );
		RBLE_DATA_SET8( Data_p, measurements_context_info->health );
		RBLE_DATA_SET8( Data_p, measurements_context_info->reserved2 );
		RBLE_DATA_SET16( Data_p, measurements_context_info->exercise_duration );
		RBLE_DATA_SET8( Data_p, measurements_context_info->exercise_intensity );
		RBLE_DATA_SET8( Data_p, measurements_context_info->medication_id );
		RBLE_DATA_SET16( Data_p, measurements_context_info->medication );
		RBLE_DATA_SET16( Data_p, measurements_context_info->HbA1c );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send RA Control Point Info
 *
 *  Send RA Control Point Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl				Connection Handle
 *  @param[in]	*ra_cp_info	Record Access control point Infomation
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_GLP_Sensor_Send_RA_Control_Point(uint16_t conhdl, RBLE_GLP_RA_CONTROL_POINT_IND_INFO *ra_cp_res_info)
{
	RBLE_STATUS 					ret = RBLE_OK;
	RBLE_HEADER_t 					*head_pnt;
	uint8_t							*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_GLP_SENSOR_SEND_RA_CONTROL_POINT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, ra_cp_res_info->OpCode );
		RBLE_DATA_SET8( Data_p, ra_cp_res_info->racp_operator );
		RBLE_DATA_SET16( Data_p, ra_cp_res_info->num_of_records );
		RBLE_DATA_SET8( Data_p, ra_cp_res_info->request_op_code );
		RBLE_DATA_SET8( Data_p, ra_cp_res_info->response_code_value );
	}
	
	return ret;
}

#endif//#if (PRF_SEL_GLPS)
