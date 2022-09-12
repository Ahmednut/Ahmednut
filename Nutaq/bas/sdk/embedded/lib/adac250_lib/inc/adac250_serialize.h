#ifndef __ADAC250_SERIALIZE__
#define __ADAC250_SERIALIZE__
#include "connection_state.h"
#include "adac250_types.h"

void adac250_pll_param_send(connection_state * state, ADAC250_PLL_PARAM * param);
void adac250_pll_param_recv(connection_state * state, ADAC250_PLL_PARAM * param);

#endif
