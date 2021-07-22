#include <cstdint>
#include "Arduino.h"
#include "SPI.h"

// Registers
#define  R_NOP        0x00
#define  R_DEVICEID   0x01
#define  R_STATUS     0x02
#define  R_SPICONFIG  0x03
#define  R_GENCONFIG  0x04
#define  R_BRDCONFIG  0x05
#define  R_SYNCCONFIG 0x06
#define  R_DACPWDWN   0x09
#define  R_DACRANGE   0x0A
#define  R_TRIGGER    0x0E
#define  R_BRDCAST    0x0F
#define  R_DACA       0x10
#define  R_DACB       0x11
#define  R_DACC       0x12
#define  R_DACD       0x13

// SPICONFIG masks
#define TEMPALM_EN(x) (x << 11)
#define DACBUSY_EN(x) (x << 10)
#define CRCALM_EN(x)  (x << 9)
#define DEV_PWDWN(x)  (x << 5)
#define CRC_EN(x)     (x << 4)
#define SDO_EN(x)     (x << 2)
#define FSDO(x)       (x << 1)

// read mask
#define RREG 0xC0

class DAC81404 {
    private:
        SPIClass *_spi;
        SPISettings _spi_settings;
        
        // pins
        int _cs_pin;
        int _rst_pin;
        int _ldac_pin;

        inline void cs_on();
        inline void cs_off();
        
        // TODO: datasheet says tcsh = SCLK falling edge 
        // to SYNC rising edge time = 5 ns (min)
        // handle with ASM nop? 
        inline void tcsh_delay() {
            delayMicroseconds(1);
        }

        void write_reg(uint8_t reg, uint16_t wdata);
        uint16_t read_reg(uint8_t reg);

        // DACRANGE is a write only register
        // Have to keep track of individual channels manually
        uint16_t dacrange_reg = 0;
        
        

    public:
        // some enums
        enum ChannelRange {U_5  = 0b0000, 
                   U_6  = 0b1000, 
                   U_10 = 0b0001, 
                   U_12 = 0b1001, 
                   U_20 = 0b0010, 
                   U_24 = 0b1010, 
                   U_40 = 0b0011, 
                   B_5  = 0b0101, 
                   B_6  = 0b1101, 
                   B_10 = 0b0110, 
                   B_12 = 0b1110, 
                   B_20 = 0b0111}; // bipolar

        enum SyncMode {SYNC_SS=0, SYNC_LDAC};

        // constructor
        DAC81404(int cspin, int rstpin = -1, int ldacpin = -1,
                 SPIClass *spi = &SPI, uint32_t spi_clock_hz=8000000);

        int init();
        
        // Hard-Reset
        
        // DAC power up/down
        // TODO: set/clear DAC_PWDWN bit in SPICONFIG reg

        // DAC channel powerdown
        void set_ch_enabled(int ch, bool state); // true/false = power ON/OFF
        bool get_ch_enabled(int ch);

        // Reference
        void set_int_reference(bool state);
        int get_int_reference();
        
        // Status

        // set/get range of a channel
        void set_range(int ch, ChannelRange range);
        int get_range(int ch);

        // write to a ch
        void set_out(int ch, uint16_t val);

        // sync 
        void set_sync(int ch, SyncMode);
};