#include <iostream>
template<int n>
class factorial{
    public:
    enum{res=n*factorial<n-1>::res};
};
template<>
class factorial<1>{
    public:
    enum{res=1};
};
int main(){
    std::cout<<factorial<10>::res<<std::endl;
    return 1;
}
