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
        int id;
        string name;
        double price;
        stack<Reviews> reviews;
        string ImageURL;
        string description;
        string color;

        public:
        Products(int id, string name, double price, stack<Reviews> reviews, string ImageURL, string description, string color)
        :name(name), price(price), reviews(reviews), ImageURL(ImageURL), description(description), color(color){}

        Products(){
                this->id = NULL;
                this->name = " ";
                this->color = " ";
                this->description = " ";
                this->ImageURL = " ";
                this->price = 0;       
        }

        Products(const Products& other){
                this->id = other.id;
                 this->name = other.name;
                 this->price  = other.price;
                 this->reviews = other.reviews;
                 this->ImageURL= other.ImageURL;
                 this->color = other.color;
                 this->description = other.description;
        }

        Products& operator =(const Products& other){
                if(this != &other){
                        this->id = other.id;   
                        this->name = other.name;
                        this->price  = other.price;
                        this->reviews = other.reviews;
                        this->ImageURL= other.ImageURL;
                        this->color = other.color;
                        this->description = other.description;
                }
        }
        string getname(){ return this->name;}
        int getprice(){return this->price;}
        string getdescription(){return this->description;}
        string getcolor(){ return this->color;}
        string getImageURL(){return this->ImageURL;}
        stack<Reviews> getReivews(){ return this->reviews;}
        int getID(){return this->id;}

        void setname(string newname){  this->name = newname;}
        void setprice(int newprice){ this->price = newprice;}
        void setdescription(string newdesc){ this->description = newdesc;}
        void setcolor(string newcolor){  this->color = newcolor;}
        void setImageURL(string newIMAGE){ this->ImageURL = newIMAGE;}
        void setReivews( stack<Reviews> newreviews){  this->reviews = newreviews;}


        ~Products(){
                delete[] &this->reviews;
        }
};

#endif


