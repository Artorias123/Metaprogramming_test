#ifndef VALVECTOR
#define VALVECTOR
#include<iostream>
#include<valarray>
struct Plus{
	//inline static double Apply(double a,double b){return a+b;}
};
struct Minus{
	//inline static double Apply(double a,double b){return a-b;}
};
struct Scale{
	//inline static double Apply(double a,double b){return a*b;}
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------

//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
static int nt=0;
class valvector{
	double* vec;
	unsigned num;
	public:
	typedef double value_type;
	valvector():num(0){
		vec=nullptr;
		nt++;std::cout<<"构造"<<nt<<std::endl;
	}
	valvector(int n):num(n){
		vec=(double*)malloc(sizeof(double)*num);
	}
	valvector(int n,double x):num(n){
		vec=(double*)malloc(sizeof(double)*num);
		for(int i=0;i<num;i++) vec[i]=x;
		nt++;std::cout<<"构造"<<nt<<std::endl;
	}
	valvector(const valvector& x):num(x.num){
		vec=(double*)malloc(sizeof(double)*num);
		for(int i=0;i<x.num;i++) vec[i]=x[i];nt++;
		std::cout<<"构造"<<nt<<std::endl;
	}
	~valvector(){free(vec);}
	const unsigned& size() const {return num;}
	inline const double& operator[](unsigned i) const{return vec[i];}
	inline const double& operator[](unsigned i) {return vec[i];}
	valvector& operator=(const valvector& x){
		if(x.num>num){
			free(vec);
			vec=(double*)malloc(sizeof(double)*x.num);;
		}
		num=x.num;
		double *tmp=vec,*end=x.vec+num*sizeof(double);
		for(double* p=x.vec;p!=end;p++) *(tmp++)=*p;
		return *this;
	}
	valvector& operator=(valvector&& x){
		free(vec);
		vec=x.vec;
		num=x.num;
		x.vec=nullptr;
		return *this;
	}
	valvector& operator=(double x){
		if(!num) return *this;
		double* p=vec;
		for(int i=0;i<num;i++) *(p++)=x;
		return *this;
	}
	template <class ExprA>
	valvector& operator=(const ExprA& x){
		if(x.num>num){
			free(vec);
			vec=(double*)malloc(sizeof(double)*x.num);;
			num=x.num;
		}
		double* p=vec;
		for(int i=0;i<x.num;i++) *(p++)=x[i];
		return *this;
	}
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <class ExprA,class ExprB,class Op>
struct BinExpr{
	const unsigned num;
	BinExpr(const ExprA& A,const ExprB& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return Op::Apply(_A[i],_B[i]);}
	inline unsigned size() const {return num;}
	private:
	ExprA _A;
	ExprB _B;
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <>
struct BinExpr<double,valvector,Scale>{
	const unsigned num;
	BinExpr(double k,const valvector& B):_k(k),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _k*_B[i];}
	inline unsigned size() const {return num;}
	private:
	const valvector& _B;
	double _k;
};
template <>
struct BinExpr<valvector,valvector,Plus>{
	const unsigned num;
	BinExpr(const valvector& A,const valvector& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]+_B[i];}
	inline unsigned size() const {return num;}
	private:
	const valvector& _A;
	const valvector& _B;
};
template <>
struct BinExpr<valvector,valvector,Minus>{
	const unsigned num;
	BinExpr(const valvector& A,const valvector& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]-_B[i];}
	inline unsigned size() const {return num;}
	private:
	const valvector& _A;
	const valvector& _B;
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <class ExprB>
struct BinExpr<double,ExprB,Scale>{
	const unsigned num;
	BinExpr(double k,const ExprB& B):_k(k),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _k*_B[i];}
	inline unsigned size() const {return num;}
	private:
	ExprB _B;
	double _k;
};
template <class ExprB>
struct BinExpr<valvector,ExprB,Plus>{
	const unsigned num;
	BinExpr(const valvector& A,const ExprB& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]+_B[i];}
	inline unsigned size() const {return num;}
	private:
	const valvector& _A;
	ExprB _B;
};
template <class ExprB>
struct BinExpr<valvector,ExprB,Minus>{
	const unsigned num;
	BinExpr(const valvector& A,const ExprB& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]-_B[i];}
	inline unsigned size() const {return num;}
	private:
	const valvector& _A;
	ExprB _B;
};
template <class ExprA>
struct BinExpr<ExprA,valvector,Plus>{
	const unsigned num;
	BinExpr(const ExprA& A,const valvector& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]+_B[i];}
	inline unsigned size() const {return num;}
	private:
	ExprA _A;
	const valvector& _B;
};
template <class ExprA>
struct BinExpr<ExprA,valvector,Minus>{
	const unsigned num;
	BinExpr(const ExprA& A,const valvector& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]-_B[i];}
	inline unsigned size() const {return num;}
	private:
	ExprA _A;
	const valvector& _B;
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <class ExprA,class ExprB>
struct BinExpr<ExprA,ExprB,Plus>{
	const unsigned num;
	BinExpr(const ExprA& A,const ExprB& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]+_B[i];}
	inline unsigned size() const {return num;}
	private:
	ExprA _A;
	ExprB _B;
};
template <class ExprA,class ExprB>
struct BinExpr<ExprA,ExprB,Minus>{
	const unsigned num;
	BinExpr(const ExprA& A,const ExprB& B):_A(A),_B(B),num(B.size()){};
	inline const double operator[](unsigned i) const{return _A[i]-_B[i];}
	inline unsigned size() const {return num;}
	private:
	ExprA _A;
	ExprB _B;
};
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
inline BinExpr<double,valvector,Scale> operator*(double k,const valvector& B){
	typedef BinExpr<double,valvector,Scale> Exprtmp;
	return (Exprtmp(k,B));
}
inline BinExpr<valvector,valvector,Plus> operator+(const valvector &A,const valvector &B){
	typedef BinExpr<valvector,valvector,Plus> Exprtmp;
	return Exprtmp(A,B);
}
inline BinExpr<valvector,valvector,Minus> operator-(const valvector &A,const valvector &B){
	typedef BinExpr<valvector,valvector,Minus> Exprtmp;
	return (Exprtmp(A,B));
}
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <class ExprA>
inline BinExpr<ExprA,valvector,Plus> operator+(const ExprA &A,const valvector &B){
	typedef BinExpr<ExprA,valvector,Plus> Exprtmp;
	return (Exprtmp(A,B));
}
template <class ExprA>
inline BinExpr<ExprA,valvector,Minus> operator-(const ExprA &A,const valvector &B){
	typedef BinExpr<ExprA,valvector,Minus> Exprtmp;
	return (Exprtmp(A,B));
}
template <class ExprB>
inline BinExpr<valvector,ExprB,Plus> operator+(const valvector &A,const ExprB &B){
	typedef BinExpr<valvector,ExprB,Plus> Exprtmp;
	return (Exprtmp(A,B));
}
template <class ExprB>
inline BinExpr<valvector,ExprB,Minus> operator-(const valvector &A,const ExprB &B){
	typedef BinExpr<valvector,ExprB,Minus> Exprtmp;
	return (Exprtmp(A,B));
}
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
template <class ExprB>
inline BinExpr<double,ExprB,Scale> operator*(double k,const ExprB &B){
	typedef BinExpr<double,ExprB,Scale> Exprtmp;
	return Exprtmp(k,B);
}
template <class ExprA,class ExprB>
inline BinExpr<ExprA,ExprB,Plus> operator+(const ExprA &A,const ExprB &B){
	typedef BinExpr<ExprA,ExprB,Plus> Exprtmp;
	return (Exprtmp(A,B));
}

