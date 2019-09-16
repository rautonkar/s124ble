/**
 ****************************************************************************************
 *
 * @file		rble_api_cscps.c
 *
 * @brief	rBLE Cycling Speed and Cadence Profile Sensor I/F API
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

#if (PRF_SEL_CSCS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Cycling Speed and Cadence Sensor Role Enable Command
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
RBLE_STATUS RBLE_CSCP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_CSCP_SENSOR_PARAM *param, RBLE_CSCPS_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_CSCPS_Info.Cscs_Handler ) {
			RBLE_CSCPS_Info.conhdl       = conhdl;
			RBLE_CSCPS_Info.Cscs_Handler = call_back;
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
                                  + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_CSCP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, sec_lvl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET16( Data_p, param->cycspd_meas_ntf_en );
		RBLE_DATA_SET16( Data_p, param->sc_cp_ind_en );
		RBLE_DATA_SET8( Data_p, param->sensor_location );
		RBLE_DATA_SET8( Data_p, param->reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Cycling Speed and Cadence Sensor Role Disable Command
 *
 *  Cycling Speed and Cadence Sensor Disable @n
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
RBLE_STATUS RBLE_CSCP_Sensor_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_SENSOR_DISABLE;

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
RBLE_STATUS RBLE_CSCP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_CSCP_MEASUREMENTS_INFO *measurements_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint32_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_SENSOR_SEND_MEASUREMENTS;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, measurements_info->flags );
	RBLE_DATA_SET8( Data_p, measurements_info->reserved );
	RBLE_DATA_SET32( Data_p, measurements_info->wheel_revolutions );
	RBLE_DATA_SET16( Data_p, measurements_info->wheel_event_time );
	RBLE_DATA_SET16( Data_p, measurements_info->crank_revolutions );
	RBLE_DATA_SET16( Data_p, measurements_info->crank_event_time );

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Send SC Control Point Info Command
 *
 *  Send SC Control Point Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	*sc_cp_info	SC Control Point Infomation
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CSCP_Sensor_Send_SC_Control_Point(uint16_t conhdl, RBLE_CSCP_SC_CONTROL_POINT_INFO *sc_cp_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint32_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_SENSOR_SEND_SC_CONTROL_POINT;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, sc_cp_info->OpCode );
	RBLE_DATA_SET8( Data_p, sc_cp_info->reserved1 );
	RBLE_DATA_SET32( Data_p, sc_cp_info->cumulative_value );
	RBLE_DATA_SET8( Data_p, sc_cp_info->sensor_location );
	RBLE_DATA_SET8( Data_p, sc_cp_info->request_op_code );
	RBLE_DATA_SET8( Data_p, sc_cp_info->response_value );
	RBLE_DATA_SET8( Data_p, sc_cp_info->reserved2 );

	return ret;
}

#endif//#if (PRF_SEL_CSCS)
