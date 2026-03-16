#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include "lvgl/lvgl.h"
#include <string.h>

lv_obj_t *find_by_name_dfs(lv_obj_t *root, const char *target);
void disable_scroll_recursive(lv_obj_t *obj);

#endif /* UI_HELPERS_H */