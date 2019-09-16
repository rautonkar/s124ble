/**
 ****************************************************************************************
 *
 * @file		rble_api_blps.c
 *
 * @brief	rBLE-Host  Blood Pressure Profile Sensor I/F API
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

#if (PRF_SEL_BLPS)

/**************************** Define Declaration **************************************/


/**************************** Structure Declaration ***********************************/

/**************************** PUBLIC Memory Declaration *******************************/


/**************************** PRIVATE Memory Declaration ******************************/

/**************************** PRIVATE PROTOTYPE Declaration ***************************/

/******************************* Program Erea *********************************************/

/******************************************************************************************/
/**
 *  @brief Blood Pressure Sensor Enable
 *
 *  Blood Pressure Profile Sensor Role Enable@n
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
RBLE_STATUS RBLE_BLP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_BLP_SENSOR_PARAM *param, RBLE_BLPS_EVENT_HANDLER call_back)
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
			if ( NULL == RBLE_BLPS_Info.Bls_Handler ) {
				RBLE_BLPS_Info.conhdl      = conhdl;
				RBLE_BLPS_Info.Bls_Handler = call_back;
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_BLP_SENSOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, sec_lvl );
		RBLE_DATA_SET8( Data_p, con_type );

		RBLE_DATA_SET16( Data_p, param->bldprs_meas_ind_en );     /* Blood pressure measurement indication configuration */
		RBLE_DATA_SET16( Data_p, param->interm_cufprs_ntf_en );   /* Intermediate cuff pressure notification configuration */
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Blood Pressure Sensor Disable
 *
 *  Blood Pressure Profile Sensor Role disable@n
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
RBLE_STATUS RBLE_BLP_Sensor_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_BLP_SENSOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Blood Pressure Send Measurements Info
 *
 *  Blood Pressure Send Measurements Infomation @n
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
RBLE_STATUS RBLE_BLP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_BLP_MEASUREMENTS_INFO *measurements_info)
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
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
									+ sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) );
		head_pnt->opcode		= RBLE_CMD_BLP_SENSOR_SEND_MEASUREMENTS;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, measurements_info->flag_stable_meas );	/* Stable or intermediary type of measurements */
		RBLE_DATA_SET8( Data_p, measurements_info->flags );				/* flags */
		RBLE_DATA_SET16( Data_p, measurements_info->press_val_field1 );	/* blood pressure value - systolic or cuff pressure */
		RBLE_DATA_SET16( Data_p, measurements_info->press_val_field2 );	/* blood pressure value - diastolic of sufield1 */
		RBLE_DATA_SET16( Data_p, measurements_info->press_val_field3 );	/* blood pressure value - MAP of sufield2 */

		RBLE_DATA_SET16( Data_p, measurements_info->stamp.year );		/* Year value */
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.month );		/* Month value */
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.day );		/* Day value */
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.hour );		/* Hour value */
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.min );		/* Minute value */
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.sec );		/* Second value */
		RBLE_DATA_SET8( Data_p, measurements_info->stamp.reserved );	/* Reserved */

		RBLE_DATA_SET16( Data_p, measurements_info->rate );				/* pulse rate */
		RBLE_DATA_SET8( Data_p, measurements_info->id );					/* user ID */
		RBLE_DATA_SET8( Data_p, measurements_info->reserved );			/* Reserved */
		RBLE_DATA_SET16( Data_p, measurements_info->meas_sts );			/* measurement status */
	}
	
	return ret;
}

#endif//#if (PRF_SEL_BLPS)
