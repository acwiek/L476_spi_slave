/*
 * process_header.h
 *
 *  Created on: Dec 19, 2021
 *      Author: acwiek
 */

#ifndef SRC_PROCESS_HEADER_H_
#define SRC_PROCESS_HEADER_H_

enum state {
    STATE_IDLE,
    STATE_HEADER,
    STATE_TRANSMIT
};

enum state get_state(uint8_t val);
uint8_t *get_header();



#endif /* SRC_PROCESS_HEADER_H_ */
