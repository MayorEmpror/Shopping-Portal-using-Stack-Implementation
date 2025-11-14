#include <iostream>
#include <string>
#include "../Cart/main.h"
#include "../account/main.h"

using namespace std;

// admin class implementation


class Admin{
  private:
  string  username;
  int password;
  string type;
  CART shoppingcart; 
  Account adminaccount;

  Admin(string username, int password, string type, CART shoppingcart, Account adminaccount):
        username(username), password(password), type(type), shoppingcart(shoppingcart), adminaccount(adminaccount){}

        string getusername(){ return this->username;}
        string type(){ return this->type;}
        int password(){ return this->password;}

        CART getshoppingcart(){return this->shoppingcart; }
        Account getAccount(){ return this->adminaccount; }

        void setname(string newname ){ this->username = newname;}
        void settype(string newtype){ this->type = newtype;}
        void changepassword(int newpass){ this->password = newpass;}
        void setshopping( CART newshopping){ this->shoppingcart = newshopping;}
        void setaccount(Account newAcc){ this->adminaccount = newAcc;}
  ~Admin(){}
};

int main(){
    return 0;
}