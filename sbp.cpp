/*
 * sbp.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <string>
#include <iostream>


#include "sbp.h"
#include "observations.h"
#include "port.h"

extern "C"
{
#include <libsbp/logging.h>
#include <libsbp/sbp.h>
#include <libsbp/observation.h>
#include <libswiftnav/track.h>
#include <libswiftnav/signal.h>
}

#define SBP_FRAMING_MAX_PAYLOAD_SIZE 255
#define MSG_OBS_HEADER_SEQ_SHIFT 4u
#define MSG_OBS_HEADER_SEQ_MASK ((1 << 4u) - 1)
#define MSG_OBS_HEADER_MAX_SIZE MSG_OBS_HEADER_SEQ_MASK
#define MSG_OBS_TOW_MULTIPLIER ((double)1000.0)
#define MSG_OBS_P_MULTIPLIER ((double)5e1)
#define MSG_OSB_LF_MULTIPLIER ((double) (1<<8))
#define MSG_OBS_SNR_MULTIPLIER ((float)4)

msg_pos_llh_t pos_llh;
msg_pos_ecef_t pos_ecef;
msg_baseline_ecef_t base_ecef;
msg_log_t logging;

sbp_state_t sbp_state;

static sbp_msg_callbacks_node_t pos_llh_node;
static sbp_msg_callbacks_node_t base_ecef_node;
static sbp_msg_callbacks_node_t pos_ecef_node;
static sbp_msg_callbacks_node_t log_node;

u16 msg_obs_max_size = 102;

sbp_gnss_signal_t sid_to_sbp(const gnss_signal_t from){
	sbp_gnss_signal_t sbp_sid = {
			from.code,
			from.sat,
	};

	/* Maintain legacy compatibility with GPS PRN encoding. Sat values for other
	* constellations are "real" satellite identifiers.
	*/
	if (sid_to_constellation(from) == CONSTELLATION_GPS)
		sbp_sid.sat -= GPS_FIRST_PRN;

	return sbp_sid;
}

void pack_obs_header(const gps_time_t *t, u8 total, u8 count, observation_header_t *msg){
    msg->t.tow = (u32)round(t->tow * MSG_OBS_TOW_MULTIPLIER);
    msg->t.wn  = t->wn;
    msg->n_obs = ((total << MSG_OBS_HEADER_SEQ_SHIFT) |
                 (count & MSG_OBS_HEADER_SEQ_MASK));
}

s8 pack_obs_content(double P, double L, double snr, u16 lock_counter,
                    gnss_signal_t sid, packed_obs_content_t *msg){

	s64 P_fp = llround(P * MSG_OBS_P_MULTIPLIER);
	if (P < 0 || P_fp > UINT32_MAX) {
		log_error("observation message packing: P integer overflow (%f)", P);
		return -1;
	}

	msg->P = (u32)P_fp;

	double Li = floor(-L);
	if (Li < INT32_MIN || Li > INT32_MAX) {
		log_error("observation message packing: L integer overflow (%f)", L);
		return -1;
	}

	double Lf = -L - Li;

	msg->L.i = (s32) Li;
	msg->L.f = (u8) (Lf * MSG_OSB_LF_MULTIPLIER);

	s32 snr_fp = lround(snr * MSG_OBS_SNR_MULTIPLIER);
	if (snr < 0 || snr_fp > UINT8_MAX) {
		log_error("observation message packing: SNR integer overflow (%f)", snr);
		return -1;
	}

	msg->cn0 = (u8)snr_fp;

	msg->lock = lock_counter;

	msg->sid = sid_to_sbp(sid);

	return 0;
}
void send_message(int n, int nmax){
	s8 wrt = 1;
	int nr_obs = n;
	if(n > nmax)
		nr_obs = 11;
	printf("nr_obs: %d\n", nr_obs);
	if(nr_obs > 0){
		while(wrt != 0){
			switch(nr_obs){
				case 1 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload1), (u8*)&payload1,
										   &piksi_port_write);
					break;
				case 2 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload2), (u8*)&payload2,
										   &piksi_port_write);
					break;
				case 3 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload3), (u8*)&payload3,
										   &piksi_port_write);
					break;
				case 4 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload4), (u8*)&payload4,
										   &piksi_port_write);
					break;
				case 5 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload5), (u8*)&payload5,
										   &piksi_port_write);
					break;
				case 6 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload6), (u8*)&payload6,
										   &piksi_port_write);
					break;
				case 7 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload7), (u8*)&payload7,
										   &piksi_port_write);
					break;
				case 8 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload8), (u8*)&payload8,
										   &piksi_port_write);
					break;
				case 9 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload9), (u8*)&payload9,
										   &piksi_port_write);
					break;
				case 10 :
					wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload10), (u8*)&payload10,
										   &piksi_port_write);
					break;
				case 11 :
					printf("test");
				    wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42, sizeof(payload11), (u8*)&payload11,
										   &piksi_port_write);
				    printf("wrt: %d\n", wrt);
			        break;

			}

	    }
	}
	wrt = 1;
}

