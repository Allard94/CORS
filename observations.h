/*
 * observations.h
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#ifndef OBSERVATIONS_H_
#define OBSERVATIONS_H_


#include <rtklib.h>
extern "C"
{
#include <libsbp/sbp.h>
#include <libswiftnav/track.h>
}

#pragma pack(push,1)
struct observation1 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[1];
	s32 whole_cycles[1];
	u8 fractional_part[1];
	u8 density[1];
	u16 lock[1];
	u16 sat[1];
	u8 code[1];
	u8 reserved[1];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation2 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[2];
	s32 whole_cycles[2];
	u8 fractional_part[2];
	u8 density[2];
	u16 lock[2];
	u16 sat[2];
	u8 code[2];
	u8 reserved[2];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation3 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[3];
	s32 whole_cycles[3];
	u8 fractional_part[3];
	u8 density[3];
	u16 lock[3];
	u16 sat[3];
	u8 code[3];
	u8 reserved[3];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation4 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[4];
	s32 whole_cycles[4];
	u8 fractional_part[4];
	u8 density[4];
	u16 lock[4];
	u16 sat[4];
	u8 code[4];
	u8 reserved[4];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation5 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[5];
	s32 whole_cycles[5];
	u8 fractional_part[5];
	u8 density[5];
	u16 lock[5];
	u16 sat[5];
	u8 code[5];
	u8 reserved[5];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation6 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[6];
	s32 whole_cycles[6];
	u8 fractional_part[6];
	u8 density[6];
	u16 lock[6];
	u16 sat[6];
	u8 code[6];
	u8 reserved[6];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation7 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[7];
	s32 whole_cycles[7];
	u8 fractional_part[7];
	u8 density[7];
	u16 lock[7];
	u16 sat[7];
	u8 code[7];
	u8 reserved[7];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation8 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[8];
	s32 whole_cycles[8];
	u8 fractional_part[8];
	u8 density[8];
	u16 lock[8];
	u16 sat[8];
	u8 code[8];
	u8 reserved[8];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation9 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[9];
	s32 whole_cycles[9];
	u8 fractional_part[9];
	u8 density[9];
	u16 lock[9];
	u16 sat[9];
	u8 code[9];
	u8 reserved[9];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation10 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[10];
	s32 whole_cycles[10];
	u8 fractional_part[10];
	u8 density[10];
	u16 lock[10];
	u16 sat[10];
	u8 code[10];
	u8 reserved[10];
};
#pragma pack(pop)

#pragma pack(push,1)
struct observation11 {
	uint32_t tow;
	uint16_t wn;
	uint8_t n_obs;
	u32 pseudorange[11];
	s32 whole_cycles[11];
	u8 fractional_part[11];
	u8 density[11];
	u16 lock[11];
	u16 sat[11];
	u8 code[11];
	u8 reserved[11];
};
#pragma pack(pop)

extern observation1 payload1;
extern observation2 payload2;
extern observation3 payload3;
extern observation4 payload4;
extern observation5 payload5;
extern observation6 payload6;
extern observation7 payload7;
extern observation8 payload8;
extern observation9 payload9;
extern observation10 payload10;
extern observation11 payload11;

void fill_payload(int nr_obs, u16 gpsweek, u32 seconds, rtcm_t rtcm);
//void fill_payload(const navigation_measurement_t *m, rtcm_t rtcm);

#endif /* OBSERVATIONS_H_ */
