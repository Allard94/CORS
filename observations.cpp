/*
 * observations.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#include "observations.h"

observation1 payload1;
observation2 payload2;
observation3 payload3;
observation4 payload4;
observation5 payload5;
observation6 payload6;
observation7 payload7;
observation8 payload8;
observation9 payload9;
observation10 payload10;
observation11 payload11;

void fill_payload(int nr_obs, u16 gpsweek, u32 seconds, rtcm_t rtcm){
	switch(nr_obs){
		case 1 :
		  payload1.n_obs = rtcm.obs.n;
		  payload1.tow = seconds;
		  payload1.wn = gpsweek;
		  for(int i = 0; i < nr_obs; i++){
			  payload1.pseudorange[i] = rtcm.obs.data[i].P[0];
			  payload1.whole_cycles[i] = rtcm.obs.data[i].L[0];
			  payload1.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
			  payload1.density[i] = rtcm.obs.data[i].SNR[0];
			  payload1.lock[i] = rtcm.obs.data[i].LLI[0];
			  payload1.sat[i] = rtcm.obs.data[i].sat;
			  payload1.code[i] = rtcm.obs.data[i].code[0];
			  payload1.reserved[i] = 0;
		  }
		  break;
		case 2 :
			payload2.n_obs = rtcm.obs.n;
			payload2.tow = seconds;
			payload2.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload2.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload2.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload2.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload2.density[i] = rtcm.obs.data[i].SNR[0];
				payload2.lock[i] = rtcm.obs.data[i].LLI[0];
				payload2.sat[i] = rtcm.obs.data[i].sat;
				payload2.code[i] = rtcm.obs.data[i].code[0];
				payload2.reserved[i] = 0;
			}
			break;
		case 3 :
			payload3.n_obs = rtcm.obs.n;
			payload3.tow = seconds;
			payload3.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload3.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload3.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload3.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload3.density[i] = rtcm.obs.data[i].SNR[0];
				payload3.lock[i] = rtcm.obs.data[i].LLI[0];
				payload3.sat[i] = rtcm.obs.data[i].sat;
				payload3.code[i] = rtcm.obs.data[i].code[0];
				payload3.reserved[i] = 0;
			}
			break;
		case 4 :
			payload4.n_obs = rtcm.obs.n;
			payload4.tow = seconds;
			payload4.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload4.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload4.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload4.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload4.density[i] = rtcm.obs.data[i].SNR[0];
				payload4.lock[i] = rtcm.obs.data[i].LLI[0];
				payload4.sat[i] = rtcm.obs.data[i].sat;
				payload4.code[i] = rtcm.obs.data[i].code[0];
			payload4.reserved[i] = 0;
		  }
		  break;
		case 5 :
			payload5.n_obs = rtcm.obs.n;
			payload5.tow = seconds;
			payload5.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload5.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload5.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload5.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload5.density[i] = rtcm.obs.data[i].SNR[0];
				payload5.lock[i] = rtcm.obs.data[i].LLI[0];
				payload5.sat[i] = rtcm.obs.data[i].sat;
				payload5.code[i] = rtcm.obs.data[i].code[0];
				payload5.reserved[i] = 0;
			}
			break;
		case 6 :
			payload6.n_obs = rtcm.obs.n;
			payload6.tow = seconds;
			payload6.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload6.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload6.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload6.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload6.density[i] = rtcm.obs.data[i].SNR[0];
				payload6.lock[i] = rtcm.obs.data[i].LLI[0];
				payload6.sat[i] = rtcm.obs.data[i].sat;
				payload6.code[i] = rtcm.obs.data[i].code[0];
				payload6.reserved[i] = 0;
			}
			break;
		case 7 :
			payload7.n_obs = rtcm.obs.n;
			payload7.tow = seconds;
			payload7.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload7.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload7.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload7.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload7.density[i] = rtcm.obs.data[i].SNR[0];
				payload7.lock[i] = rtcm.obs.data[i].LLI[0];
				payload7.sat[i] = rtcm.obs.data[i].sat;
				payload7.code[i] = rtcm.obs.data[i].code[0];
				payload7.reserved[i] = 0;
			}
			break;
		case 8 :
			payload8.n_obs = rtcm.obs.n;
			payload8.tow = seconds;
			payload8.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload8.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload8.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload8.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload8.density[i] = rtcm.obs.data[i].SNR[0];
				payload8.lock[i] = rtcm.obs.data[i].LLI[0];
				payload8.sat[i] = rtcm.obs.data[i].sat;
				payload8.code[i] = rtcm.obs.data[i].code[0];
				payload8.reserved[i] = 0;
			}
			break;
		case 9 :
			payload9.n_obs = rtcm.obs.n;
			payload9.tow = seconds;
			payload9.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload9.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload9.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload9.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload9.density[i] = rtcm.obs.data[i].SNR[0];
				payload9.lock[i] = rtcm.obs.data[i].LLI[0];
				payload9.sat[i] = rtcm.obs.data[i].sat;
				payload9.code[i] = rtcm.obs.data[i].code[0];
				payload9.reserved[i] = 0;
			}
			break;
		case 10 :
			payload10.n_obs = rtcm.obs.n;
			payload10.tow = seconds;
			payload10.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload10.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload10.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload10.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload10.density[i] = rtcm.obs.data[i].SNR[0];
				payload10.lock[i] = rtcm.obs.data[i].LLI[0];
				payload10.sat[i] = rtcm.obs.data[i].sat;
				payload10.code[i] = rtcm.obs.data[i].code[0];
				payload10.reserved[i] = 0;
			}
			break;
		case 11 :
			payload11.n_obs = rtcm.obs.n;
			payload11.tow = seconds;
			payload11.wn = gpsweek;
			for(int i = 0; i < nr_obs; i++){
				payload11.pseudorange[i] = rtcm.obs.data[i].P[0];
				payload11.whole_cycles[i] = rtcm.obs.data[i].L[0];
				payload11.fractional_part[i] = rtcm.obs.data[i].L[0] / 256;
				payload11.density[i] = rtcm.obs.data[i].SNR[0];
				payload11.lock[i] = rtcm.obs.data[i].LLI[0];
				payload11.sat[i] = rtcm.obs.data[i].sat;
				payload11.code[i] = rtcm.obs.data[i].code[0];
				payload11.reserved[i] = 0;
			}
			break;
	}
}

//void fill_payload(navigation_measurement_t *m, rtcm_t rtcm){
//	int nr_obs = rtcm.obs.n;
//	for(int i = 0; i < nr_obs; i++){
//		m[i].raw_pseudorange = rtcm.obs.data[i].P[0];
//		m[i].raw_carrier_phase = rtcm.obs.data[i].L[0];
//		m[i].lock_counter = rtcm.obs.data[i].LLI[0];
//		m[i].sid.sat = rtcm.obs.data[i].sat;
//		m[i].sid.code = CODE_GPS_L1CA;
//
//	}
//}

