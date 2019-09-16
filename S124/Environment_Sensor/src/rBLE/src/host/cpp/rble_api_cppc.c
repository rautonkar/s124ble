/**
 ****************************************************************************************
 *
 * @file		rble_api_cppc.c
 *
 * @brief	rBLE Cycling Power Collector I/F API
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

#if (PRF_SEL_CPPC)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief Cycling Power Collector Role Enable Command
 *
 *  Cycling Power Collector Role Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	con_type	Connection Type
 *  @param[in]	*cps		Cycling Power Service Infomation
 *  @param[in]	*dis		Device Information Service Infomation
 *  @param[in]	*bas		Battery Service Infomation
 *  @param[in]	call_back	call_back Address
 *
 *  @retval		RBLE_OK			Success
 */
/******************************************************************************************/
RBLE_STATUS RBLE_CPP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_CPS_CONTENT *cps, RBLE_DIS_CONTENT *dis, RBLE_BATS_CONTENT *bas, RBLE_CPPC_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_CPPC_Info.Cpc_Handler ) {
			RBLE_CPPC_Info.conhdl      = conhdl;
			RBLE_CPPC_Info.Cpc_Handler = call_back;
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
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )	/* measurements */
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )											/* feature */
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )											/* sensor location */
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )						/* vector */
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )						/* control point */
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
									+ sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )  + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t )		/* BAS */
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) );
		head_pnt->opcode		= RBLE_CMD_CPP_COLLECTOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET8( Data_p, 0 );			/* Reserve */

		RBLE_DATA_SET16( Data_p, cps->shdl );
		RBLE_DATA_SET16( Data_p, cps->ehdl );
		RBLE_DATA_SET16( Data_p, cps->cp_meas_char_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_meas_val_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_meas_cfg_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_meas_brd_cfg_hdl );
		RBLE_DATA_SET8( Data_p, cps->cp_meas_prop );
		RBLE_DATA_SET8( Data_p, cps->reserved1 );
		RBLE_DATA_SET16( Data_p, cps->cp_feature_char_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_feature_val_hdl );
		RBLE_DATA_SET8( Data_p, cps->cp_feature_prop );
		RBLE_DATA_SET8( Data_p, cps->reserved2 );
		RBLE_DATA_SET16( Data_p, cps->sensor_loc_char_hdl );
		RBLE_DATA_SET16( Data_p, cps->sensor_loc_val_hdl );
		RBLE_DATA_SET8( Data_p, cps->sensor_loc_prop );
		RBLE_DATA_SET8( Data_p, cps->reserved3 );
		RBLE_DATA_SET16( Data_p, cps->cp_vector_char_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_vector_val_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_vector_cfg_hdl );
		RBLE_DATA_SET8( Data_p, cps->cp_vector_prop );
		RBLE_DATA_SET8( Data_p, cps->reserved4 );
		RBLE_DATA_SET16( Data_p, cps->cp_cp_char_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_cp_val_hdl );
		RBLE_DATA_SET16( Data_p, cps->cp_cp_cfg_hdl );
		RBLE_DATA_SET8( Data_p, cps->cp_cp_prop );
		RBLE_DATA_SET8( Data_p, cps->reserved5 );

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

		RBLE_DATA_SET16( Data_p, bas->shdl );
		RBLE_DATA_SET16( Data_p, bas->ehdl );
		RBLE_DATA_SET16( Data_p, bas->battery_lvl_char_hdl );
		RBLE_DATA_SET16( Data_p, bas->battery_lvl_val_hdl );
		RBLE_DATA_SET16( Data_p, bas->battery_lvl_cfg_hdl );
		RBLE_DATA_SET8( Data_p, bas->battery_lvl_prop );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Cycling Power Collector Role Disable Command
 *
 *  Cycling Power Collector Role Disable @n
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
RBLE_STATUS RBLE_CPP_Collector_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CPP_COLLECTOR_DISABLE;

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
RBLE_STATUS RBLE_CPP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
	head_pnt->opcode		= RBLE_CMD_CPP_COLLECTOR_READ_CHAR;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, char_code );

	return ret;
}
/******************************************************************************************/
/**
 *  @brief Write CP Control Point Command
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
RBLE_STATUS RBLE_CPP_Collector_Write_CP_Control_Point(uint16_t conhdl, RBLE_CPP_WR_CONTROL_POINT_INFO *wr_cp_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t )
                              + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint32_t ) + sizeof( uint8_t ) + sizeof( uint8_t )		/* OpCode ~ reserved2 */
                              + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t );	/* crank_len ~ mask_meas */
	head_pnt->opcode		= RBLE_CMD_CPP_COLLECTOR_WRITE_CONTROL_POINT;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, wr_cp_info->OpCode );
	RBLE_DATA_SET8( Data_p, wr_cp_info->reserved1 );
	RBLE_DATA_SET32( Data_p, wr_cp_info->cumulative_value );
	RBLE_DATA_SET8( Data_p, wr_cp_info->sensor_location );
	RBLE_DATA_SET8( Data_p, wr_cp_info->reserved2 );
	RBLE_DATA_SET16( Data_p, wr_cp_info->crank_length );
	RBLE_DATA_SET16( Data_p, wr_cp_info->chain_length );
	RBLE_DATA_SET16( Data_p, wr_cp_info->chain_weight );
	RBLE_DATA_SET16( Data_p, wr_cp_info->span_length );
	RBLE_DATA_SET16( Data_p, wr_cp_info->mask_meas_content );

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
RBLE_STATUS RBLE_CPP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Create Command Packet Header */
	head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
	head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
	head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t );
	head_pnt->opcode		= RBLE_CMD_CPP_COLLECTOR_WRITE_CHAR;

	/* Create Command Payloard */
	Data_p = &head_pnt->data[ 0 ];
	RBLE_DATA_SET16( Data_p, conhdl );
	RBLE_DATA_SET8( Data_p, char_code );
	RBLE_DATA_SET8( Data_p, 0 );			/* Reserve */
	RBLE_DATA_SET16( Data_p, cfg_val );

	return ret;
}

#endif//#if (PRF_SEL_CPPC)
