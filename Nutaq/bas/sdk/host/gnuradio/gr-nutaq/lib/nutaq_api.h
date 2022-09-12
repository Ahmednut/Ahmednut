#ifndef NUTAQ_API__
#define NUTAQ_API__

/* Wrapper for Nutaq API */

#include <nutaq/nutaq_cb.h>

#include <bas_error.h>

#include "nutaq/carrier.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <pthread.h>

#ifndef MODULE_GNURADIO
#define MODULE_GNURADIO 0xA0
#endif

namespace gr {
    namespace nutaq {

        class nutaq_api 
        {
        private:
            //singleton
            nutaq_api();

        public:

            static nutaq_api * get_instance();

            carrier * get_carrier_byid(std::string targetid);
            std::vector<carrier*> get_carrier_vector_byid(std::string targetid);
    
            void register_carrier(std::string targetid, carrier * c);
            void unregister_carrier(std::string targetid);
            
            unsigned get_datarate(std::string targetid);
            void set_datarate(std::string targetid, unsigned datarate);

            void register_callback(nutaq_cb * cb);
            int do_callback();

            bool is_inited();
            void set_inited(bool init);

            inline void lock()
            {
                pthread_mutex_lock(&mutex);
            }

            inline void unlock()
            {
                pthread_mutex_unlock(&mutex);
            }

            int try_init();
            int wait_for_init();

        private:
            bool d_inited;
            int d_init_result;
            
            std::map<std::string, carrier*> d_carriers;
            std::map<std::string, unsigned> d_datarates;
            
            //threading
            pthread_mutex_t mutex;

            //init callback
            std::vector<nutaq_cb*> d_cb;
            pthread_mutex_t     init_mutex;
            pthread_cond_t      init_cond;

            //Singleton
            static nutaq_api * _singleton;    
        };
    } // namespace nutaq
} // namespace gr

enum gnuradio_error {
    GNURADIO_OK  = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_GNURADIO, 0),
    GNURADIO_CONNECT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 1),  /* Could not connect to carrier board */
    GNURADIO_MACADDR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 2),  /* Could not get MAC address */
    GNURADIO_RESETCORE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 3), /* Could not reset RTDEX FPGA core */
    GNURADIO_FLOWCONTROL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 4), /* Error while enabling flow control */
    GNURADIO_RXFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 5), /* RX has failed */
    GNURADIO_TXFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 6), /* TX has failed */
    GNURADIO_UNSUPPORTEDREV = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 7), /* The revision of Radio420 is not support by this API */
    GNURADIO_RESET = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 8), /* Reset did not complete successfully */
    GNURADIO_SERVERNOTFOUND = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 9), /* Sever not found */
    GNURADIO_INVVALUE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 10), /* One of the arguments has an invalid value */
    GNURADIO_CARRIER_NOT_INIT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_GNURADIO, 11), /* The carrier is not initialized, so this function cannot be executed */
};


#endif

