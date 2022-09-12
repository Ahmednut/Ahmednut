#ifndef __ADC5000_SERIALIZE__
#define __ADC5000_SERIALIZE__
#include "connection_state.h"
#include "adc5000_defines.h"
#include "ad9517_defines.h"

void adc5000_pll_param_send(connection_state * state, ad9517_param * param);
void adc5000_pll_param_recv(connection_state * state, ad9517_param * param);

#endif
