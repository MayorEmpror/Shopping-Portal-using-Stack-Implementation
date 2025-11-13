#ifndef PRODUCTS_MAIN_H
#define PRODUCTS_MAIN_H

#include "../coupons/main.h"
#include "../review/main.h"
#include "../../components/stacks/ImplementationStack.h"
#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Products{
        private:
        string name;
        double price;
        stack<Reviews> reviews;
        
};

#endif


