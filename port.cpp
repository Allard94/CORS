/*
 * port.cpp
 *
 *  Created on: Nov 23, 2016
 *      Author: root
 */

#include <stdlib.h>
#include <stdio.h>

#include <libserialport.h>

#include "port.h"

struct sp_port *piksi_port = NULL;

uint32_t result = 0;

/* Reads the serial port. */
u32 piksi_port_read(u8 *buff, u32 n, void *context){
	(void)context;

	result = sp_blocking_read(piksi_port, buff, n, 0);
	return result;
}

uint32_t piksi_port_write(uint8_t *buff, uint32_t n, void *context){
	(void)context;

	result = sp_blocking_write(piksi_port, buff, n, 0);
	return result;
}

/* Checks if the serial port name has been set. */
void check_port_name(char* serial_port){
	if (!serial_port) {
		fprintf(stderr, "Please supply the serial port path where the Piksi is " \
	                    "connected!\n");
	    exit(EXIT_FAILURE);
	 }
}

/* Checks if serial device has been connected. */
void locate_port(char* serial_port){
	result = sp_get_port_by_name(serial_port, &piksi_port);
	if (result != SP_OK) {
	    fprintf(stderr, "Cannot find provided serial port!\n");
	    exit(EXIT_FAILURE);
	}
}

/* Opens serial port. */
void open_port(char* serial_port){
	result = sp_open(piksi_port, /*SP_MODE_READ_WRITE*/ SP_MODE_WRITE);
	if (result != SP_OK) {
		fprintf(stderr, "Cannot open %s for reading!\n", serial_port);
	    exit(EXIT_FAILURE);
	}
}

/* Port setup */
void setup_port()
{
	/* sets baudrate of serial port to 1000000. */
    result = sp_set_baudrate(piksi_port, /*1000000*/ 57600);
    if (result != SP_OK) {
    	fprintf(stderr, "Cannot set port baud rate!\n");
    	exit(EXIT_FAILURE);
    }

    /* Sets flow control type of serial port to none. */
    result = sp_set_flowcontrol(piksi_port, SP_FLOWCONTROL_NONE);
    if (result != SP_OK) {
    	fprintf(stderr, "Cannot set flow control!\n");
    	exit(EXIT_FAILURE);
    }

    /* Sets data bits of serial port to 8. */
    result = sp_set_bits(piksi_port, 8);
    if (result != SP_OK) {
    	fprintf(stderr, "Cannot set data bits!\n");
    	exit(EXIT_FAILURE);
    }

    /* Sets the parity settings of serial port to none. */
    result = sp_set_parity(piksi_port, SP_PARITY_NONE);
    if (result != SP_OK) {
    	fprintf(stderr, "Cannot set parity!\n");
    	exit(EXIT_FAILURE);
    }

    /* Sets the stop bits of serial port to 1. */
    result = sp_set_stopbits(piksi_port, 1);
    if (result != SP_OK) {
    	fprintf(stderr, "Cannot set stop bits!\n");
    	exit(EXIT_FAILURE);
    }
}


