#include <cmath>
#include <iomanip>

#include "GaussFilter.h"

GaussFilter::GaussFilter(sc_module_name n)
    : sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_filter);

  t_skt.register_b_transport(this, &GaussFilter::blocking_transport);
}

GaussFilter::~GaussFilter() = default;

void GaussFilter::do_filter() {
  row_sent = 0;
  while (true) {
    row_sent++;
    for (int i = 0; i < (image_size + 2); i++){
      red[(row_sent-1)%3][i] = i_r.read();
      green[(row_sent-1)%3][i] = i_g.read();
      blue[(row_sent-1)%3][i] = i_b.read();
    }
    if (row_sent < 3){
      wait(50 * CLOCK_PERIOD, SC_NS);
      //printf("row sent: %d \n", row_sent);
      continue;
    }
    for (int w = 1; w < image_size+1; w++){
      for (unsigned int i = 0; i < MASK_N; ++i) {
        o_red = 0;
        o_green = 0;
        o_blue = 0;
      }
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          for (unsigned int i = 0; i != MASK_N; ++i) {
            o_red += red[(row_sent-(MASK_Y-v))%3][w + (u-1)] * mask[i][u][v] / mult_factor;
            o_green += green[(row_sent-(MASK_Y-v))%3][w + (u-1)] * mask[i][u][v] / mult_factor;
            o_blue += blue[(row_sent-(MASK_Y-v))%3][w + (u-1)] * mask[i][u][v] / mult_factor;
          }
        }
      }    
      int result = ((((o_blue<<8)+ o_green)<<8) + o_red);
      o_result.write(result);
    }
  }
}

void GaussFilter::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr -= base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case GAUSS_FILTER_RESULT_ADDR:
      buffer.uint = o_result.read();
      break;
    case GAUSS_FILTER_CHECK_ADDR:
      buffer.uint = o_result.num_available();
    break;
    default:
      std::cerr << "Error! GaussFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case GAUSS_FILTER_R_ADDR:
      if (mask_ptr[0] == 0xff) {
        i_r.write(data_ptr[0]);
      }
      if (mask_ptr[1] == 0xff) {
        i_g.write(data_ptr[1]);
      }
      if (mask_ptr[2] == 0xff) {
        i_b.write(data_ptr[2]);
      }
      break;
    default:
      std::cerr << "Error! GaussFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    break;
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}