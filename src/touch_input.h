#ifndef TOUCH_INPUT_H
#define TOUCH_INPUT_H

#include <stdbool.h>

bool touch_input_init(const char *dev_path, int screen_w, int screen_h);
void touch_input_deinit(void);
bool touch_input_get_state(bool *pressed, int *x, int *y);

#endif /* TOUCH_INPUT_H */