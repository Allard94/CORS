/*
 * sbp.h
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#ifndef SBP_H_
#define SBP_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <libsbp/navigation.h>
#include <libsbp/sbp.h>
#include <libswiftnav/track.h>
#ifdef __cplusplus
}
#endif

extern sbp_state_t sbp_state;

extern msg_pos_llh_t pos_llh;

void send_message(int n, int nmax);
static void send_observations(u8 n, navigation_measurement_t *m, const gps_time_t *t);
void sbp_setup(void);


#endif /* SBP_H_ */
