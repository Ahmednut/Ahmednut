/* -*- c++ -*- */
/* 
 * Copyright 2013 Nutaq Innovation inc.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_NUTAQ_CARRIER_PERSEUS_IMPL_H
#define INCLUDED_NUTAQ_CARRIER_PERSEUS_IMPL_H

#include <nutaq/carrier.h>

namespace gr {
    namespace nutaq {

        class carrier_perseus_impl : public carrier
        {
        private:
            /* Nothing to see here, move along */

        public:
            carrier_perseus_impl(std::string id, std::string ip_addr);
            ~carrier_perseus_impl();

            carrier_class get_class();
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_CARRIER_PERSEUS_IMPL_H */

