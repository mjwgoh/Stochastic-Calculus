#ifndef MAIN_CPP_OPTION_PRICING_H
#define MAIN_CPP_OPTION_PRICING_H
#include "BS_Pricing.h"
#include "Binomial_Pricing.h"

class Price_Options :
        virtual public Vanilla_BS_Pricing,
        virtual public Vanilla_Binomial_Pricing,
        virtual public Grid_BS_Pricing {

};

#endif //MAIN_CPP_OPTION_PRICING_H
