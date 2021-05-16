#ifndef SIMON_MATRIX_HPP_
#define SIMON_MATRIX_HPP_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../../util/i2c-dev.hpp"
#include "../../util/8x8font.hpp"

#include <vector>


class SimonMatrix {

    private:
        int res, i2cbus, daddress, address, size, file;

        __u16 block[I2C_SMBUS_BLOCK_MAX];
        
    public:

        SimonMatrix(int i2cbus, int address);
        ~SimonMatrix();

        int _display_img(__u16 bmp[]);
        int clear();

        int display_number(int n);
        int display_score(int n1, int n2);
        int display_numbers_time(int n1, int n2, int time);
        int display_score_time(int n, int time);
};

#endif /* SIMON_MATRIX_HPP_ */
