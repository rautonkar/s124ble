/**
 ****************************************************************************************
 *
 * @file		rble_api_cscpc.c
 *
 * @brief	rBLE Cycling Speed and Cadence Collector I/F API
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

#if (PRF_SEL_CSCC)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Cycling Speed and Cadence Collector Role Enable Command
 *
 *  Cycling Speed and Cadence Collector Role Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	con_type	Connection Type
 *  @param[in]	*cscs		Cycling Speed and Cadence Service Infomation
 *  @param[in]	*dis		Device Information Service Infomation
 *  @param[in]	call_back	call_back Address
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CSCP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_CSCS_CONTENT *cscs, RBLE_DIS_CONTENT *dis, RBLE_CSCPC_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_CSCPC_Info.Cscc_Handler ) {
			RBLE_CSCPC_Info.conhdl      = conhdl;
			RBLE_CSCPC_Info.Cscc_Handler = call_back;
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
									+ sizeof( uint16_t ) + sizeof( uint16_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
									+ sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )  + sizeof( uint8_t ) );
		head_pnt->opcode		= RBLE_CMD_CSCP_COLLECTOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET8( Data_p, 0 );			/* Reserve */

		RBLE_DATA_SET16( Data_p, cscs->shdl );
		RBLE_DATA_SET16( Data_p, cscs->ehdl );
		RBLE_DATA_SET16( Data_p, cscs->cycspd_meas_char_hdl );
		RBLE_DATA_SET16( Data_p, cscs->cycspd_meas_val_hdl );
		RBLE_DATA_SET16( Data_p, cscs->cycspd_meas_cfg_hdl );
		RBLE_DATA_SET8( Data_p, cscs->cycspd_meas_prop );
		RBLE_DATA_SET8( Data_p, cscs->reserved1 );
		RBLE_DATA_SET16( Data_p, cscs->csc_feature_char_hdl );
		RBLE_DATA_SET16( Data_p, cscs->csc_feature_val_hdl );
		RBLE_DATA_SET8( Data_p, cscs->csc_feature_prop );
		RBLE_DATA_SET8( Data_p, cscs->reserved2 );
		RBLE_DATA_SET16( Data_p, cscs->sensor_loc_char_hdl );
		RBLE_DATA_SET16( Data_p, cscs->sensor_loc_val_hdl );
		RBLE_DATA_SET8( Data_p, cscs->sensor_loc_prop );
		RBLE_DATA_SET8( Data_p, cscs->reserved3 );
		RBLE_DATA_SET16( Data_p, cscs->sc_cp_char_hdl );
		RBLE_DATA_SET16( Data_p, cscs->sc_cp_val_hdl );
		RBLE_DATA_SET16( Data_p, cscs->sc_cp_cfg_hdl );
		RBLE_DATA_SET8( Data_p, cscs->sc_cp_prop );
		RBLE_DATA_SET8( Data_p, cscs->reserved4 );

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
 *  @brief Cycling Speed and Cadence Collector Role Disable Command
 *
 *  Cycling Speed and Cadence Collector Role Disable @n
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
RBLE_STATUS RBLE_CSCP_Collector_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_COLLECTOR_DISABLE;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Read Characteristic Command
 *
 *  Read Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	char_code	Characteristic Code
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CSCP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_COLLECTOR_READ_CHAR;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, char_code );

	return ret;
}
/******************************************************************************************/
/**
 *  @brief Write SC Control Point Command
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
 *  @param[in]	sc_sp_info	SC Control Point Value
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CSCP_Collector_Write_SC_Control_Point(uint16_t conhdl, RBLE_CSCP_SC_CONTROL_POINT_INFO *sc_cp_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint32_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_COLLECTOR_WRITE_SC_CONTROL_POINT;

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

/******************************************************************************************/
/**
 *  @brief Write Characteristic Command
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
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CSCP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CSCP_COLLECTOR_WRITE_CHAR;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, char_code );
	RBLE_DATA_SET8( Data_p, 0 );			/* Reserve */
	RBLE_DATA_SET16( Data_p, cfg_val );

	return ret;
}

#endif//#if (PRF_SEL_CSCC)
