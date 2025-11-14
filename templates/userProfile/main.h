#include <iostream>
#include <string>
#include "../Cart/main.h"
#include "../account/main.h"

using namespace std;

// user class implementation


class User{
  private:
  string  username;
  int password;
  string type;
  CART shoppingcart; 
  Account useraccount;

  User(string username, int password, string type, CART shoppingcart, Account useraccount):
        username(username), password(password), type(type), shoppingcart(shoppingcart), useraccount(useraccount){}

        string getusername(){ return this->username;}
        string type(){ return this->type;}
        int password(){ return this->password;}

        CART getshoppingcart(){return this->shoppingcart; }
        Account getAccount(){ return this->useraccount; }

        void setname(string newname ){ this->username = newname;}
        void settype(string newtype){ this->type = newtype;}
        void changepassword(int newpass){ this->password = newpass;}
        void setshopping( CART newshopping){ this->shoppingcart = newshopping;}
        void setaccount(Account newAcc){ this->useraccount = newAcc;}

    

        void AddtoCart(Products newProduct){
            this->shoppingcart.addToCart(newProduct);
            cout<<"added to shopping cart:: funct-ran:userProfile/main.h"<<endl;
        }
        void RemfromCart(Products toRemProd){
            this->shoppingcart.removeFromCart(toRemProd);
        }

        bool Buy(){
            double Totalbalence = this->shoppingcart.getTotalPrice();
            double Funds = this->useraccount.gettotalbalence();
            if(Totalbalence > Funds) {
                cout<<"Transaction Unsuccessfull, insufficient funds"<<endl;
                return false;
            }
            Totalbalence -= Funds;
            this->useraccount.setlifetimespent(Funds);
            this->useraccount.settotalbalence(Totalbalence);
            cout<<"Transaction Successfull, you'er delivery is on its way"<<endl;
            return true;
        }
        
        
  ~User(){}
};

int main(){
    return 0;
}