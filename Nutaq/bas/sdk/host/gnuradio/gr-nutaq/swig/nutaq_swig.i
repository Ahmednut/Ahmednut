/* -*- c++ -*- */

#define NUTAQ_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "nutaq_swig_doc.i"

%{
#include "nutaq/rtdex.h"
#include "nutaq/rtdex_sink.h"
#include "nutaq/rtdex_source.h"
#include "nutaq/radio420_tx.h"
#include "nutaq/radio420_rx.h"
#include "nutaq/adac250.h"
#include "nutaq/carrier.h"
#include "nutaq/playback.h"
#include "nutaq/custom_register.h"
#include "nutaq/generic_register.h"
%}

%include "nutaq/rtdex.h"

%include "nutaq/rtdex_sink.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, rtdex_sink);
%include "nutaq/rtdex_source.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, rtdex_source);
%include "nutaq/radio420_tx.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, radio420_tx);
%include "nutaq/radio420_rx.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, radio420_rx);
%include "nutaq/adac250.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, adac250);
%include "nutaq/carrier.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, carrier);
%include "nutaq/playback.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, playback);
%include "nutaq/custom_register.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, custom_register);
%include "nutaq/generic_register.h"
GR_SWIG_BLOCK_MAGIC2(nutaq, generic_register);
