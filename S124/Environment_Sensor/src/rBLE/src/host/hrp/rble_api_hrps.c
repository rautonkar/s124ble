/**
 ****************************************************************************************
 *
 * @file		rble_api_modem_hrps.c
 *
 * @brief	rBLE Heart Rate Profile Sensor I/F API
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

#if (PRF_SEL_HRPS)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Heart Rate Sensor Role Enable Command
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
RBLE_STATUS RBLE_HRP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_HRP_SENSOR_PARAM *param, RBLE_HRPS_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_HRPS_Info.Hrs_Handler ) {
			RBLE_HRPS_Info.conhdl      = conhdl;
			RBLE_HRPS_Info.Hrs_Handler = call_back;
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
		head_pnt->opcode		= RBLE_CMD_HRP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, sec_lvl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET16( Data_p, param->hrtrate_meas_ntf_en );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Heart Rate Sensor Role Disable Command
 *
 *  Heart Rate Disable @n
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
RBLE_STATUS RBLE_HRP_Sensor_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_HRP_SENSOR_DISABLE;

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
RBLE_STATUS RBLE_HRP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_HRP_MEASUREMENTS_INFO *measurements_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;
	int_t								i;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + ( sizeof( uint16_t ) * RBLE_HRP_RR_INTERVAL_MAX );
	head_pnt->opcode		= RBLE_CMD_HRP_SENSOR_SEND_MEASUREMENTS;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, measurements_info->flags );				/* flags */
	RBLE_DATA_SET8( Data_p, measurements_info->rr_interval_num );	/* rr interval num */
	RBLE_DATA_SET16( Data_p, measurements_info->heart_rate_measure );	/* heart reate value - measurement */
	RBLE_DATA_SET16( Data_p, measurements_info->energy_expended );	/* heart reate value - energy(J) */
	for(i = 0; i < RBLE_HRP_RR_INTERVAL_MAX; i++ ) {
		RBLE_DATA_SET16( Data_p, measurements_info->rr_interval[i] );		/* heart reate value - rr interval(s) */
	}
	return ret;
}

#endif//#if (PRF_SEL_HRPS)
