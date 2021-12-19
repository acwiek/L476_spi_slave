/*
 * bytes_parsing_state_machine.c
 *
 *  Created on: Dec 19, 2021
 *      Author: acwiek
 */


#include <stdint.h>
#include "process_header.h"


uint8_t header_it = 0;
uint8_t header[4] = { };

enum state state_machine = STATE_IDLE;

uint8_t *get_header() {
  return header;
}

void set_state(enum state val) {
  state_machine = val;
}

enum state get_state(uint8_t val) {
  //  phy->iobuf[0] = (in ? 0x80 : 0) | (transfer_len - 1);
  //  phy->iobuf[1] = 0xd4;
  //  phy->iobuf[2] = addr >> 8;
  //  phy->iobuf[3] = addr;
  switch (state_machine) {
  case STATE_IDLE:
    // waiting for the beginning of the header
    // if input value != 0 start filling header buffer
    if (val != 0) {
      header[header_it] = val;
      header_it = header_it + 1;
      state_machine = STATE_HEADER;
    }
    break;

  case STATE_HEADER:
    // first value of the potential header was
    // captured in STATE_IDLE
    header[header_it] = val;
    header_it = header_it + 1;
    if (header_it > (4 - 1)) {
      state_machine = STATE_TRANSMIT;
      header_it = 0;
    }
    break;

  case STATE_TRANSMIT:


    break;

  }
  return state_machine;
}
