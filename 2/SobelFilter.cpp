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
  int check = 0;
  int width = i_img_width.read() * 4;
  cout << "Now at width " << width << endl;
  int temp_r[9] = {0};
  int temp_g[9] = {0};
  int temp_b[9] = {0};
  while (true)
  {

    int buffer_r[6] = {0};
    int buffer_g[6] = {0};
    int buffer_b[6] = {0};

    avg_r = 0;
    avg_g = 0;
    avg_b = 0;
    // buffer move

    for (unsigned int v = 0; v < MASK_Y; ++v)
    {
      for (unsigned int u = 0; u < MASK_X; ++u)
      {
        // if check==0 then we read 9pixl
        if (check == 0)
        {
          temp_r[u + 3 * v] = i_r.read();
          temp_g[u + 3 * v] = i_g.read();
          temp_b[u + 3 * v] = i_b.read();
        }
        else // check!=0 then we only read 3pixl
        {
          if (u == 2)
          {
            temp_r[u + 3 * v] = i_r.read();
            temp_g[u + 3 * v] = i_g.read();
            temp_b[u + 3 * v] = i_b.read();
          }
        }
        // if it is center we dont read it still 0
        if (u == 1 && v == 1)
        {
          avg_r = avg_r;
          avg_g = avg_g;
          avg_b = avg_b;
        }
        else // sum
        {
          avg_r = temp_r[u + 3 * v] + avg_r;
          avg_g = temp_g[u + 3 * v] + avg_g;
          avg_b = temp_b[u + 3 * v] + avg_b;
        }
      }
    }
    //  update check
    check = check + 1;
    if (check == width) // if already jump to next row then check==0;
    {
      check = 0;
    }
    if (check != 0)
    {
      buffer_r[0] = temp_r[1];
      buffer_g[0] = temp_g[1];
      buffer_b[0] = temp_b[1];

      buffer_r[1] = temp_r[2];
      buffer_g[1] = temp_g[2];
      buffer_b[1] = temp_b[2];

      buffer_r[2] = temp_r[4];
      buffer_g[2] = temp_g[4];
      buffer_b[2] = temp_b[4];

      buffer_r[3] = temp_r[5];
      buffer_g[3] = temp_g[5];
      buffer_b[3] = temp_b[5];

      buffer_r[4] = temp_r[7];
      buffer_g[4] = temp_g[7];
      buffer_b[4] = temp_b[7];

      buffer_r[5] = temp_r[8];
      buffer_g[5] = temp_g[8];
      buffer_b[5] = temp_b[8];
    }

    sort(begin(temp_r), begin(temp_r) + 9);
    sort(begin(temp_g), begin(temp_g) + 9);
    sort(begin(temp_b), begin(temp_b) + 9);

    avg_r = (avg_r + 2 * temp_r[4]) / 10;
    avg_g = (avg_g + 2 * temp_g[4]) / 10;
    avg_b = (avg_b + 2 * temp_b[4]) / 10;
    if (check != 0)
    {
      temp_r[0] = buffer_r[0];
      temp_g[0] = buffer_g[0];
      temp_b[0] = buffer_b[0];

      temp_r[1] = buffer_r[1];
      temp_g[1] = buffer_g[1];
      temp_b[1] = buffer_b[1];

      temp_r[3] = buffer_r[2];
      temp_g[3] = buffer_g[2];
      temp_b[3] = buffer_b[2];

      temp_r[4] = buffer_r[3];
      temp_g[4] = buffer_g[3];
      temp_b[4] = buffer_b[3];

      temp_r[6] = buffer_r[4];
      temp_g[6] = buffer_g[4];
      temp_b[6] = buffer_b[4];

      temp_r[7] = buffer_r[5];
      temp_g[7] = buffer_g[5];
      temp_b[7] = buffer_b[5];
    }

    o_avg_r.write(avg_r);
    o_avg_g.write(avg_g);
    o_avg_b.write(avg_b);
    wait(10); // emulate module delay
  }
}
