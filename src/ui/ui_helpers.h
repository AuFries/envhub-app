#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include "lvgl.h"

lv_obj_t *find_by_name_dfs(lv_obj_t *root, const char *target);

#endif /* UI_HELPERS_H */