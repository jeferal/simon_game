#include "simon_matrix.hpp"

SimonMatrix::SimonMatrix(int i2cbus, int address) {
    char *end;
    int count,cont;
    char filename[20];
    unsigned char letter;
    int i,t,y;

    daddress = 0;

    //Start up the matrix

    size = I2C_SMBUS_BYTE;
	sprintf(filename, "/dev/i2c-%d", i2cbus);
	file = open(filename, O_RDWR);
	if (file<0) {
		if (errno == ENOENT) {
			fprintf(stderr, "Error: Could not open file "
				            "/dev/i2c-%d: %s\n", i2cbus, strerror(ENOENT));
		}
		else {
			fprintf(stderr, "Error: Could not open file " 
                            "`%s': %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) {
		fprintf(stderr, "Error: Could not set address to 0x%02x: %s\n", address, strerror(errno));
		exit(-1);
	}


	res = i2c_smbus_write_byte(file, daddress);
	if (res < 0) {
		fprintf(stderr, "Warning - write failed, filename=%s, daddress=%d\n",
			filename, daddress);
	}



	daddress = 0x21; // Start oscillator (page 10)
	printf("writing: 0x%02x\n", daddress);
	res = i2c_smbus_write_byte(file, daddress);

	daddress = 0x81; // Display on, blinking off (page 11)
	printf("writing: 0x%02x\n", daddress);
	res = i2c_smbus_write_byte(file, daddress);

	daddress = 0xef; // Full brightness (page 15)
	printf("Full brightness writing: 0x%02x\n", daddress);
	res = i2c_smbus_write_byte(file, daddress);

	daddress = 0x00; // Start writing to address 0 (page 13)
	printf("Start writing to address 0 writing: 0x%02x\n", daddress);
	res = i2c_smbus_write_byte(file, daddress);
}

SimonMatrix::~SimonMatrix() {
    clear();

    daddress = 0x20;
    for(daddress = 0xef; daddress >= 0xe0; daddress--) {
        res = i2c_smbus_write_byte(file, daddress);
    }
    close(file);
}

int SimonMatrix::_display_img(__u16 bmp[]) {
        
    for(int i=0; i<8; i++) {
        block[i] = (bmp[i]&0xfe) >>1 |
        (bmp[i]&0x01) << 7;
    }
    
    return res = i2c_smbus_write_i2c_block_data(file, daddress, 16, (__u8 *)block);
}

int SimonMatrix::clear() {
    unsigned short int clear_data[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    return _display_img(clear_data);
}

int SimonMatrix::display_number(int n) {

    unsigned short int number_data[8];
    
    number_data[0] = 0x00;
    number_data[1] = decimal_numbers_score[n][0];
    number_data[2] = decimal_numbers_score[n][1];
    number_data[3] = decimal_numbers_score[n][2];
    number_data[4] = decimal_numbers_score[n][3];
    number_data[5] = decimal_numbers_score[n][4];
    number_data[6] = 0x00;
    number_data[7] = 0x00;

    return _display_img(number_data);
}

int SimonMatrix::display_score(int n1, int n2) {

    unsigned short int number_data[8];
    
    number_data[0] = 0x00;
    number_data[1] = decimal_numbers_score[n1][0] | decimal_numbers_score[n2][0] << 4;
    number_data[2] = decimal_numbers_score[n1][1] | decimal_numbers_score[n2][1] << 4;
    number_data[3] = decimal_numbers_score[n1][2] | decimal_numbers_score[n2][2] << 4;
    number_data[4] = decimal_numbers_score[n1][3] | decimal_numbers_score[n2][3] << 4;
    number_data[5] = decimal_numbers_score[n1][4] | decimal_numbers_score[n2][4] << 4;
    number_data[6] = 0x00;
    number_data[7] = 0x00;

    return _display_img(number_data);
}

int SimonMatrix::display_score_time(int n1, int n2, int time) {

    unsigned short int number_data[8];
    
    number_data[0] = 0x00;
    number_data[1] = decimal_numbers_score[n1][0] | decimal_numbers_score[n2][0] << 4;
    number_data[2] = decimal_numbers_score[n1][1] | decimal_numbers_score[n2][1] << 4;
    number_data[3] = decimal_numbers_score[n1][2] | decimal_numbers_score[n2][2] << 4;
    number_data[4] = decimal_numbers_score[n1][3] | decimal_numbers_score[n2][3] << 4;
    number_data[5] = decimal_numbers_score[n1][4] | decimal_numbers_score[n2][4] << 4;
    number_data[6] = 0x00;
    number_data[7] = 0xff << time;

    return _display_img(number_data);
}