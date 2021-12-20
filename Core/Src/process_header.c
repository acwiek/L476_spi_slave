/*
 * bytes_parsing_state_machine.c
 *
 *  Created on: Dec 19, 2021
 *      Author: acwiek
 */


#include <stdint.h>
#include <stddef.h>
#include "process_header.h"

typedef int (*event_cb_t)(uint8_t *data, size_t length);

// transfer callback
event_cb_t cb;

uint8_t header_it = 0;
uint8_t header[4] = { };

struct cmd tpm_command;

enum state state_machine = STATE_IDLE;


void register_transmit_callback(event_cb_t input) {
  cb = input;
}

struct cmd *get_command() {
  return &tpm_command;
}

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
    header[header_it] = val;
    header_it = header_it + 1;
    if (header_it > (4 - 1)) {
      header_it = 0;
      uint8_t *header = get_header();

      if (header[0] > 0x7F) {
        tpm_command.rw = READ_REQ;
        tpm_command.size = (header[0] - 0x80) + 1;
      } else {
        tpm_command.rw = WRITE_REQ;
        tpm_command.size = header[0] + 1;
      }

      tpm_command.addr = (header[1] << 16L) + (header[2] << 8L) + header[3];

      state_machine = STATE_FLOW_CONTROL;
    }
    break;

  case STATE_FLOW_CONTROL: {
    uint8_t data = 0;

    val = cb(&data, 1);
    if (val == -1) {
      printf("val == -1\n");
    }

    data = 1;
    val = cb(&data, 1);
    if (val == -1) {
      printf("val == -1\n");
    }

    set_state(STATE_TRANSMIT);
  }
    break;

  case STATE_TRANSMIT:
    ;

    break;

  }

  return state_machine;
}
