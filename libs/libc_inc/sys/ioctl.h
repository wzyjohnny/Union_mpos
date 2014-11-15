#ifndef SYS_IOCTL_H_
#define SYS_IOCTL_H_

#include <stdint.h>
/******************** System IOCTL ********************/
#define IOCTL_SYS_VERSION           0x53595300
#define IOCTL_SYS_RESET             0x53595301
#define IOCTL_SYS_MDELAY            0x53595302
#define IOCTL_SYS_SHUTDOWN          0x53595303
#define IOCTL_SYS_SUSPEND           0x53595304

/******************** Serial IOCTL ********************/
/* Stop bit selections */
typedef enum {
    SERIAL_STOP_0_5 = 0,
    SERIAL_STOP_1,
    SERIAL_STOP_1_5,
    SERIAL_STOP_2
}serial_stop_bits_t;

/* Parity modes */
typedef enum {
    SERIAL_PARITY_NONE = 0,
    SERIAL_PARITY_EVEN,
    SERIAL_PARITY_ODD,
    SERIAL_PARITY_MARK,
    SERIAL_PARITY_SPACE
}serial_parity_t;

/* Word length */
typedef enum {
    SERIAL_WORD_LENGTH_5 = 5,
    SERIAL_WORD_LENGTH_6,
    SERIAL_WORD_LENGTH_7,
    SERIAL_WORD_LENGTH_8,
    SERIAL_WORD_LENGTH_9
}serial_word_length_t;

typedef enum {
    SERIAL_FLOW_NONE = 0,
    SERIAL_FLOW_XONXOFF_RX = (1 << 0), /* receive flow control, send xon/xoff when necessary */
    SERIAL_FLOW_XONXOFF_TX = (1 << 1), /* transmit flow control, act on received xon/xoff */
    SERIAL_FLOW_RTSCTS_RX = (1 << 2),  /* receive flow control, send RTS when necessary */
    SERIAL_FLOW_RTSCTS_TX = (1 << 3),  /* transmit flow control, act when not CTS */
    SERIAL_FLOW_DSRDTR_RX = (1 << 4),  /* receive flow control, send DTR when necessary */
    SERIAL_FLOW_DSRDTR_TX = (1 << 5)   /* transmit flow control, act when not DSR */
}serial_flow_ctrl_t;

typedef struct {
    uint32_t baud;
    serial_stop_bits_t stop;
    serial_parity_t parity;
    serial_word_length_t word_length;
    serial_flow_ctrl_t flow_ctrl;
}serial_info_t;

#define IOCTL_SERIAL_SET_PARAM          0x434F4D01
#define IOCTL_SERIAL_GET_PARAM          0x434F4D02
#define IOCTL_SERIAL_CLR_TXBUF          0x434F4D03
#define IOCTL_SERIAL_CLR_RXBUF          0x434F4D04
#define IOCTL_SERIAL_GET_RXLEN          0x434F4D05

/******************** CRC IOCTL ********************/
#define IOCTL_CRC_SET_ATTR              0x43524301

typedef struct {
    uint32_t poly;
    uint32_t seed;
    uint8_t xorout;
    uint8_t refin;
    uint8_t refout;
    uint8_t crc32;
}crc_attr_t;


/******************** RNG IOCTL ********************/
#define IOCTL_RNG_SET_ENTROPY           0x524E4701

/******************** CRYPTO IOCTL ********************/
#define IOCTL_CRYPTO_SET_KEY            0x43525901
#define IOCTL_CRYPTO_SET_ENCRYPT        0x43525902
#define IOCTL_CRYPTO_SET_DECRYPT        0x43525903
#define IOCTL_CRYPTO_CLR_KEY            0x43525904
#define IOCTL_CRYPTO_SET_ROOTKEY        0x43525905

/******************** ICC IOCTL ********************/
#define IOCTL_ICC_SELECT_SLOT           0x49434301
#define IOCTL_ICC_POWERUP               0x49434302
#define IOCTL_ICC_POWERDOWN             0x49434303
#define IOCTL_ICC_APDU                  0x49434304

#define EX_BUF_LEN 300

typedef struct {
    unsigned char exbuf[EX_BUF_LEN];
    int inlen;
    int resp;
}icc_ex_t;

/******************** LCD IOCTL ********************/
#define IOCTL_LCD_GET_INFO              0x4C434401
#define IOCTL_LCD_UPDATE                0x4C434402
#define IOCTL_LCD_BL                    0x4C434403
#define IOCTL_LCD_SCONTRAST             0x4C434404

typedef struct {
    uint32_t bits_per_pixel;
    uint32_t width;
    uint32_t height;
    uint32_t fbuf_len;
    uint8_t * fbuf;
    const char * name;
}lcd_info_t;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
}lcd_block_t;

/******************** USB IOCTL ********************/
#define IOCTL_USB_DETACH                0x55534201
#define IOCTL_USB_ATTACH                0x55534202

/******************** Flash IOCTL ********************/
#define IOCTL_FLASH_GET_INFO            0x464C4101
#define IOCTL_FLASH_COMMAND             0x464C4102

#define FLASH_CMD_ERASE     0x00
#define FLASH_CMD_READ      0x01
#define FLASH_CMD_WRITE     0x02

typedef struct {
    uint32_t flash_cmd;
    uint32_t offset;
    uint32_t length;
    uint8_t * buffer;
}flash_op_t;

typedef struct {
    const char *name;
    uint32_t base_addr;
    uint32_t page_shift;
    uint32_t sector_shift;
    uint32_t sector_nr;
    uint32_t read;
}flash_info_t;

int ioctl(int d, int request, void *arg);

#endif /* SYS_IOCTL_H_ */