template <class ExprA,class ExprB>
inline BinExpr<ExprA,ExprB,Minus> operator-(const ExprA &A,const ExprB &B){
	typedef BinExpr<ExprA,ExprB,Minus> Exprtmp;
	return (Exprtmp(A,B));
}
//-------1---------2---------3---------4---------5---------6---------7---------8---------9---------
class vect{
	public:
	double *x;
	int num;
	vect(int n){
		num=n;
		x=(double*)malloc(sizeof(double)*n);
		nt++;std::cout<<"构造"<<nt<<std::endl;
	}
	vect(int n,double k){
		num=n;
		x=(double*)malloc(sizeof(double)*n);
		for(int i=0;i<n;i++) x[i]=k;
		nt++;std::cout<<"构造"<<nt<<std::endl;
	}
	vect(const vect& y){
		x=(double*)malloc(sizeof(double)*y.num);
		for(int i=0;i<y.num;i++) x[i]=y.x[i];
		num=y.num;
		nt++;std::cout<<"构造"<<nt<<std::endl;
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
	vect operator-(const vect&y){
		vect tmp(num);
		for(int i=0;i<y.num;i++) tmp.x[i]=x[i]-y.x[i];
		return tmp;  
	}
	friend vect operator*(double k,const vect&y){
		vect tmp(y.num);
		for(int i=0;i<y.num;i++) tmp.x[i]=k*y.x[i];
		return tmp;
	}
	const double& operator[](unsigned i){return x[i];}
};
#define N 5000000
int main(int argc,char* argv[]){
	
	valvector x(N,1),y(N,2),z(N,3);
	z=x+y+z;
	std::cout<<z[N-1]<<std::endl;
	
	/*
	vect x(N,1),y(N,2),z(N,3);
	z=x+y+z;
	std::cout<<z[N-1]<<std::endl;
	*/
	/*
	std::valarray<double> x(N),y(N),z(N);
	x=1;
	y=2;
	z=3;
	z=x+y+z;
	std::cout<<z[N-1]<<std::endl;
	*/
	return 0;
}
#endif
