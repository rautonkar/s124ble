/**
 ****************************************************************************************
 *
 * @file		rble_api_htpt.c
 *
 * @brief	rBLE-Host  Health Thermometer Profile Thermometer I/F API
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

#if (PRF_SEL_HTPT)

/**************************** Define Declaration **************************************/


/**************************** Structure Declaration ***********************************/

/**************************** PUBLIC Memory Declaration *******************************/


/**************************** PRIVATE Memory Declaration ******************************/

/**************************** PRIVATE PROTOTYPE Declaration ***************************/

/******************************* Program Erea *********************************************/

/******************************************************************************************/
/**
 *  @brief Health Thermometer Thermometer Enable
 *
 *  Health Thermometer Profile Thermometer Role Enable@n
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
 *  @param[in] *param		Thermometer Parameter
 *  @param[in] *call_back	Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HTP_Thermometer_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_HTP_THERM_PARAM *param, RBLE_HTPT_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

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
			if ( NULL == RBLE_HTPT_Info.Htt_Handler ) {
				RBLE_HTPT_Info.conhdl      = conhdl;
				RBLE_HTPT_Info.Htt_Handler = call_back;
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
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) );
		head_pnt->opcode		= RBLE_CMD_HTP_THERMOMETER_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, sec_lvl );
		RBLE_DATA_SET8( Data_p, con_type );

		RBLE_DATA_SET16( Data_p, param->temp_meas_ind_en );   /* Temperature measurement indication configuration */
		RBLE_DATA_SET16( Data_p, param->interm_temp_ntf_en ); /* Intermediate temperature notification configuration */
		RBLE_DATA_SET16( Data_p, param->meas_intv_ind_en );   /* Measurement interval indication configuration */
		RBLE_DATA_SET16( Data_p, param->meas_intv );         /* Measurement interval */

	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Health Thermometer Thermometer Disable
 *
 *  Health Thermometer Profile Thermometer Role disable@n
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
RBLE_STATUS RBLE_HTP_Thermometer_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_HTP_THERMOMETER_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Health Thermometer Send Temperature Info
 *
 *  Health Thermometer Send Temperature Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *  @param[in] temp_info	Temperature Infomation
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HTP_Thermometer_Send_Temp(uint16_t conhdl, RBLE_HTP_TEMP_INFO *temp_info)
{
	RBLE_STATUS 							ret = RBLE_OK;
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
		head_pnt->packet_length = ( sizeof( uint16_t )
									+ sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint32_t )
									+ sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t )+sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint8_t ) + sizeof( uint8_t ) );	/* Add Reserved */
		head_pnt->opcode		= RBLE_CMD_HTP_THERMOMETER_SEND_TEMP;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );

		RBLE_DATA_SET8( Data_p, temp_info->flag_stable_meas );	/* Stable or intermediary type of temperature */
		RBLE_DATA_SET8( Data_p, temp_info->flags );				/* flags */
		RBLE_DATA_SET32( Data_p, temp_info->temp_val );			/* temp value */

		RBLE_DATA_SET16( Data_p, temp_info->stamp.year );		/* Year value */
		RBLE_DATA_SET8( Data_p, temp_info->stamp.month );		/* Month value */
		RBLE_DATA_SET8( Data_p, temp_info->stamp.day );		/* Day value */
		RBLE_DATA_SET8( Data_p, temp_info->stamp.hour );		/* Hour value */
		RBLE_DATA_SET8( Data_p, temp_info->stamp.min );		/* Minute value */
		RBLE_DATA_SET8( Data_p, temp_info->stamp.sec );		/* Second value */
		RBLE_DATA_SET8( Data_p, temp_info->stamp.reserved );	/* Reserved */

		RBLE_DATA_SET8( Data_p, temp_info->type );				/* type */
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Measurement_Period_Indicate Request
 *
 *  Request  Measurement Period Indicate @n
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
RBLE_STATUS RBLE_HTP_Thermometer_Req_Measurement_Period_Ind(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_HTP_THERMOMETER_REQ_MEASUREMENT_PERIOD_IND;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	
	return ret;
}

#endif//#if (PRF_SEL_HTPT)
