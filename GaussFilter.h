#ifndef GAUSS_FILTER_H_
#define GAUSS_FILTER_H_
#include <systemc>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "filter_def.h"

class GaussFilter : public sc_module {
public:
  tlm_utils::simple_target_socket<GaussFilter> t_skt;

  sc_fifo<unsigned char> i_r;
  sc_fifo<unsigned char> i_g;
  sc_fifo<unsigned char> i_b;
  sc_fifo<int> o_result {256};

  SC_HAS_PROCESS(GaussFilter);
  GaussFilter(sc_module_name n);
  ~GaussFilter();

private:
  void do_filter();
  int val[MASK_N];
  int o_red, o_green, o_blue;

  static const int image_size = 256;
  const int mult_factor = 16;
  unsigned char red[3][image_size + 2];
  unsigned char green[3][image_size + 2];
  unsigned char blue[3][image_size + 2];
  int row_sent;

  
  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif
