
#include <iostream>


void int_als_variable(int i){
  std::cout <<"int_als_variable:in"<<i<<std::endl;

  i = 1000;
  
  std::cout <<"int_als_variable:out"<<i<<std::endl;
}

void int_als_pointer(int * i){
  std::cout <<"int_als_pointer:in"<<*i<<std::endl;

  *i = 1000;

  std::cout <<"int_als_pointer:out"<<*i<<std::endl;

}

void int_als_reference(int & i){
  std::cout <<"int_als_reference:in"<<i<<std::endl;

  i = 2000;

  std::cout <<"int_als_reference:out"<<i<<std::endl;

}

//using namespace std;
int main(int argc, char ** argv){

  int x;

  x=10;
  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

  int * z;
  z=&x;
  std::cout << "Dereferenzierung von z "<< *z <<std::endl;
  std::cout << "Adresse von z "<< z <<std::endl;

  *z=20;

  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

  std::cout << "Dereferenzierung von z "<<*z<<std::endl;
  std::cout << "Adresse von z "<<z<<std::endl;

  x=30;

  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

  std::cout << "Dereferenzierung von z "<<*z<<std::endl;
  std::cout << "Adresse von z "<<z<<std::endl;

  int y=100;

  z=&y;

  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

  std::cout << "Wert von y "<<y<<std::endl;
  std::cout << "Adresse von y "<<&y<<std::endl;

  std::cout << "Dereferenzierung von z "<<*z<<std::endl;
  std::cout << "Adresse von z "<<z<<std::endl;

  y=x;

  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

  std::cout << "Wert von y "<<y<<std::endl;
  std::cout << "Adresse von y "<<&y<<std::endl;

  std::cout << "Dereferenzierung von z "<<*z<<std::endl;
  std::cout << "Adresse von z "<<z<<std::endl;

  int_als_variable(x);
  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;


  int_als_pointer(&x);
  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

  int_als_reference(x);
  std::cout << "Wert von x "<<x<<std::endl;
  std::cout << "Adresse von x "<<&x<<std::endl;

}



