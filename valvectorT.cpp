#ifndef VALVECTOR
#define VALVECTOR
#include<iostream>
template <typename T>
struct Plus{
	inline static T Apply(T a,T b){return a+b;}
};
template <typename T>
struct Minus{
	inline static T Apply(T a,T b){return a-b;}
};
template <typename T>
struct Scale{
	inline static T Apply(T a,T b){return a*b;}
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <typename T>
struct Const{};
template <class ExprA,typename T>
struct Expr{
	const unsigned num;
	Expr(const ExprA& A):_Expr(A),num(A.num){};
	T operator[](unsigned i) const{return _Expr[i];}
	private:
	const ExprA _Expr;
};
template <typename T>
struct Expr<Const<T>,T>{
	const unsigned num;
	Expr(T x):k(x),num(1){};
	T operator[](unsigned i) const{return k;}
	private:
	const double k;
};
template <class ExprA,class ExprB,class Op,typename T>
struct BinExpr{
	const unsigned num;
	BinExpr(const ExprA& A,const ExprB& B):_A(A),_B(B),num(B.num){};
	T operator[](unsigned i) const{return Op::Apply(_A[i],_B[i]);}
	private:
	const ExprA _A;
	const ExprB _B;
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <typename T>
class valvector{
	T* vec;
	unsigned num;
	public:
	typedef T value_type;
	valvector():num(0){vec=nullptr;}
	valvector(int n):num(n){vec=new T[n];}
	valvector(int n,T x):num(n){vec=new T[n];for(int i=0;i<num;i++) vec[i]=x;}
	valvector(const valvector& x):num(x.num){vec=new T[x.num];for(int i=0;i<x.num;i++) vec[i]=x[i];}
	~valvector(){delete []vec;}
	const unsigned& size() const {return num;}
	const T& operator[](unsigned i) const{return vec[i];}
	T& operator[](unsigned i) {return vec[i];}
	valvector& operator=(const valvector& x){
		if(x.num>num){
			delete []vec;
			vec=new T[x.num];
		}
		num=x.num;
		T* tmp=vec,end=x.vec+num*sizeof(T);
		for(T* p=x.vec;p!=end;p++) *(tmp++)=*p;
		return *this;
	}
	valvector& operator=(valvector&& x){
		delete []vec;
		vec=x.vec;
		num=x.num;
		x.vec=nullptr;
		return *this;
	}
	valvector& operator=(T x){
		if(!num) return *this;
		T* p=vec;
		num=x.num;
		for(int i=0;i<num;i++) *(p++)=x;
	}
	template <class ExprA>
	valvector& operator=(const Expr<ExprA,T>& x){
		if(x.num>num){
			delete []vec;
			vec=new T[x.num];
			num=x.num;
		}
		T* p=vec;
		for(int i=0;i<x.num;i++) *(p++)=x[i];
		return *this;
	}
	template <class ExprB>
	Expr<BinExpr<Expr<valvector,T>,Expr<ExprB,T>,Plus<T>,T>,T> operator+(const Expr<ExprB,T> &B){
		typedef BinExpr<Expr<valvector,T>,Expr<ExprB,T>,Plus<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(Expr<valvector,T>(*this),B));
	}
	template <class ExprB>
	Expr<BinExpr<Expr<valvector,T>,Expr<ExprB,T>,Minus<T>,T>,T> operator-(const Expr<ExprB,T> &B){
		typedef BinExpr<Expr<valvector,T>,Expr<ExprB,T>,Minus<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(Expr<valvector,T>(*this),B));
	}
	//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
	Expr<BinExpr<Expr<valvector,T>,Expr<valvector,T>,Plus<T>,T>,T> operator+(const valvector &B){
		typedef BinExpr<Expr<valvector,T>,Expr<valvector,T>,Plus<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(Expr<valvector,T>(*this),Expr<valvector,T>(B)));
	}
	Expr<BinExpr<Expr<valvector,T>,Expr<valvector,T>,Minus<T>,T>,T> operator-(const valvector &B){
		typedef BinExpr<Expr<valvector,T>,Expr<valvector,T>,Minus<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(Expr<valvector,T>(*this),Expr<valvector,T>(B)));
	}
	//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
	friend Expr<BinExpr<Expr<Const<T>,T>,Expr<valvector,T>,Scale<T>,T>,T> operator*(T k,const valvector &B){
		typedef BinExpr<Expr<Const<T>,T>,Expr<valvector,T>,Scale<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(Expr<Const<T>,T>(k),Expr<valvector,T>(B)));
	}
	//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
	template <class ExprA,class ExprB>
	friend Expr<BinExpr<Expr<ExprA,T>,Expr<ExprB,T>,Plus<T>,T>,T> operator+(const Expr<ExprA,T> &A,const Expr<ExprB,T> &B){
		typedef BinExpr<Expr<ExprA,T>,Expr<ExprB,T>,Plus<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(A,B));
	}
	template <class ExprA,class ExprB>
	friend Expr<BinExpr<Expr<ExprA,T>,Expr<ExprB,T>,Minus<T>,T>,T> operator-(const Expr<ExprA,T> &A,const Expr<ExprB,T> &B){
		typedef BinExpr<Expr<ExprA,T>,Expr<ExprB,T>,Minus<T>,T> Exprtmp;
		return Expr<Exprtmp,T>(Exprtmp(A,B));
	}
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <typename T>
struct Expr<valvector<T>,T>{
	const unsigned num;
	Expr(const valvector<T>& x):_Expr(x),num(x.size()){}
	T operator[](unsigned i) const{return (_Expr)[i];}
	private:
	const valvector<T>& _Expr;
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
class vect{
	public:
	double *x;
	int num;
	vect(int n){
		num=n;
		x=(double*)malloc(sizeof(double)*n);
	}
	vect(int n,double k){
		num=n;
		x=(double*)malloc(sizeof(double)*n);
		for(int i=0;i<n;i++) x[i]=k;
	}
	vect(const vect& y){
		x=(double*)malloc(sizeof(double)*y.num);
		for(int i=0;i<y.num;i++) x[i]=y.x[i];
		num=y.num;
	}
	~vect(){free(x);}
	vect& operator=(vect&& y){
		free(x);
		x=y.x;
		num=y.num;
		y.x=nullptr;
		return *this;
	}
	vect& operator=(const vect& y){
		if(num<y.num){
			free(x);
			x=(double*)malloc(sizeof(double)*y.num);
		}
		for(int i=0;i<y.num;i++) x[i]=y.x[i];
		num=y.num;
		return *this;
	}
	vect operator+(const vect&y){
		vect tmp(num);
		for(int i=0;i<y.num;i++) tmp.x[i]=x[i]+y.x[i];
		return tmp;  
	}
	friend vect operator*(double k,const vect&y){
		vect tmp(y.num);
		for(int i=0;i<y.num;i++) tmp.x[i]=k*y.x[i];
		return tmp;
	}
	const double& operator[](unsigned i){return x[i];}
};
#define N 10000000
int main(int argc,char* argv[]){
	/*
	valvector<double> x(N,1),y(N,2),z(N,3);
	z=2.0*x+3.0*y+4.0*y;
	for(int i=0;i<10;i++) std::cout<<z[i]<<std::endl;
	*/
	
	vect x(N,1),y(N,2),z(N,3);
	z=2.0*x+3.0*y+4.0*y;
	for(int i=0;i<10;i++) std::cout<<z[i]<<std::endl;
	
	return 0;
}
#endif
