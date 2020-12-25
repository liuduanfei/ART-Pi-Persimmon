/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-10-23     liuduanfei     first version
 */

#include <rtthread.h>
#include "touch.h"
#include <js_persim.h>

rt_thread_t touch_thread;
rt_device_t touch;

void touch_thread_entry(void *parameter)
{
    struct rt_touch_data *read_data;

    read_data = (struct rt_touch_data *)rt_calloc(1, sizeof(struct rt_touch_data));

    while(1)
    {
        rt_device_read(touch, 0, read_data, 1);

        if ((read_data->event == RT_TOUCH_EVENT_DOWN) || (read_data->event == RT_TOUCH_EVENT_MOVE))
        {
#ifdef PKG_USING_PERSIMMON_SRC
            rtgui_server_post_touch(read_data->y_coordinate, read_data->x_coordinate, 1);
#endif /* PKG_USING_PERSIMMON_SRC */
        }

        if (read_data->event == RT_TOUCH_EVENT_UP)
        {
#ifdef PKG_USING_PERSIMMON_SRC
            rtgui_server_post_touch(read_data->y_coordinate, read_data->x_coordinate, 0);
#endif /* PKG_USING_PERSIMMON_SRC */
        }
        rt_thread_delay(10);
    }
}

int touch_init(void)
{
    touch = rt_device_find("touch");

    rt_device_open(touch, RT_DEVICE_FLAG_RDONLY);

    struct rt_touch_info info;
    rt_device_control(touch, RT_TOUCH_CTRL_GET_INFO, &info);
    rt_kprintf("type       :%d\n", info.type);
    rt_kprintf("vendor     :%d\n", info.vendor);
    rt_kprintf("point_num  :%d\n", info.point_num);
    rt_kprintf("range_x    :%d\n", info.range_x);
    rt_kprintf("range_y    :%d\n", info.range_y);

    touch_thread = rt_thread_create("touch", touch_thread_entry, RT_NULL, 800, 10, 20);
    if (touch_thread == RT_NULL)
    {
        rt_kprintf("create touch thread err");

        return -RT_ENOMEM;
    }
    rt_thread_startup(touch_thread);

    return RT_EOK;
}

#ifdef PKG_USING_FT5426
#include <ft5426.h>

int ft5426_init(void)
{
    struct rt_touch_config config;
    config.dev_name = "i2c1";

    rt_hw_ft5426_init("touch", &config);

    return 0;
}
INIT_ENV_EXPORT(ft5426_init);

#endif /* PKG_USING_FT5426 */

