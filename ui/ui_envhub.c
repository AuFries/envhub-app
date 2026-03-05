/**
 * @file ui_envhub.c
 */
#include "ui_envhub.h"
#include "lvgl.h"
#include <string.h>

static lv_obj_t * find_by_name_dfs(lv_obj_t * root, const char * target)
{
    if(!root) return NULL;

    const char * n = lv_obj_get_name(root);   /* LVGL 9 */
    if(n && strcmp(n, target) == 0) return root;

    uint32_t cnt = lv_obj_get_child_cnt(root);
    for(uint32_t i = 0; i < cnt; i++) {
        lv_obj_t * hit = find_by_name_dfs(lv_obj_get_child(root, i), target);
        if(hit) return hit;
    }
    return NULL;
}

static lv_obj_t * find_tiles_wrap_anywhere(void)
{
    lv_display_t * d = lv_display_get_default();
    if(!d) return NULL;

    lv_obj_t * roots[] = {
        lv_display_get_screen_active(d),
        lv_display_get_layer_top(d),
        lv_display_get_layer_sys(d),
        lv_display_get_layer_bottom(d),
    };

    for(size_t i = 0; i < sizeof(roots)/sizeof(roots[0]); i++) {
        lv_obj_t * hit = find_by_name_dfs(roots[i], "ui_tiles_wrap");
        if(hit) return hit;
    }
    return NULL;
}

static void apply_tiles_grid(lv_obj_t * wrap)
{
    if(!wrap) return;

    static lv_coord_t col_dsc[] = {110, 110, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {70, 70, 70, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_style_grid_column_dsc_array(wrap, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(wrap, row_dsc, 0);
    lv_obj_set_layout(wrap, LV_LAYOUT_GRID);

    lv_obj_set_style_pad_left(wrap, 6, 0);
    lv_obj_set_style_pad_right(wrap, 6, 0);
    lv_obj_set_style_pad_top(wrap, 6, 0);
    lv_obj_set_style_pad_row(wrap, 14, 0);
    lv_obj_set_style_pad_column(wrap, 14, 0);

    lv_obj_clear_flag(wrap, LV_OBJ_FLAG_SCROLLABLE);

    uint32_t n = lv_obj_get_child_cnt(wrap);
    if(n < 6) return;

    for(uint32_t i = 0; i < 6; i++) {
        lv_obj_t * tile = lv_obj_get_child(wrap, i);
        uint32_t col = i % 2;
        uint32_t row = i / 2;

        lv_obj_set_grid_cell(tile,
                             LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);
    }

    lv_obj_update_layout(wrap);
}

static void apply_grid_cb(lv_timer_t * t)
{
    lv_obj_t * wrap = find_tiles_wrap_anywhere();
    if(wrap) {
        apply_tiles_grid(wrap);
        lv_timer_del(t); /* done */
    }
}

void ui_envhub_init(const char * asset_path)
{
    lv_xml_register_font(NULL, "plex_sans_12", plex_sans_12);
    ui_envhub_init_gen(asset_path);

    /* The preview runtime may not have activated the screen yet; run ASAP and retry until found */
    lv_timer_create(apply_grid_cb, 1, NULL);
}