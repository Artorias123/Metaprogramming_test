#include<iostream>
using namespace std;
struct Syms{
	inline double operator()(double x){return x;}
};
struct Const{
	private:
	const double k;
	public:
	explicit Const(double x):k(x){};
	inline const double operator()(double x) const{return k;}
};
struct Plus{
	inline static double Apply(double a, double b){return a + b;}
};
struct Minus{
	inline static double Apply(double a,double b){return a-b;}
};
struct Scale{
	inline static double Apply(double a,double b){return a*b;}
};
template <class ExprA, class ExprB, class OP>
struct BinExpr{
	private:
	ExprA _A;
	ExprB _B;
	public:
	BinExpr(const ExprA& a, const ExprB& b) : _A(a), _B(b){}
	inline double operator() (double i) const{return OP::Apply(_A(i), _B(i));}
};
template <class ExprA>
struct Expr{
	Expr(const ExprA& x) : _Expr(x){}
	inline double operator() (double x) const{return _Expr(x);}
	private:
	ExprA _Expr;
};
template <>
struct Expr<Syms>{
	inline double operator() (double x) const{return x;}
};
template <>
struct Expr<Const>{
	private:
	const double k;
	public:
	Expr(double x):k(x){};
	inline double operator()(double x) const{return k;}
};
template <class ExprA,class ExprB>
Expr<BinExpr<Expr<ExprA>,Expr<ExprB>,Plus>> operator +(const Expr<ExprA> &a,Expr<ExprB> &b){
	typedef BinExpr<Expr<ExprA>,Expr<ExprB>,Plus> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(a,b));
};
template <class ExprA,class ExprB>
Expr<BinExpr<Expr<ExprA>,Expr<ExprB>,Minus>> operator -(const Expr<ExprA> &a,const Expr<ExprB> &b){
	typedef BinExpr<Expr<ExprA>,Expr<ExprB>,Minus> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(a,b));
};
template <class ExprA,class ExprB>
Expr<BinExpr<Expr<ExprA>,Expr<ExprB>,Scale>> operator *(const Expr<ExprA> &a,const Expr<ExprB> &b){
	typedef BinExpr<Expr<ExprA>,Expr<ExprB>,Scale> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(a,b));
};
template <class ExprB>
Expr<BinExpr<Expr<Const>,Expr<ExprB>,Scale>> operator *(const double a,const Expr<ExprB> &b){
	typedef BinExpr<Expr<Const>,Expr<ExprB>,Scale> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(Expr<Const>(a),b));
};
template <class ExprB>
Expr<BinExpr<Expr<Const>,Expr<ExprB>,Scale>> operator *(const Expr<ExprB> &b,const double a){
	typedef BinExpr<Expr<Const>,Expr<ExprB>,Scale> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(Expr<Const>(a),b));
};
template <class ExprB>
Expr<BinExpr<Expr<Const>,Expr<ExprB>,Plus>> operator +(const double a,const Expr<ExprB> &b){
	typedef BinExpr<Expr<Const>,Expr<ExprB>,Plus> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(Expr<Const>(a),b));
};
template <class ExprB>
Expr<BinExpr<Expr<Const>,Expr<ExprB>,Plus>> operator +(const Expr<ExprB> &b,const double a){
	typedef BinExpr<Expr<Const>,Expr<ExprB>,Plus> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(Expr<Const>(a),b));
};
template <class ExprB>
Expr<BinExpr<Expr<Const>,Expr<ExprB>,Minus>> operator -(const double a,const Expr<ExprB> &b){
	typedef BinExpr<Expr<Const>,Expr<ExprB>,Minus> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(Expr<Const>(a),b));
};
template <class ExprB>
Expr<BinExpr<Expr<Const>,Expr<ExprB>,Minus>> operator -(const Expr<ExprB> &b,const double a){
	typedef BinExpr<Expr<Const>,Expr<ExprB>,Minus> ExprTmp;
	return Expr<ExprTmp>(ExprTmp(Expr<Const>(a),b));
};
template <class ExprA>
void eval(Expr<ExprA> x,double start,double end){
	const double delta=0.1;
	end=end+delta/2;
	for(double i=start;i<end;i+=delta) cout<<x(i)<<'\n';
}
int main(int argc,char *argv[]){
	Expr<Syms> x;
	if(argc>1) {
		cout<<"xrange: 0 - "<<x(atof(argv[1]))<<endl;
	}
	else {
		cout<<"no input"<<endl;
		return 0;
	}
	cout<<"f(x)=2*x"<<endl;
	eval(x*2.0+1,0.0,atof(argv[1]));
	return 1;
}