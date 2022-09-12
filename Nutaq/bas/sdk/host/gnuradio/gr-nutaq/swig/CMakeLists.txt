# Copyright 2011 Free Software Foundation, Inc.
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

include_directories(${Boost_INCLUDE_DIR} ${nutaq_includes})
link_directories(${Boost_LIBRARY_DIRS} ${NUTAQ_SDKROOT}/host/lib64/)

########################################################################
# Include swig generation macros
########################################################################
find_package(SWIG)
find_package(PythonLibs)
if(NOT SWIG_FOUND OR NOT PYTHONLIBS_FOUND)
    return()
endif()
include(GrSwig)
include(GrPython)

########################################################################
# Setup swig generation
########################################################################
foreach(incdir ${GNURADIO_RUNTIME_INCLUDE_DIRS})
    list(APPEND GR_SWIG_INCLUDE_DIRS ${incdir}/gnuradio/swig)
endforeach(incdir)

set(GR_SWIG_LIBRARIES gnuradio-nutaq)
set(GR_SWIG_DOC_FILE ${CMAKE_CURRENT_BINARY_DIR}/nutaq_swig_doc.i)
set(GR_SWIG_DOC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include)

GR_SWIG_MAKE(nutaq_swig nutaq_swig.i)

########################################################################
# Install the build swig module
########################################################################
GR_SWIG_INSTALL(TARGETS nutaq_swig DESTINATION ${GR_PYTHON_DIR}/nutaq)

########################################################################
# Install swig .i files for development
########################################################################
install(
    FILES
    nutaq_swig.i
    ${CMAKE_CURRENT_BINARY_DIR}/nutaq_swig_doc.i
    DESTINATION ${GR_INCLUDE_DIR}/nutaq/swig
)
