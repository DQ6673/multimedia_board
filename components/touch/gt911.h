typedef struct
{
    void (*init)(void);
    void (*read_cfg)(void);
    void (*write_cfg)(void);
    void (*swap_x_y)(void);
    void (*mirror_x)(void);
    void (*mirror_y)(void);

    uint8_t point_num;

    uint8_t point1_track_id;
    uint16_t point1_pos_x;
    uint16_t point1_pos_y;
    uint16_t point1_size;

} gt911_t;

typedef gt911_t *gt911_handle_t;

gt911_handle_t gt911_new_dev(void);
void gt911_read_pos(gt911_handle_t handle);

#define GT911_EVENT_TOUCH (1 << 0)

#define GTP_CONFIG_VESION 0x8047
#define GTP_XMAX_LOW 0x8048
#define GTP_XMAX_HIGH 0x8049
#define GTP_YMAX_LOW 0x804A
#define GTP_YMAX_HIGH 0x804B
#define GTP_TOUCHNUM 0x804C       // Êä³ö´¥µã¸öÊýÉÏÏÞ(1-5)
#define GTP_MOUDE_SWITCH1 0x804D  // ¿ÉÒÔÉèÖÃXY×ø±ê½»»»£¬ÉèÖÃÖÐ¶Ï´¥·¢·½Ê½
#define GTP_MOUDE_SWITCH2 0x804E  // Water Proof Disable
#define GTP_SNAKE_COUNT 0x804F    // ÊÖÖ¸°´ÏÂ/ËÉ¿ªÈ¥¶¶´ÎÊý
#define GTP_LARGE_TOUCH 0x8051    // ´óÃæ»ý½Ó´¥µã¸öÊý
#define GTP_NOISE_REDUCE 0x8052   // ÔëÒôÏû³ýÖµ(0-15)
#define GTP_SCNTOUCHLEVEL 0x8053  // ÆÁÄ»´¥µã´ÓÎÞµ½ÓÐµÄ·§Öµ
#define GTP_SCNLEAVELEVEL 0x8054  // ÆÁÄ»´¥µã´ÓÓÐµ½ÎÞµÄ·§Öµ
#define GTP_LOWPOWERCTL 0x8055    // ½øµÍ¹¦ºÄµÄÊ±¼ä(0-15Ãë)
#define GTP_REFRESHRATE 0x8056    // ×ø±êÉÏ±¨ÂÊ(5+N ms)
#define GTP_TOPBOTTOMSPACE 0x805B // ÉÏÏÂ¿Õ°×Çø(ÒÔ32ÎªÏµÊý)
#define GTP_LEFTRIGHTSPACE 0x805C // ×óÓÒ¿Õ°×Çø(ÒÔ32ÎªÏµÊý)
#define GTP_STRETCH_R0 0x805E     // À­ÉìÇø¼ä1ÏµÊý
#define GTP_STRETCH_R1 0x805F     // À­ÉìÇø¼ä2ÏµÊý
#define GTP_STRETCH_R2 0x8060     // À­ÉìÇø¼ä3ÏµÊý
#define GTP_STRETCH_RM 0x8061     // ¸÷À­ÉìÇø¼ä»ùÊý
#define GTP_DRV_GROUPA_NUM 0x8062
#define GTP_DRV_GROUPB_NUM 0x8063
#define GTP_SENSOR_NUM 0x8064
#define GTP_FREQA_FACTOR 0x8065
#define GTP_FREQB_FACTOR 0x8066
#define GTP_PANNEL_BITFREQL 0x8067 // Çý¶¯×éA,BµÄ»ùÆµL
#define GTP_PANNEL_BIRFREQH 0x8068 // Çý¶¯×éA,BµÄ»ùÆµH
#define GTP_PANNEL_TX_GAIN 0x806B  //
#define GTP_PANNEL_RX_GAIN 0x806C  //
#define GTP_PANNEL_DUMP_SFT 0x806D // ÆÁÔ­Ê¼Öµ·Å´óÏµÊý
#define GTP_FREQ_HOP_START 0x807A  // ÌøÆµ·¶Î§µÄÆðµãÆµÂÊ(ÒÔ2KHzÎªµ¥Î»,ÀýÈç50´ú±í100KHz)
#define GTP_FREQ_HOP_END 0x807B    // ÌøÆµ·¶Î§µÄÖÕµãÆµÂÊ(ÒÔ2KHzÎªµ¥Î»,ÀýÈç150´ú±í300KHz)
#define GTP_NOISE_DET_TIMES 0x807C // ÔëÒô¼ì²â´ÎÊý
#define GTP_HOPPING_FLAG 0x807D    // ÔëÒô¼ì²â³¬Ê±Ê±¼ä(ÒÔÃëÎªµ¥Î»)
#define GTP_HOP_THRESHOLD 0x807E   // ×îÓÅÆµÂÊÑ¡¶¨Ìõ¼þ,µ±Ç°¹¤×÷ÆµÂÊ¸ÉÈÅÁ¿Ò»×îÐ¡¸ÉÈÅÁ¿>Éè¶¨Öµ*4,ÔòÑ¡¶¨×îÓÅÆµÂÊºÍÌøÆµ
#define GTP_NOISE_THRESHOLD 0x807F // ÅÐ±ðÓÐ¸ÉÈÅµÄÃÅÏÞ
#define GTP_HOP_SEG1_L 0x8082      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î1ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG1_H 0x8083      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î1ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG1_FACTOR 0x8084 // ÌøÆµ¼ì²âÇø¼äÆµ¶Î1ÖÐÐÄµã±¶ÆµÏµÊý£¨ÊÊÓÃÓÚÇý¶¯A£¬Çý¶¯BÔÚ´Ë»ù´¡ÉÏ»»Ëã³öÀ´£©
#define GTP_HOP_SEG2_L 0x8085      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î2ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG2_H 0x8086      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î2ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG2_FACTOR 0x8087 // ÌøÆµ¼ì²âÇø¼äÆµ¶Î2ÖÐÐÄµã±¶ÆµÏµÊý£¨ÊÊÓÃÓÚÇý¶¯A£¬Çý¶¯BÔÚ´Ë»ù´¡ÉÏ»»Ëã³öÀ´£©
#define GTP_HOP_SEG3_L 0x8088      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î3ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG3_H 0x8089      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î3ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG3_FACTOR 0x808A // ÌøÆµ¼ì²âÇø¼äÆµ¶Î3ÖÐÐÄµã±¶ÆµÏµÊý£¨ÊÊÓÃÓÚÇý¶¯A£¬Çý¶¯BÔÚ´Ë»ù´¡ÉÏ»»Ëã³öÀ´£©
#define GTP_HOP_SEG4_L 0x808B      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î4ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG4_H 0x808C      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î4ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG4_FACTOR 0x808D // ÌøÆµ¼ì²âÇø¼äÆµ¶Î4ÖÐÐÄµã±¶ÆµÏµÊý£¨ÊÊÓÃÓÚÇý¶¯A£¬Çý¶¯BÔÚ´Ë»ù´¡ÉÏ»»Ëã³öÀ´£©
#define GTP_HOP_SEG5_L 0x808E      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î5ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG5_H 0x8F80      // ÌøÆµ¼ì²âÇø¼äÆµ¶Î5ÖÐÐÄµã»ùÆµ(ÊÔÓÃÓëÇý¶¯A,B)
#define GTP_HOP_SEG5_FACTOR 0x8090 // ÌøÆµ¼ì²âÇø¼äÆµ¶Î5ÖÐÐÄµã±¶ÆµÏµÊý£¨ÊÊÓÃÓÚÇý¶¯A£¬Çý¶¯BÔÚ´Ë»ù´¡ÉÏ»»Ëã³öÀ´£©
#define GTP_CONFIG_CHKSUM 0x80FF   // ÅäÖÃÐÅÏ¢Ð£Ñé(0x8047µ½0x80FE×Ö½ÚºÍµÄ²¹Âë)
#define GTP_CONFIG_FRESH 0x8100    // ÅäÖÃÒÑ¸üÐÂ±ê¼Ç(ÓÉÖ÷¿ØÐ´Èë)

