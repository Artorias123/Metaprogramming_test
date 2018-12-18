#include <iostream>
#define N 40
int Fibonacci(int n){
    if(n<3) return 1;
    return (Fibonacci(n-1)+Fibonacci(n-2));
}
int main(){
    std::cout<<Fibonacci(40)<<std::endl;
    return 1;
}