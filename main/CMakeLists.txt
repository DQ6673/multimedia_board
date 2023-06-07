set(srcs
    app_main.c
    app_display.c
    benchmark/img_benchmark_cogwheel_alpha16.c
    benchmark/img_benchmark_cogwheel_argb.c
    benchmark/img_benchmark_cogwheel_chroma_keyed.c
    benchmark/img_benchmark_cogwheel_indexed16.c
    benchmark/img_benchmark_cogwheel_rgb.c
    benchmark/img_benchmark_cogwheel_rgb565a8.c
    benchmark/lv_demo_benchmark.c
    benchmark/lv_font_bechmark_montserrat_12_compr_az.c.c
    benchmark/lv_font_bechmark_montserrat_16_compr_az.c.c
    benchmark/lv_font_bechmark_montserrat_28_compr_az.c.c
)

set(includes
    .
    ./benchmark
)

set(requires
    driver
    esp_lcd
    esp_timer
    heap
    lcd
    touch
    lvgl
)

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${includes}
    REQUIRES ${requires}
)
