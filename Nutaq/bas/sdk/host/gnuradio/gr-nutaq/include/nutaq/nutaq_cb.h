#ifndef NUTAQ_CALLBACK_H__
#define NUTAQ_CALLBACK_H__

namespace gr {
    namespace nutaq {

        class nutaq_cb
        {
        public:
            nutaq_cb();

            nutaq_cb(int pr);
            
            virtual int callback()=0;
            
            void set_priority(int pr);

            int get_priority();

            static bool cb_compare(nutaq_cb * i, nutaq_cb * j)
            {
                return i->get_priority() < j->get_priority();
            }
            
        private:
            int priority;
        };
    } //namespace nutaq
} // namespace gr

#endif
