template <typename T=int> struct range{
    T e,b=0,s=1;range(T b,T e,T s):e(e),b(b),s(s){} range(T b,T e): e(e), b(b){} range(T e):e(e){}
    struct it{T v,s; it(T v,T s):v(v),s(s){} operator T()const{return v;} inline operator T&(){return v;}T operator*()const{return v;}
        inline it& operator++(){v+=s;return *this;}}; it begin(){return {b,s};} it end(){return {e,s};}};
