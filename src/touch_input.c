#include "touch_input.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static int g_fd = -1;
static int g_screen_w = 320;
static int g_screen_h = 240;

static int g_raw_x = 0;
static int g_raw_y = 0;
static bool g_pressed = false;

static int g_raw_x_left = 3700;
static int g_raw_x_right = 450;
static int g_raw_y_top = 3700;
static int g_raw_y_bottom = 450;

static int clamp_i(int v, int lo, int hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

static int map_range_inverted(int raw, int raw_hi, int raw_lo, int out_max)
{
    if (out_max <= 1 || raw_hi == raw_lo)
    {
        return 0;
    }

    if (raw_hi < raw_lo)
    {
        int tmp = raw_hi;
        raw_hi = raw_lo;
        raw_lo = tmp;
    }

    raw = clamp_i(raw, raw_lo, raw_hi);

    /* Convert so raw_hi -> 0 and raw_lo -> out_max-1 */
    long num = (long)(raw_hi - raw) * (long)(out_max - 1);
    long den = (long)(raw_hi - raw_lo);
    return (int)(num / den);
}

bool touch_input_init(const char *dev_path, int screen_w, int screen_h)
{
    if (!dev_path)
    {
        return false;
    }

    g_fd = open(dev_path, O_RDONLY | O_NONBLOCK);
    if (g_fd < 0)
    {
        perror("touch_input open");
        return false;
    }

    g_screen_w = screen_w;
    g_screen_h = screen_h;
    return true;
}

void touch_input_deinit(void)
{
    if (g_fd >= 0)
    {
        close(g_fd);
        g_fd = -1;
    }
}

bool touch_input_get_state(bool *pressed, int *x, int *y)
{
    if (g_fd < 0 || !pressed || !x || !y)
    {
        return false;
    }

    struct input_event ev;
    ssize_t n;

    while (1)
    {
        n = read(g_fd, &ev, sizeof(ev));
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            perror("touch_input read");
            return false;
        }

        if (n != (ssize_t)sizeof(ev))
        {
            break;
        }

        switch (ev.type)
        {
            case EV_KEY:
                if (ev.code == BTN_TOUCH)
                {
                    g_pressed = (ev.value != 0);
                }
                break;

            case EV_ABS:
                if (ev.code == ABS_X)
                {
                    g_raw_x = ev.value;
                }
                else if (ev.code == ABS_Y)
                {
                    g_raw_y = ev.value;
                }
                break;

            default:
                break;
        }
    }

    *pressed = g_pressed;
    *x = map_range_inverted(g_raw_x, g_raw_x_left, g_raw_x_right, g_screen_w);
    *y = map_range_inverted(g_raw_y, g_raw_y_top, g_raw_y_bottom, g_screen_h);

    return true;
}