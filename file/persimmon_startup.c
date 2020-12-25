/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-10-23     liuduanfei     first version
 */
 
#include <rtdevice.h>
#include <js_persim.h>

extern int js_user_init(void);
extern void persim_auto_start(void);
extern int jp_app_ready_init(void);
extern int rtgui_system_server_init(void);
extern void turn_on_lcd_backlight(void);
extern int touch_init(void);

static void persim_start(void)
{
    js_user_init();
    jp_app_ready_init();
    rtgui_system_server_init();
    js_persim_thread_init("/flash/test/app.js");
    js_app_ready_sem_release();

    //touch_init();
}
MSH_CMD_EXPORT(persim_start, .......);

