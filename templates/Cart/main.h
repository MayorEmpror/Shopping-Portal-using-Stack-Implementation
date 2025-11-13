#ifndef CART_MAIN_H
#define CART_MAIN_H

#include <iostream>
#include <list>
#include <stack>
#include "../products/main.h"
#include "../coupons/main.h"

using namespace std;

class CART {
private:
    stack<Products> ItemsToBuy;
    list<Coupons> AvailableCouponsList;
    double totalprice;

public:
    // Default constructor
    CART() {
        totalprice = 0.0;
    }

    // Parameterized constructor
    CART(stack<Products> items, list<Coupons> coupons, double price) {
        ItemsToBuy = items;
        AvailableCouponsList = coupons;
        totalprice = price;
    }

    // Copy constructor
    CART(const CART &other) {
        ItemsToBuy = other.ItemsToBuy;
        AvailableCouponsList = other.AvailableCouponsList;
        totalprice = other.totalprice;
    }

    // Copy assignment operator
    CART &operator=(const CART &other) {
        if (this != &other) {
            ItemsToBuy = other.ItemsToBuy;
            AvailableCouponsList = other.AvailableCouponsList;
            totalprice = other.totalprice;
        }
        return *this;
    }

    // Getters
    stack<Products> getItemsToBuy() const { return ItemsToBuy; }
    list<Coupons> getAvailableCoupons() const { return AvailableCouponsList; }
    double getTotalPrice() const { return totalprice; }

    // Setters
    void setItemsToBuy(stack<Products> newItems) { ItemsToBuy = newItems; }
    void setAvailableCoupons(list<Coupons> newCoupons) { AvailableCouponsList = newCoupons; }
    void setTotalPrice(double newPrice) { totalprice = newPrice; }

    // Add product to cart
    void addToCart( Products &newProduct) {
        ItemsToBuy.push(newProduct);
        updateBill(newProduct.getprice()); // assuming Products has getPrice()
    }

    // Remove product from cart (LIFO)
    void removeFromCart() {
        if (!ItemsToBuy.empty()) {
            Products removed = ItemsToBuy.top();
            ItemsToBuy.pop();
            updateBill(-removed.getprice()); // subtract price
        }
    }

    // Apply coupons (stub)
    void applyCoupons(const list<Coupons> &couponsToApply) {
        // logic for applying coupons
    }

    // Match coupon availability (stub)
    bool matchCouponAvailability(const list<Coupons> &couponsToApply) {
        // return true if all coupons in couponsToApply exist in AvailableCouponsList
        return true; // placeholder
    }

    // Update bill
    void updateBill(double amount) {
        totalprice += amount;
        if (totalprice < 0)
            totalprice = 0;
    }

    // Destructor
    ~CART() {}
};

#endif
