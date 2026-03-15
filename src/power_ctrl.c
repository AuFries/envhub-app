#include "power_ctrl.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <gpiod.h>

#define POWER_EVENT_DEV "/dev/input/event0"
#define POWER_GPIOCHIP "/dev/gpiochip0"
#define POWER_OFF_LINE 14 /* P8_16 on your current setup */

static pthread_t g_thread;
static atomic_bool g_thread_run = false;
static atomic_bool g_shutdown_requested = false;

static struct gpiod_chip *g_chip = NULL;
static struct gpiod_line *g_off_line = NULL;

static void *power_thread_fn(void *arg)
{
    (void)arg;

    int fd = open(POWER_EVENT_DEV, O_RDONLY);
    if (fd < 0)
    {
        syslog(LOG_ERR, "power_service: open(%s) failed: %s", POWER_EVENT_DEV, strerror(errno));
        return NULL;
    }

    syslog(LOG_INFO, "power_service: monitoring %s", POWER_EVENT_DEV);

    while (atomic_load(&g_thread_run))
    {
        struct input_event ev;
        ssize_t n = read(fd, &ev, sizeof(ev));

        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            syslog(LOG_ERR, "power_service: read failed: %s", strerror(errno));
            break;
        }

        if (n != (ssize_t)sizeof(ev))
        {
            syslog(LOG_WARNING, "power_service: short read: %zd", n);
            continue;
        }

        if (ev.type == EV_KEY && ev.code == KEY_POWER)
        {
            if (ev.value == 1)
            {
                syslog(LOG_INFO, "power_service: KEY_POWER pressed");
                atomic_store(&g_shutdown_requested, true);
            }
            else if (ev.value == 0)
            {
                syslog(LOG_INFO, "power_service: KEY_POWER released");
            }
        }
    }

    close(fd);
    syslog(LOG_INFO, "power_service: monitor thread exiting");
    return NULL;
}

bool power_service_init(void)
{
    if (g_off_line)
    {
        syslog(LOG_WARNING, "power_service: already initialized");
        return true;
    }

    g_chip = gpiod_chip_open(POWER_GPIOCHIP);
    if (!g_chip)
    {
        syslog(LOG_ERR, "power_service: gpiod_chip_open(%s) failed", POWER_GPIOCHIP);
        return false;
    }

    g_off_line = gpiod_chip_get_line(g_chip, POWER_OFF_LINE);
    if (!g_off_line)
    {
        syslog(LOG_ERR, "power_service: gpiod_chip_get_line(%d) failed", POWER_OFF_LINE);
        gpiod_chip_close(g_chip);
        g_chip = NULL;
        return false;
    }

    /*
     * Hold OFF low during normal runtime.
     * This request must remain active for the life of the process.
     */
    if (gpiod_line_request_output(g_off_line, "envhub-soft-power-off", 0) < 0)
    {
        syslog(LOG_ERR, "power_service: gpiod_line_request_output failed: %s", strerror(errno));
        g_off_line = NULL;
        gpiod_chip_close(g_chip);
        g_chip = NULL;
        return false;
    }

    atomic_store(&g_shutdown_requested, false);

    syslog(LOG_INFO, "power_service: OFF gpio initialized low on %s line %d", POWER_GPIOCHIP,
           POWER_OFF_LINE);

    return true;
}

bool power_service_start(void)
{
    if (!g_off_line)
    {
        syslog(LOG_ERR, "power_service: start called before init");
        return false;
    }

    atomic_store(&g_thread_run, true);

    int rc = pthread_create(&g_thread, NULL, power_thread_fn, NULL);
    if (rc != 0)
    {
        atomic_store(&g_thread_run, false);
        syslog(LOG_ERR, "power_service: pthread_create failed: %s", strerror(rc));
        return false;
    }

    return true;
}

void power_service_stop(void)
{
    if (!atomic_load(&g_thread_run))
    {
        return;
    }

    atomic_store(&g_thread_run, false);

    /*
     * The thread blocks in read(). Cancellation is acceptable here because
     * this is a simple helper thread and we close power by hardware anyway.
     */
    pthread_cancel(g_thread);
    pthread_join(g_thread, NULL);

    syslog(LOG_INFO, "power_service: stopped");
}

bool power_service_shutdown_requested(void)
{
    return atomic_load(&g_shutdown_requested);
}

void power_service_clear_shutdown_requested(void)
{
    atomic_store(&g_shutdown_requested, false);
}

bool power_service_power_cut(void)
{
    if (!g_off_line)
    {
        syslog(LOG_ERR, "power_service: power_cut called before init");
        return false;
    }

    syslog(LOG_INFO, "power_service: asserting OFF high");

    if (gpiod_line_set_value(g_off_line, 1) < 0)
    {
        syslog(LOG_ERR, "power_service: gpiod_line_set_value(1) failed: %s", strerror(errno));
        return false;
    }

    /*
     * Intentionally do NOT release the line or close the chip here.
     * The line must stay asserted until the hardware actually removes power.
     */
    syslog(LOG_INFO, "power_service: OFF asserted high");
    return true;
}

void power_service_deinit(void)
{
    /*
     * Only call this on non-power-cut error paths or normal teardown where
     * you explicitly want to release the line.
     */
    if (g_off_line)
    {
        gpiod_line_release(g_off_line);
        g_off_line = NULL;
    }

    if (g_chip)
    {
        gpiod_chip_close(g_chip);
        g_chip = NULL;
    }

    syslog(LOG_INFO, "power_service: deinitialized");
}