static void send_observations(u8 n, navigation_measurement_t *m,
                              const gps_time_t *t)
{
  static u8 buff[256];

  /* Upper limit set by SBP framing size, preventing underflow */
  u16 msg_payload_size = MAX(
      MIN(msg_obs_max_size, SBP_FRAMING_MAX_PAYLOAD_SIZE),
      sizeof(observation_header_t)
    ) - sizeof(observation_header_t);

  /* Lower limit set by sending at least 1 observation */
  msg_payload_size = MAX(msg_payload_size, sizeof(packed_obs_content_t));

  /* Round down the number of observations per message */
  u16 obs_in_msg = msg_payload_size / sizeof(packed_obs_content_t);

  /* Round up the number of messages */
  u16 total = MIN((n + obs_in_msg - 1) / obs_in_msg, MSG_OBS_HEADER_MAX_SIZE);

  u8 obs_i = 0;
  for (u8 count = 0; count < total; count++) {

    u8 curr_n = MIN(n - obs_i, obs_in_msg);
    pack_obs_header(t, total, count, (observation_header_t*) buff);
    packed_obs_content_t *obs = (packed_obs_content_t *)&buff[sizeof(observation_header_t)];

    for (u8 i = 0; i < curr_n; i++, obs_i++) {
      if (pack_obs_content(m[obs_i].raw_pseudorange,
            m[obs_i].raw_carrier_phase,
            m[obs_i].snr,
            m[obs_i].lock_counter,
            m[obs_i].sid,
            &obs[i]) < 0) {
        /* Error packing this observation, skip it. */
        i--;
        curr_n--;
      }
    }
    s8 wrt = 1;
    while(wrt != 0){
    	wrt = sbp_send_message(&sbp_state, SBP_MSG_OBS , 0x42,sizeof(observation_header_t) + curr_n*sizeof(packed_obs_content_t), buff, &piksi_port_write);
    }
  }
}

void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
	std::string solution;
	pos_llh = *(msg_pos_llh_t *)msg;
	switch(pos_llh.flags & 255){
		case 0 :
			solution = "SPP";
			break;
		case 1 :
			solution = "Fixed RTK";
			break;
		case 2 :
			solution = "Float RTK";
			break;
	}
	std::cout << solution << std::endl;
	std::ofstream file;
	file.open("CorsDataBult.csv", std::ofstream::out | std::ofstream::app);
	file << solution << "," <<pos_llh.lat << "," << pos_llh.lon << "," << pos_llh.height << "," << (int)pos_llh.n_sats << "," << pos_llh.tow << "\n";
	file.close();
	printf("lat: %f\n", pos_llh.lat);
	printf("lon: %f\n", pos_llh.lon);
	printf("sats: %u\n", pos_llh.n_sats);
}

void sbp_base_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context){
	base_ecef = *(msg_baseline_ecef_t *)msg;
	switch(base_ecef.flags & 255){
		case 0 :
			printf("Float RTK\n");
			break;
		case 1 :
			printf("Fixed RTK\n");
			break;
	}
	printf("basex: %d\n", base_ecef.x);
	printf("basey: %d\n", base_ecef.y);
	printf("basez: %d\n", base_ecef.z);

}
void sbp_pos_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context){
	pos_ecef = *(msg_pos_ecef_t *)msg;
	printf("roverx: %f\n", pos_ecef.x);
	printf("rovery: %f\n", pos_ecef.y);
	printf("roverz: %f\n", pos_ecef.z);

}

void sbp_log_callback(u16 sender_id, u8 len, u8 msg[], void *context){
	printf("len: %u\n", len);
	char * str = (char*)malloc(len);
	memcpy(str, &msg[1], len - 1);
	str[len - 1] = 0;
	std::string text (str);
	std::cout << text << std::endl;
	if(text == "message received"){
		printf("message received\n");
	}
	if(text == "sequence verified"){
		printf("sequence verified\n");
	}
}


void sbp_setup(void){
	sbp_state_init(&sbp_state);
	sbp_register_callback(&sbp_state, SBP_MSG_POS_LLH, &sbp_pos_llh_callback,
		                  NULL, &pos_llh_node);
	sbp_register_callback(&sbp_state, SBP_MSG_BASELINE_ECEF, &sbp_base_ecef_callback,
			              NULL, &base_ecef_node);
	sbp_register_callback(&sbp_state, SBP_MSG_POS_ECEF, &sbp_pos_ecef_callback,
				          NULL, &pos_ecef_node);
	sbp_register_callback(&sbp_state, SBP_MSG_LOG, &sbp_log_callback,
					      NULL, &log_node);
}
