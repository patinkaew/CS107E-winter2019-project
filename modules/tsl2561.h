#ifndef TSL2561_H
#define TSL2561_H

enum reg_addr{
    CTRL        = 0x0;
    TIMING      = 0x1;

    CHIP_ID     = 0xa;
    DATA0_LOW   = 0xc;
    DATA0_HIGH  = 0xd;
    DATA1_LOW   = 0xe;
    DATA1_HIGH  = 0xf;
};

#endif TSL2561_H
