/*
 * port.h
 *
 *  Created on: Nov 23, 2016
 *      Author: root
 */

#ifndef PORT_H_
#define PORT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <libsbp/sbp.h>
#ifdef __cplusplus
}
#endif

u32 piksi_port_read(u8 *buff, u32 n, void *context);
uint32_t piksi_port_write(uint8_t *buff, uint32_t n, void *context);

void check_port_name(char* serial_port);
void locate_port(char* serial_port);
void open_port(char* serial_port);
void setup_port();

#endif /* PORT_H_ */
