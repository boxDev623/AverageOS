#include "mouse.h"
#include "intr/isr.h"
#include "devices/lfb.h"
#include "io.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <printk.h>

int g_mouse_x_pos = 0, g_mouse_y_pos = 0;
int prev_x = 0, prev_y = 0;
uint32_t under_mouse_buffer;

#define MOUSE_size_x 8
#define MOUSE_size_y 8

mouse_status_t g_status;

int32_t mouse_get_x(void)
{
    return g_mouse_x_pos;
}

int32_t mouse_get_y(void)
{
    return g_mouse_y_pos;
}

mouse_status_t mouse_get_status(void)
{
    return g_status;
}

void mouse_wait(bool type) {
    uint32_t time_out = 10000000;
    if (type == false) {
        // suspend until status is 1
        while (time_out--) {
            if ((inportb(PS2_CMD_PORT) & 1) == 1) {
                return;
            }
        }
        return;
    } else {
        while (time_out--) {
            if ((inportb(PS2_CMD_PORT) & 2) == 0) {
                return;
            }
        }
    }
}

void mouse_write(uint8_t data)
{
    // sending write command
    mouse_wait(true);
    outportb(PS2_CMD_PORT, 0xD4);
    mouse_wait(true);
    // finally write data to port
    outportb(MOUSE_DATA_PORT, data);
}

uint8_t mouse_read(void)
{
    mouse_wait(false);
    return inportb(MOUSE_DATA_PORT);
}

void get_mouse_status(char status_byte, mouse_status_t *status)
{
    memset(status, 0, sizeof(mouse_status_t));
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

void print_mouse_info(void) {
    printk("Mouse Demo X: %d, Y: %d\n", g_mouse_x_pos, g_mouse_y_pos);
    if (g_status.left_button) {
        printk("Left button clicked");
    }
    if (g_status.right_button) {
        printk("Right button clicked");
    }
    if (g_status.middle_button) {
        printk("Middle button clicked");
    }
}

void mouse_handler(regs_t *r)
{
    static uint8_t mouse_cycle = 0;
    static char mouse_byte[3];
    switch (mouse_cycle) {
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

            g_mouse_x_pos = g_mouse_x_pos + mouse_byte[1];
            g_mouse_y_pos = g_mouse_y_pos - mouse_byte[2];

            if (g_mouse_x_pos < 0)
                g_mouse_x_pos = 0;
            else if (g_mouse_x_pos > lfb_width)
                g_mouse_x_pos = lfb_width - 1;
            if (g_mouse_y_pos < 0)
                g_mouse_y_pos = 0;
            else if (g_mouse_y_pos > lfb_height)
                g_mouse_y_pos = lfb_height - 1;
            prev_x = g_mouse_x_pos;
            prev_y = g_mouse_y_pos;
            mouse_cycle = 0;
            break;
    }
    isr_end_interrupt(IRQ_BASE + 12);
}

/**
 * available rates 10, 20, 40, 60, 80, 100, 200
 */
void set_mouse_rate(uint8_t rate) {
    uint8_t status;

    outportb(MOUSE_DATA_PORT, MOUSE_CMD_SAMPLE_RATE);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        printk("error: failed to send mouse sample rate command\n");
        return;
    }
    outportb(MOUSE_DATA_PORT, rate);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        printk("error: failed to send mouse sample rate data\n");
        return;
    }
}

void mouse_initialize(void)
{
    uint8_t status;

    g_mouse_x_pos = 5;
    g_mouse_y_pos = 2;

    // enable mouse device
    mouse_wait(true);
    outportb(PS2_CMD_PORT, 0xA8);

    // print mouse id
    outportb(MOUSE_DATA_PORT, MOUSE_CMD_MOUSE_ID);
    status = mouse_read();

    //set_mouse_rate(10);

    //outportb(MOUSE_DATA_PORT, MOUSE_CMD_RESOLUTION);
    //outportb(MOUSE_DATA_PORT, 0);

    // enable the interrupt
    mouse_wait(true);
    outportb(PS2_CMD_PORT, 0x20);
    mouse_wait(false);
    // get and set second bit
    status = (inportb(MOUSE_DATA_PORT) | 2);
    // write status to port
    mouse_wait(true);
    outportb(PS2_CMD_PORT, MOUSE_DATA_PORT);
    mouse_wait(true);
    outportb(MOUSE_DATA_PORT, status);

    // set mouse to use default settings
    mouse_write(MOUSE_CMD_SET_DEFAULTS);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE)
    {
        printk("error: failed to set default mouse settings\n");
        return;
    }

    // enable packet streaming to receive
    mouse_write(MOUSE_CMD_ENABLE_PACKET_STREAMING);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE)
    {
        printk("error: failed to enable mouse packet streaming\n");
        return;
    }

    // set mouse handler
    isr_register_interrupt_handler(IRQ_BASE + 12, mouse_handler);
}