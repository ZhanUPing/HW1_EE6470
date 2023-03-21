#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;
#include "filter_def.h"

class SobelFilter : public sc_module
{
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;
  sc_fifo_in<unsigned char> i_r;
  sc_fifo_in<unsigned char> i_g;
  sc_fifo_in<unsigned char> i_b;
  sc_fifo_in<unsigned char> i_img_width;

  sc_fifo_out<int> o_avg_r;
  sc_fifo_out<int> o_avg_g;
  sc_fifo_out<int> o_avg_b;

  SC_HAS_PROCESS(SobelFilter);
  SobelFilter(sc_module_name n);
  ~SobelFilter() = default;

private:
  void do_filter();
  int avg_r;
  int avg_g;
  int avg_b;
};
#endif
