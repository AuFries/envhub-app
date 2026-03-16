#include "ui_helpers.h"

lv_obj_t *find_by_name_dfs(lv_obj_t *root, const char *target)
{
    if (!root)
        return NULL;

    const char *n = lv_obj_get_name(root); /* LVGL 9 */
    if (n && strcmp(n, target) == 0)
        return root;

    uint32_t cnt = lv_obj_get_child_cnt(root);
    for (uint32_t i = 0; i < cnt; i++)
    {
        lv_obj_t *hit = find_by_name_dfs(lv_obj_get_child(root, i), target);
        if (hit)
            return hit;
    }
    return NULL;
}