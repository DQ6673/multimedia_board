#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

// pin define
#define LCD_PIN_DB0 13
#define LCD_PIN_DB1 12
#define LCD_PIN_DB2 11
#define LCD_PIN_DB3 10
#define LCD_PIN_DB4 9
#define LCD_PIN_DB5 46
#define LCD_PIN_DB6 3
#define LCD_PIN_DB7 20
#define LCD_PIN_RDX 14
#define LCD_PIN_WRX 21
#define LCD_PIN_DCX 47
#define LCD_PIN_CSX 48
#define LCD_PIN_RST -1
#define LCD_PIN_TE 36

// resolution
#define LCD_RES_H 480
#define LCD_RES_V 320

void lcd_init(void);

