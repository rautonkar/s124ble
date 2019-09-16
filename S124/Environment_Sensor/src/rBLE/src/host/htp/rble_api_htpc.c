/**
 ****************************************************************************************
 *
 * @file		rble_api_htpc.c
 *
 * @brief	rBLE-Host Health Thermometer Profile Collector I/F API
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

#if (PRF_SEL_HTPC)

/**************************** Define Declaration **************************************/


/**************************** Structure Declaration ***********************************/

/**************************** PUBLIC Memory Declaration *******************************/


/**************************** PRIVATE Memory Declaration ******************************/

/**************************** PRIVATE PROTOTYPE Declaration ***************************/

/******************************* Program Erea *********************************************/

/******************************************************************************************/
/**
 *  @brief Health Thermometer Collector Enable
 *
 *  Health Thermometer Collector Role Enable@n
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
 *  @param[in] con_type		Connection Type
 *  @param[in] *hts			Health Thermometer Service Infomation
 *  @param[in] *dis			Device Information Service Infomation
 *  @param[in] *call_back	Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HTP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_HTS_CONTENT *hts, RBLE_DIS_CONTENT *dis, RBLE_HTPC_EVENT_HANDLER call_back)
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
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_HTPC_Info.Htc_Handler ) {
			RBLE_HTPC_Info.conhdl      = conhdl;
			RBLE_HTPC_Info.Htc_Handler = call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
									+ sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
									+ sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
									+ sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )  + sizeof( uint8_t ) );
		head_pnt->opcode		= RBLE_CMD_HTP_COLLECTOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET8( Data_p, 0 );			/* Reserve */

		RBLE_DATA_SET16( Data_p, hts->shdl );
		RBLE_DATA_SET16( Data_p, hts->ehdl );
		RBLE_DATA_SET16( Data_p, hts->temp_meas_char_hdl );
		RBLE_DATA_SET16( Data_p, hts->temp_meas_val_hdl );
		RBLE_DATA_SET16( Data_p, hts->temp_meas_cfg_hdl );
		RBLE_DATA_SET8( Data_p, hts->temp_meas_prop );
		RBLE_DATA_SET8( Data_p, hts->reserved );
		RBLE_DATA_SET16( Data_p, hts->temp_type_char_hdl );
		RBLE_DATA_SET16( Data_p, hts->temp_type_val_hdl );
		RBLE_DATA_SET8( Data_p, hts->temp_type_prop );
		RBLE_DATA_SET8( Data_p, hts->reserved2 );
		RBLE_DATA_SET16( Data_p, hts->interm_temp_char_hdl );
		RBLE_DATA_SET16( Data_p, hts->interm_temp_val_hdl );
		RBLE_DATA_SET16( Data_p, hts->interm_temp_cfg_hdl );
		RBLE_DATA_SET8( Data_p, hts->interm_temp_prop );
		RBLE_DATA_SET8( Data_p, hts->reserved3 );
		RBLE_DATA_SET16( Data_p, hts->meas_intv_char_hdl );
		RBLE_DATA_SET16( Data_p, hts->meas_intv_val_hdl );
		RBLE_DATA_SET16( Data_p, hts->meas_intv_cfg_hdl );
		RBLE_DATA_SET16( Data_p, hts->valid_range_hdl );
		RBLE_DATA_SET8( Data_p, hts->meas_intv_prop );
		RBLE_DATA_SET8( Data_p, hts->reserved4 );

		RBLE_DATA_SET16( Data_p, dis->shdl );
		RBLE_DATA_SET16( Data_p, dis->ehdl );
		RBLE_DATA_SET16( Data_p, dis->sys_id_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->sys_id_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->sys_id_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved );
		RBLE_DATA_SET16( Data_p, dis->model_nb_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->model_nb_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->model_nb_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved2 );
		RBLE_DATA_SET16( Data_p, dis->serial_nb_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->serial_nb_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->serial_nb_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved3 );
		RBLE_DATA_SET16( Data_p, dis->fw_rev_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->fw_rev_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->fw_rev_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved4 );
		RBLE_DATA_SET16( Data_p, dis->hw_rev_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->hw_rev_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->hw_rev_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved5 );
		RBLE_DATA_SET16( Data_p, dis->sw_rev_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->sw_rev_val_hdl );
		RBLE_DATA_SET8( Data_p, dis-> sw_rev_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved6 );
		RBLE_DATA_SET16( Data_p, dis->manuf_name_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->manuf_name_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->manuf_name_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved7 );
		RBLE_DATA_SET16( Data_p, dis->ieee_certif_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->ieee_certif_val_hdl );
		RBLE_DATA_SET8( Data_p, dis->ieee_certif_prop );
		RBLE_DATA_SET8( Data_p, dis->reserved8 );

	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Health Thermometer Collector Disable
 *
 *  Health Thermometer Collector Role disable@n
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
RBLE_STATUS RBLE_HTP_Collector_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_HTP_COLLECTOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );

	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Read Characteristic Info
 *
 *  Read Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *  @param[in] char_code	Characteristic Code
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HTP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );		/* Add Reserved */
		head_pnt->opcode		= RBLE_CMD_HTP_COLLECTOR_READ_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, char_code );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Characteristic Info
 *
 *  Write Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	char_code	Characteristic Code
 *  @param[in]	cfg_val		Configure Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HTP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_HTP_COLLECTOR_WRITE_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, char_code );
		RBLE_DATA_SET8( Data_p, 0 );				/* Reserved */
		RBLE_DATA_SET16( Data_p, cfg_val );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Set Measurement Period
 *
 *  Set Measurement Period @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	intv		Interval
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HTP_Collector_Set_Measurement_Period(uint16_t conhdl, uint16_t intv)
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
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint16_t ) );
		head_pnt->opcode		= RBLE_CMD_HTP_COLLECTOR_SET_MEASUREMENT_PERIOD;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET16( Data_p, intv );

	}

	return ret;
}

#endif//#if (PRF_SEL_HTPC)