// Read Only
#define GTP_PRODUCTID1 0x8140   // productID (first byte ASCIIÂë)
#define GTP_PRODUCTID2 0x8141   // productID (second byte ASCIIÂë)
#define GTP_PRODUCTID3 0x8142   // productID (third byte ASCIIÂë)
#define GTP_PRODUCTID4 0x8143   // productID (forth byte ASCIIÂë)
#define GTP_FIRM_VER_L 0x8144   // Firmware Vesion(16½øÖÆ Low Byte)
#define GTP_FIRM_VER_H 0x8145   // Firmware Vesion(16½øÖÆ High Byte)
#define GTP_X_COOR_RES_L 0x8146 // x coordinate resolution (low byte)
#define GTP_X_COOR_RES_H 0x8147 // x coordinate resolution (High byte)
#define GTP_Y_COOR_RES_L 0x8148 // y coordinate resolution (low byte)
#define GTP_Y_COOR_RES_H 0x8149 // y coordinate resolution (High byte)
#define GTP_VENDOR_ID 0x814A    // Vendor_id   (µ±Ç°Ä£×éÑ¡ÏîÐÅÏ¢)

/***********************************************************************************/

#define GTP_STATUS_RW 0x814E // ¿É¶ÁÐ´(ÓÃÀ´±êÊ¶×´Ì¬£¬¿É¶ÁÈ¡µÄ×ø±êµã¸öÊý)

