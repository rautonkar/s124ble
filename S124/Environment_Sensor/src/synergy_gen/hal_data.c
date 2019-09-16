/* generated HAL source file - do not edit */
#include "hal_data.h"
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer0) && !defined(SSP_SUPPRESS_ISR_DTCELC_EVENT_SCI9_TXI)
#define DTC_ACTIVATION_SRC_ELC_EVENT_SCI9_TXI
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_0) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_0);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0
#endif
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_1) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_1);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1
#endif
#endif
#endif

dtc_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info,
  .activation_source = ELC_EVENT_SCI9_TXI,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer0,
  .irq_ipl = (BSP_IRQ_DISABLED) };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
#if (2) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart_ble) && !defined(SSP_SUPPRESS_ISR_SCI9)
SSP_VECTOR_DEFINE_CHAN(sci_uart_rxi_isr, SCI, RXI, 9);
#endif
#endif
#if (2) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart_ble) && !defined(SSP_SUPPRESS_ISR_SCI9)
SSP_VECTOR_DEFINE_CHAN(sci_uart_txi_isr, SCI, TXI, 9);
#endif
#endif
#if (2) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart_ble) && !defined(SSP_SUPPRESS_ISR_SCI9)
SSP_VECTOR_DEFINE_CHAN(sci_uart_tei_isr, SCI, TEI, 9);
#endif
#endif
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart_ble) && !defined(SSP_SUPPRESS_ISR_SCI9)
SSP_VECTOR_DEFINE_CHAN(sci_uart_eri_isr, SCI, ERI, 9);
#endif
#endif
sci_uart_instance_ctrl_t g_uart_ble_ctrl;

/** UART extended configuration for UARTonSCI HAL driver */
const uart_on_sci_cfg_t g_uart_ble_cfg_extend =
{ .clk_src = SCI_CLK_SRC_INT, .baudclk_out = false, .rx_edge_start = true, .noisecancel_en = false, .p_extpin_ctrl =
          NULL,
  .bitrate_modulation = true, .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX };

/** UART interface configuration */
const uart_cfg_t g_uart_ble_cfg =
{ .channel = 9, .baud_rate = 4800, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits =
          UART_STOP_BITS_1,
  .ctsrts_en = false, .p_callback = ble_uart_callback, .p_context = &g_uart_ble, .p_extend = &g_uart_ble_cfg_extend,
#define SYNERGY_NOT_DEFINED (1)                        
#if (SYNERGY_NOT_DEFINED == g_transfer0)
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &g_transfer0,
#endif            
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &SYNERGY_NOT_DEFINED,
#endif   
#undef SYNERGY_NOT_DEFINED            
  .rxi_ipl = (2),
  .txi_ipl = (2), .tei_ipl = (2), .eri_ipl = (BSP_IRQ_DISABLED), };

/* Instance structure to use this module. */
const uart_instance_t g_uart_ble =
{ .p_ctrl = &g_uart_ble_ctrl, .p_cfg = &g_uart_ble_cfg, .p_api = &g_uart_on_sci };
#if (2) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timer_ble) && !defined(SSP_SUPPRESS_ISR_GPT0)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 0);
#endif
#endif
static gpt_instance_ctrl_t g_timer_ble_ctrl;
static const timer_on_gpt_cfg_t g_timer_ble_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timer_ble_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 10, .unit = TIMER_UNIT_PERIOD_MSEC, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0, .autostart = true, .p_callback = rBLE_timer_isr, .p_context = &g_timer_ble, .p_extend =
          &g_timer_ble_extend,
  .irq_ipl = (2), };
/* Instance structure to use this module. */
const timer_instance_t g_timer_ble =
{ .p_ctrl = &g_timer_ble_ctrl, .p_cfg = &g_timer_ble_cfg, .p_api = &g_timer_on_gpt };
void g_hal_init(void)
{
    g_common_init ();
}
