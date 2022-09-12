#ifndef NUTAQ_CARRIER_CLIENT__
#define NUTAQ_CARRIER_CLIENT__

#include <string>

namespace gr {
    namespace nutaq {

        class carrier_client
        {
        private:
            std::string d_target_id;
        public:
            carrier_client()
            {}

            carrier_client(std::string target_id)
            {
                d_target_id = target_id;
            }

            void set_target_id(std::string target_id)
            {
                d_target_id = target_id;
            }

            std::string get_target_id()
            {
                return d_target_id;
            }
        };

    } // namespace nutaq
} // namespace gr


#endif
