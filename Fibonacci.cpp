#include <iostream>
template<int n>
class Fibonacci{
    public:
    enum{f1=Fibonacci<n-1>::f2,f2=Fibonacci<n-1>::f1+Fibonacci<n-1>::f2};
};
template<>
class Fibonacci<2>{
    public:
    enum{f1=1,f2=1};
};
template<>
class Fibonacci<1>{
    public:
    enum{f1=1,f2=1};
};
int main(){
    std::cout<<Fibonacci<40>::f2<<std::endl;
    return 1;
}