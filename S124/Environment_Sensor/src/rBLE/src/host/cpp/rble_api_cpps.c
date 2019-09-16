/**
 ****************************************************************************************
 *
 * @file		rble_api_cpps.c
 *
 * @brief	rBLE Cycling Power Profile Sensor I/F API
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

#if (PRF_SEL_CPPS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Cycling Power Sensor Role Enable Command
 *
 *  Sensor Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]  sec_lvl		Security level
 *  @param[in]	con_type	Connection Type
 *  @param[in]	param		Sensor Parameter
 *  @param[in]	call_back	call_back Address
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Enable( uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_CPP_SENSOR_PARAM *param, RBLE_CPPS_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_CPPS_Info.Cps_Handler ) {
			RBLE_CPPS_Info.conhdl       = conhdl;
			RBLE_CPPS_Info.Cps_Handler = call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
                                  + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
                                  + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, sec_lvl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET16( Data_p, param->cp_meas_ntf_en );
		RBLE_DATA_SET16( Data_p, param->cp_meas_brd_en );
		RBLE_DATA_SET16( Data_p, param->cp_vector_ntf_en );
		RBLE_DATA_SET16( Data_p, param->cp_cp_ind_en );
		RBLE_DATA_SET16( Data_p, param->battery_level_ntf_en );
		RBLE_DATA_SET8( Data_p, param->sensor_location );
		RBLE_DATA_SET8( Data_p, param->reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Cycling Power Sensor Role Disable Command
 *
 *  Cycling Power Sensor Disable @n
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
RBLE_STATUS RBLE_CPP_Sensor_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_DISABLE;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Measurements Info Command
 *
 *  Send Measurements Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	*measurements_info	Measurements Infomation
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_CPP_MEASUREMENTS_INFO *measurements_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )							/* flags/instant_power/pedal_balance */
                              + sizeof( uint16_t ) + sizeof( uint32_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )	/* wheel/crank */
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )						/* magnitude */
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t );	/* angle/dead_spot/energy */
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_SEND_MEASUREMENTS;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	
	RBLE_DATA_SET16( Data_p, measurements_info->flags );
	RBLE_DATA_SET16( Data_p, measurements_info->instant_power );
	RBLE_DATA_SET8(  Data_p, measurements_info->pedal_balance );
	RBLE_DATA_SET8(  Data_p, measurements_info->reserved );
	RBLE_DATA_SET16( Data_p, measurements_info->accumulated_torque );
	RBLE_DATA_SET32( Data_p, measurements_info->wheel_revolutions );
	RBLE_DATA_SET16( Data_p, measurements_info->wheel_event );
	RBLE_DATA_SET16( Data_p, measurements_info->crank_revolutions );
	RBLE_DATA_SET16( Data_p, measurements_info->crank_event );
	RBLE_DATA_SET16( Data_p, measurements_info->max_force_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->min_force_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->max_torque_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->min_torque_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->max_angle );
	RBLE_DATA_SET16( Data_p, measurements_info->min_angle );
	RBLE_DATA_SET16( Data_p, measurements_info->top_dead_spot );
	RBLE_DATA_SET16( Data_p, measurements_info->bottom_dead_spot );
	RBLE_DATA_SET16( Data_p, measurements_info->accumulated_energy );

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Broadcast Measurements Info Command
 *
 *  Broadcast Measurements Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	*measurements_info	Measurements Infomation
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Broadcast_Measurements(uint16_t conhdl, RBLE_CPP_MEASUREMENTS_INFO *measurements_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;


	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )							/* flags/instant_power/pedal_balance */
                              + sizeof( uint16_t ) + sizeof( uint32_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )	/* wheel/crank */
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )						/* magnitude */
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t );	/* angle/dead_spot/energy */
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_BROADCAST_MEASUREMENTS;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	
	RBLE_DATA_SET16( Data_p, measurements_info->flags );
	RBLE_DATA_SET16( Data_p, measurements_info->instant_power );
	RBLE_DATA_SET8(  Data_p, measurements_info->pedal_balance );
	RBLE_DATA_SET8(  Data_p, measurements_info->reserved );
	RBLE_DATA_SET16( Data_p, measurements_info->accumulated_torque );
	RBLE_DATA_SET32( Data_p, measurements_info->wheel_revolutions );
	RBLE_DATA_SET16( Data_p, measurements_info->wheel_event );
	RBLE_DATA_SET16( Data_p, measurements_info->crank_revolutions );
	RBLE_DATA_SET16( Data_p, measurements_info->crank_event );
	RBLE_DATA_SET16( Data_p, measurements_info->max_force_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->min_force_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->max_torque_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->min_torque_magnitude );
	RBLE_DATA_SET16( Data_p, measurements_info->max_angle );
	RBLE_DATA_SET16( Data_p, measurements_info->min_angle );
	RBLE_DATA_SET16( Data_p, measurements_info->top_dead_spot );
	RBLE_DATA_SET16( Data_p, measurements_info->bottom_dead_spot );
	RBLE_DATA_SET16( Data_p, measurements_info->accumulated_energy );

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Vector Info Command
 *
 *  Send Vector Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*vector_info	Vector Infomation
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Send_Vector(uint16_t conhdl, RBLE_CPP_VECTOR_INFO *vector_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;
	uint8_t								i;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + (sizeof( uint16_t ) * RBLE_CPP_MAGNITUDE_MAX);
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_SEND_VECTOR;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, vector_info->flags );
	RBLE_DATA_SET8( Data_p, vector_info->reserved1 );
	RBLE_DATA_SET16( Data_p, vector_info->crank_revolutions );
	RBLE_DATA_SET16( Data_p, vector_info->crank_event );
	RBLE_DATA_SET16( Data_p, vector_info->first_crank_angle );
	RBLE_DATA_SET8( Data_p, vector_info->array_num );
	RBLE_DATA_SET8( Data_p, vector_info->reserved2 );
	for(i = 0; i < RBLE_CPP_MAGNITUDE_MAX; i++)
	{
		RBLE_DATA_SET16( Data_p, vector_info->magnitude[i] );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send CP Control Point Info Command
 *
 *  Send CP Control Point Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*ind_cp_info	CP Control Point Infomation
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Send_CP_Control_Point(uint16_t conhdl, RBLE_CPP_IND_CONTROL_POINT_INFO *ind_cp_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )		/* OpCode ~ reserved1 */
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )	/* crank_len ~ span_len */
                              + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )							/* offset_compensation ~ reserved2 */
                              + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );	/* stamp */
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_SEND_CONTROL_POINT;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, ind_cp_info->OpCode );
	RBLE_DATA_SET8( Data_p, ind_cp_info->request_op_code );
	RBLE_DATA_SET8( Data_p, ind_cp_info->response_value );
	RBLE_DATA_SET8( Data_p, ind_cp_info->reserved1 );
	RBLE_DATA_SET16( Data_p, ind_cp_info->crank_length );
	RBLE_DATA_SET16( Data_p, ind_cp_info->chain_length );
	RBLE_DATA_SET16( Data_p, ind_cp_info->chain_weight );
	RBLE_DATA_SET16( Data_p, ind_cp_info->span_length );
	RBLE_DATA_SET16( Data_p, ind_cp_info->offset_compensation );
	RBLE_DATA_SET8( Data_p, ind_cp_info->sampling_rate );
	RBLE_DATA_SET8( Data_p, ind_cp_info->reserved2 );
	
	RBLE_DATA_SET16( Data_p, ind_cp_info->stamp.year );
	RBLE_DATA_SET8( Data_p, ind_cp_info->stamp.month );
	RBLE_DATA_SET8( Data_p, ind_cp_info->stamp.day );
	RBLE_DATA_SET8( Data_p, ind_cp_info->stamp.hour );
	RBLE_DATA_SET8( Data_p, ind_cp_info->stamp.min );
	RBLE_DATA_SET8( Data_p, ind_cp_info->stamp.sec );
	RBLE_DATA_SET8( Data_p, ind_cp_info->stamp.reserved );

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
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	battery_level	Battery Level
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Send_Battery_Level(uint16_t conhdl, uint8_t battery_level)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_SEND_BATTERY_LEVEL;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, battery_level );

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send Write Response Command
 *
 *  Send Battery Level @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	res_code		Response Code
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Sensor_Send_Write_Response(uint16_t conhdl, uint8_t res_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
	head_pnt->opcode		= RBLE_CMD_CPP_SENSOR_SEND_WRITE_RESPONSE;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, res_code );

	return ret;
}
#endif//#if (PRF_SEL_CPPS)