/***********************************************************************************/
#define GTP_TRACK1_ID 0x814F     // track ID
#define GTP_POINT1_X_L 0x8150    // point 1 x coordinate (low byte)
#define GTP_POINT1_X_H 0x8151    // point 1 x coordinate (high byte)
#define GTP_POINT1_Y_L 0x8152    // point 1 y coordinate (low byte)
#define GTP_POINT1_Y_H 0x8153    // point 1 y coordinate (high byte)
#define GTP_POINT1_SIZE_L 0x8154 // Point 1 Size(low byte)
#define GTP_POINT1_SIZE_H 0x8155 // Point 1 Size(high byte)

#define GTP_TRACK2_ID 0x8157     // track ID
#define GTP_POINT2_X_L 0x8158    // point 2 x coordinate (low byte)
#define GTP_POINT2_X_H 0x8159    // point 2 x coordinate (high byte)
#define GTP_POINT2_Y_L 0x815A    // point 2 y coordinate (low byte)
#define GTP_POINT2_Y_H 0x815B    // point 2 y coordinate (high byte)
#define GTP_POINT2_SIZE_L 0x815C // Point 2 Size(low byte)
#define GTP_POINT2_SIZE_H 0x815D // Point 2 Size(high byte)

#define GTP_TRACK3_ID 0x815F     // track ID
#define GTP_POINT3_X_L 0x8160    // point 3 x coordinate (low byte)
#define GTP_POINT3_X_H 0x8161    // point 3 x coordinate (high byte)
#define GTP_POINT3_Y_L 0x8162    // point 3 y coordinate (low byte)
#define GTP_POINT3_Y_H 0x8163    // point 3 y coordinate (high byte)
#define GTP_POINT3_SIZE_L 0x8164 // Point 3 Size(low byte)
#define GTP_POINT3_SIZE_H 0x8165 // Point 3 Size(high byte)

#define GTP_TRACK4_ID 0x8167     // track ID
#define GTP_POINT4_X_L 0x8168    // point 4 x coordinate (low byte)
#define GTP_POINT4_X_H 0x8169    // point 4 x coordinate (high byte)
#define GTP_POINT4_Y_L 0x816A    // point 4 y coordinate (low byte)
#define GTP_POINT4_Y_H 0x816B    // point 4 y coordinate (high byte)
#define GTP_POINT4_SIZE_L 0x816C // Point 4 Size(low byte)
#define GTP_POINT4_SIZE_H 0x816D // Point 4 Size(high byte)

#define GTP_TRACK5_ID 0x816F     // track ID
#define GTP_POINT5_X_L 0x8170    // point 5 x coordinate (low byte)
#define GTP_POINT5_X_H 0x8171    // point 5 x coordinate (high byte)
#define GTP_POINT5_Y_L 0x8172    // point 5 y coordinate (low byte)
#define GTP_POINT5_Y_H 0x8173    // point 5 y coordinate (high byte)
#define GTP_POINT5_SIZE_L 0x8174 // Point 5 Size(low byte)
#define GTP_POINT5_SIZE_H 0x8175 // Point 5 Size(high byte)
