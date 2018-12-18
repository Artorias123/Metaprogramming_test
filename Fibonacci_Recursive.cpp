#include <iostream>
template<int n>
class Fibonacci{
    public:
    enum{f=Fibonacci<n-1>::f+Fibonacci<n-2>::f};
};
template<>
class Fibonacci<2>{
    public:
    enum{f=1};
};
template<>
class Fibonacci<1>{
    public:
    enum{f=1};
};
int main(){
    std::cout<<Fibonacci<40>::f<<std::endl;
    return 1;
}