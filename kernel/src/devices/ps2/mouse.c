#include "mouse.h"
#include "io.h"
#include "intr/isr.h"
#include "devices/lfb.h"

#include <printk.h>
#include <string.h>

float g_mouse_x_pos = 0, g_mouse_y_pos = 0;
float mouse_speed = 6;
uint32_t under_mouse_buffer = NULL;

mouse_state_t g_status;

int32_t mouse_get_x(void)
{
    return g_mouse_x_pos;
}

int32_t mouse_get_y(void)
{
    return g_mouse_y_pos;
}

mouse_state_t mouse_get_state(void)
{
    return g_status;
}

void mouse_wait(bool type)
{
    /*uint32_t time_out = 100000;
    if (type == false)
    {
        // suspend until status is 1
        while (time_out--)
            if ((inportb(PS2_CMD_PORT) & 1) == 1)
                return;
        return;
    }
    else
    {
        while (time_out--)
            if ((inportb(PS2_CMD_PORT) & 2) == 0)
                return;
    }*/

    return;
}

void mouse_write(uint8_t data)
{
    mouse_wait(true);
    outportb(PS2_CMD_PORT, 0xD4);
    mouse_wait(true);
    outportb(MOUSE_DATA_PORT, data);
}

uint8_t mouse_read(void)
{
    mouse_wait(false);
    return inportb(MOUSE_DATA_PORT);
}

void get_mouse_status(char status_byte, mouse_state_t *status) 
{
    memset(status, 0, sizeof(mouse_state_t));
    if (status_byte & 0x01)
        status->left_button = 1;
    if (status_byte & 0x02)
        status->right_button = 1;
    if (status_byte & 0x04)
        status->middle_button = 1;
    if (status_byte & 0x08)
        status->always_1 = 1;
    if (status_byte & 0x10)
        status->x_sign = 1;
    if (status_byte & 0x20)
        status->y_sign = 1;
    if (status_byte & 0x40)
        status->x_overflow = 1;
    if (status_byte & 0x80)
        status->y_overflow = 1;
}

void print_mouse_info(void)
{
    printk("Mouse X: %d, Y: %d\n", g_mouse_x_pos, g_mouse_y_pos);
    if (g_status.left_button)
        printk("Left button clicked");
    if (g_status.right_button)
        printk("Right button clicked");
    if (g_status.middle_button)
        printk("Middle button clicked");
}

void mouse_handler(regs_t *regs)
{
    static uint8_t mouse_cycle = 0;
    float prev_mouse_x_pos = g_mouse_x_pos, prev_mouse_y_pos = g_mouse_y_pos;

    static char mouse_byte[3];
    switch (mouse_cycle)
    {
        case 0:
            mouse_byte[0] = mouse_read();
            get_mouse_status(mouse_byte[0], &g_status);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = mouse_read();
            g_mouse_x_pos = g_mouse_x_pos + (int)(mouse_byte[1] * (mouse_speed / 10.0f));
            g_mouse_y_pos = g_mouse_y_pos - (int)(mouse_byte[2] * (mouse_speed / 10.0f));

            if (g_mouse_x_pos < 0)
                g_mouse_x_pos = 0;
            if (g_mouse_y_pos < 0)
                g_mouse_y_pos = 0;

            if (g_mouse_x_pos > (int)lfb_get_width())
                g_mouse_x_pos = (int)lfb_get_width() - 1;
            if (g_mouse_y_pos > (int)lfb_get_height())
                g_mouse_y_pos = (int)lfb_get_height() - 1;
            
            mouse_cycle = 0;
            break;
    }
    isr_end_interrupt(IRQ_BASE + 12);
}

// rates: 10, 20, 40, 60, 80, 100, 200
void set_mouse_rate(uint8_t rate)
{
    uint8_t status;

    outportb(MOUSE_DATA_PORT, MOUSE_CMD_SAMPLE_RATE);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE)
    {
        printk("error: failed to send mouse sample rate command\n");
        return;
    }
    outportb(MOUSE_DATA_PORT, rate);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE)
    {
        printk("error: failed to send mouse sample rate data\n");
        return;
    }
}

void mouse_initialize(void)
{
    uint8_t status;

    g_mouse_x_pos = lfb_get_width() * 0.5f;
    g_mouse_y_pos = lfb_get_height() * 0.5f;

    mouse_wait(true);
    outportb(PS2_CMD_PORT, 0xA8);

    outportb(MOUSE_DATA_PORT, MOUSE_CMD_MOUSE_ID);
    status = mouse_read();

    set_mouse_rate(10);

    mouse_wait(true);
    outportb(PS2_CMD_PORT, 0x20);
    mouse_wait(false);

    status = (inportb(MOUSE_DATA_PORT) | 2);

    mouse_wait(true);
    outportb(PS2_CMD_PORT, MOUSE_DATA_PORT);
    mouse_wait(true);
    outportb(MOUSE_DATA_PORT, status);

    mouse_write(MOUSE_CMD_SET_DEFAULTS);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE)
    {
        //kprintf("Error: Failed to set default mouse settings!\n");
        return;
    }

    mouse_write(MOUSE_CMD_ENABLE_PACKET_STREAMING);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE)
    {
        //kprintf("Error: Failed to enable mouse packet streaming!\n");
        return;
    }

    isr_register_interrupt_handler(IRQ_BASE + IRQ12_AUXILIARY, mouse_handler);
}