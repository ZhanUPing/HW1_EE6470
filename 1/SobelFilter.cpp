#include <cmath>
#include <vector>
#include "SobelFilter.h"
#include <algorithm>
#include <iostream>

using namespace std;
SobelFilter::SobelFilter(sc_module_name n) : sc_module(n)
{
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

/*// sobel mask
const int mask[MASK_N][MASK_X][MASK_Y] = {{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},

                                          {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}};
*/
void SobelFilter::do_filter()
{

  while (true)
  {
    int temp_r[9] = {0};
    int temp_g[9] = {0};
    int temp_b[9] = {0};

    avg_r = 0;
    avg_g = 0;
    avg_b = 0;

    for (unsigned int v = 0; v < MASK_Y; ++v)
    {
      for (unsigned int u = 0; u < MASK_X; ++u)
      {
        temp_r[u + 3 * v] = i_r.read();
        temp_g[u + 3 * v] = i_g.read();
        temp_b[u + 3 * v] = i_b.read();

        if (u == 1 && v == 1)
        {
          avg_r = avg_r;
          avg_g = avg_g;
          avg_b = avg_b;
        }
        else
        {
          avg_r = temp_r[u + 3 * v] + avg_r;
          avg_g = temp_g[u + 3 * v] + avg_g;
          avg_b = temp_b[u + 3 * v] + avg_b;
        }
      }
    }

    sort(begin(temp_r), begin(temp_r) + 9);
    sort(begin(temp_g), begin(temp_g) + 9);
    sort(begin(temp_b), begin(temp_b) + 9);

    avg_r = (avg_r + 2 * temp_r[4]) / 10;
    avg_g = (avg_g + 2 * temp_g[4]) / 10;
    avg_b = (avg_b + 2 * temp_b[4]) / 10;

    o_avg_r.write(avg_r);
    o_avg_g.write(avg_g);
    o_avg_b.write(avg_b);
    wait(10); // emulate module delay
  }
}
