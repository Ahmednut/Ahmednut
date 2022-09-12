# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

set(NUTAQ_SDKROOT "/opt/Nutaq/bas/sdk")

list(APPEND nutaq_includes
    ${NUTAQ_SDKROOT}/embedded/lib/adac250_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/adc5000_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/aurora_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/carrier_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/common/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/drivers_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/FMCEEprom_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/lvds_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/lvdsxinxout_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/mi125_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/mi250_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/mo1000_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/perseus601x_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/ppssync_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/radio420_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/sysmon_lib/inc/
    ${NUTAQ_SDKROOT}/embedded/lib/timestamp_lib/inc/
    ${NUTAQ_SDKROOT}/host/api/eapi/inc/
    ${NUTAQ_SDKROOT}/host/api/record_playback/inc/
    ${NUTAQ_SDKROOT}/host/api/rtdex/inc/
    ${NUTAQ_SDKROOT}/host/api/utils/inc/
)

include_directories(${Boost_INCLUDE_DIR} ${nutaq_includes} )
link_directories(${Boost_LIBRARY_DIRS} ${NUTAQ_SDKROOT}/host/lib64/)

list(APPEND nutaq_sources
    rtdex.cc
    rtdex_sink_impl.cc
    rtdex_source_impl.cc
    radio420_tx_impl.cc
    radio420_rx_impl.cc
    adac250_impl.cc
    carrier.cc
    carrier_perseus_impl.cc
    carrier_zedboard_impl.cc
    nutaq_api.cc
    nutaq_cb.cc
    delay_counter.cc
    ${NUTAQ_SDKROOT}/host/api/utils/src/mac_utils.c
    custom_register_impl.cc
    generic_register_impl.cc
    playback_impl.cc
)

add_library(gnuradio-nutaq SHARED ${nutaq_sources})
target_link_libraries(gnuradio-nutaq ${Boost_LIBRARIES} ${GNURADIO_RUNTIME_LIBRARIES} eapi.a rtdex.a librecplay.a pcap rt)
set_target_properties(gnuradio-nutaq PROPERTIES DEFINE_SYMBOL "gnuradio_nutaq_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-nutaq
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_nutaq_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_nutaq.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_nutaq.cc
)

add_executable(test-nutaq ${test_nutaq_sources})

target_link_libraries(
  test-nutaq
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-nutaq
)

GR_ADD_TEST(test_nutaq test-nutaq)
