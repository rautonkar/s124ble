/* HAL-only entry function */
#include "hal_data.h"
#include "rble_host.h"

#define RL78G1D_RESET IOPORT_PORT_01_PIN_11
#define RL78G1D_TOOL0 IOPORT_PORT_04_PIN_03

void hal_entry(void)
{
    RBLE_STATUS rble_status = RBLE_OK;
    extern RBLE_STATUS RBLE_Init_Timer(void * p_arg);
    extern bool APP_Init(void * p_uart);
    extern RBLE_STATUS APP_Run( void );
    extern const timer_instance_t g_timer_ble;
    extern const uart_instance_t g_uart_ble;

    uart_instance_t const * const p_uart_ble = &g_uart_ble;

    /** 0. Initialize random number generator */
    {
        ssp_err_t err = g_sce.p_api->open(g_sce.p_ctrl, g_sce.p_cfg);

        if(SSP_SUCCESS == err)
        {
            err = g_sce_trng.p_api->open(g_sce_trng.p_ctrl, g_sce_trng.p_cfg);
        }
    }

    /** 1. Initialize the RBLE Modem on the PMOD Interface */
    {
        /* Reset the Modem */
        g_ioport_on_ioport.pinWrite(RL78G1D_TOOL0, IOPORT_LEVEL_HIGH);
        g_ioport_on_ioport.pinWrite(RL78G1D_RESET, IOPORT_LEVEL_LOW);
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
        g_ioport_on_ioport.pinWrite(RL78G1D_RESET, IOPORT_LEVEL_HIGH);

        /* Wait for Reset to complete */
        R_BSP_SoftwareDelay(3, BSP_DELAY_UNITS_SECONDS);
    }

    /** 2. Initialize a 10ms Periodic RBLE Timer to control timing for the stack */
    rble_status = RBLE_Init_Timer((void*)&g_timer_ble);

    if(RBLE_OK==rble_status)
    {
        /** 3. Initialize the Application and the RBLE Host stack */
        rble_status = (true==APP_Init((void * )p_uart_ble)) ? RBLE_OK:RBLE_ERR;
    }

    while(RBLE_OK!=rble_status)
    {
        /* Wait here if initialization fails */
        ;
    }

    while(1)
    {
        /** 4. Run pending actions needed for the Application */
        rble_status = APP_Run();

        /** 5. Exchange information to the Modem over the UART */
        rble_status = rBLE_Run();
    }

    g_sce_trng.p_api->close(g_sce_trng.p_ctrl);
    g_sce.p_api->close(g_sce.p_ctrl);
}
