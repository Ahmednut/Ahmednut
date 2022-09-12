#include "nutaq/nutaq_cb.h"
#include "nutaq_api.h"

namespace gr {
    namespace nutaq {

        nutaq_cb::nutaq_cb()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            priority = 0;
            api->register_callback(this);
        }

        nutaq_cb::nutaq_cb(int pr)
        {
            nutaq_api * api = nutaq_api::get_instance();

            priority = pr;
            api->register_callback(this);
        }

        void nutaq_cb::set_priority(int pr)
        {
            priority = pr;
        }

        int nutaq_cb::get_priority()
        {
            return priority;
        }
        
    } //namespace nutaq
} // namespace gr
