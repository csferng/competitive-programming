#include <vector>
using namespace std;

namespace fft {
struct Polynomial {
    vector<int> coef;
    inline void Clear() {
        coef.clear();
    }
    inline int Length() const {
        return coef.size();
    }
    inline void SetCoef(int n, int a) {
        if (coef.size() <= n) coef.resize(n+1);
        coef[n] = a;
    }
    inline void IncrCoef(int n, int a) {
        if (coef.size() <= n) coef.resize(n+1);
        coef[n] += a;
    }
    void Init(int *a, int step, int n) {
        Clear();
        for (int i = 0, *p = a; i < n; ++i, p += step) coef.push_back(*p);
    }
    void Add(const Polynomial &that) {
        if (coef.size() < that.coef.size()) coef.resize(that.coef.size());
        for (int i = 0; i < that.coef.size(); ++i) {
            coef[i] += that.coef[i];
        }
    }
    void Sub(const Polynomial &that) {
        if (coef.size() < that.coef.size()) coef.resize(that.coef.size());
        for (int i = 0; i < that.coef.size(); ++i) {
            coef[i] -= that.coef[i];
        }
    }
    void Print(FILE *f) {
            for (int i = coef.size()-1; i > 0; --i) {
                    fprintf(f, "%+d*x^%d ", coef[i], i);
            }
            fprintf(f, "%+d\n", coef[0]);
    }

};

struct CrtNumber {
        static const long long P1 = 7340033;   // 7*2^20+1
        static const long long G1 = 2187;      // 3^7 % P1
        static const long long L1 = 20;        // argmin: G1^L1 % P1 == 1
        static const long long P2 = 23068673;  // 11*2^21+1
        static const long long G2 = 177147;    // 3^11 % P2
        static const long long L2 = 21;        // argmin: G2^L2 % P2 == 1
        long long r1, r2;
        CrtNumber():r1(0), r2(0) {}
        CrtNumber(int a):r1(a%P1), r2(a%P2) {}
        void operator+=(const CrtNumber& a) {
                r1 += a.r1, r2 += a.r2;
                if (r1 >= P1) r1 -= P1;
                if (r2 >= P2) r2 -= P2;
        }
        void operator-=(const CrtNumber& a) {
                r1 -= a.r1, r2 -= a.r2;
                if (r1 < 0) r1 += P1;
                if (r2 < 0) r2 += P2;
        }
        void operator*=(const CrtNumber& a) {
                r1 = (r1*a.r1) % P1;
                r2 = (r2*a.r2) % P2;
        }
        CrtNumber operator+(const CrtNumber& a) const {
                CrtNumber x = *this;
                x += a;
                return x;
        }
        CrtNumber operator-(const CrtNumber& a) const {
                CrtNumber x = *this;
                x -= a;
                return x;
        }
        CrtNumber operator*(const CrtNumber& a) const {
                CrtNumber x = *this;
                x *= a;
                return x;
        }
};

long long powmod(long long a, int n, long long p) {
        long long x = 1;
        for (int b = 1; b <= n; b <<= 1, a = (a*a)%p) if (n&b) x = (x*a) % p;
        return x;
}

inline void SetBase(int n, CrtNumber* w) {
        w->r1 = powmod(CrtNumber::G1, (1<<CrtNumber::L1)/n, CrtNumber::P1);
        w->r2 = powmod(CrtNumber::G2, (1<<CrtNumber::L2)/n, CrtNumber::P2);
}
long long inv(long long a, long long b) {
        // 1 = x*a+y*b = x*a+y*(q*a+r)
        if (a == 1) return 1;
        long long r = b % a;
        long long y = inv(r, a);
        return (1-y*b) / a;
}
inline CrtNumber Inverse(const CrtNumber& w) {
        CrtNumber x;
        x.r1 = inv(w.r1, CrtNumber::P1);
        if (x.r1 < 0) x.r1 = (x.r1%CrtNumber::P1) + CrtNumber::P1;
        x.r2 = inv(w.r2, CrtNumber::P2);
        if (x.r2 < 0) x.r2 = (x.r2%CrtNumber::P2) + CrtNumber::P2;
        return x;
}
inline long long ToInt(const CrtNumber& w) {
        // x = q1*p1+r1 = r2 (%P2)
        long long r = w.r2<w.r1 ? w.r2+CrtNumber::P2-w.r1 : w.r2-w.r1;
        long long q = (r*1537913) % CrtNumber::P2;  // inv(P1,P2) = 1537913
        return q*CrtNumber::P1 + w.r1;
}
inline CrtNumber Multiply(const CrtNumber& a, const CrtNumber& b) { return a*b; }

typedef CrtNumber Value;
//typedef complex<long double> Value;

inline void SetBase(int n, complex<long double>* w) {
    w->real(cos(2.*acos(-1)/n));
    w->imag(sin(2.*acos(-1)/n));
}
    
inline complex<long double> Inverse(const complex<long double>& w) {
    return complex<long double>(w.real(), -w.imag());
}
    
inline long long ToInt(const complex<long double>& w) {
    return (long long)(w.real()+0.5);
}

inline complex<long double> Multiply(const complex<long double> &a, const complex<long double> &b) {
        return complex<long double>(a.real()*b.real()-a.imag()*b.imag(), a.real()*b.imag()+a.imag()*b.real());
}

vector<int> BIT_REVERSE[24];
void InitBitReverse() {
        for (int b = 0; b < 20; ++b) {
                int n = 1 << b;
                vector<int>&br = BIT_REVERSE[b];
                br.resize(n);
                for (int i = 1; i < n; ++i) {
                        int& j = br[i];
                        for (int b2 = 0; b2 < b; ++b2) j = (j<<1) | ((i>>b2)&1);
                }
        }
}

void DoFft(int nb, const Value w_base, vector<Value>* v) {
        //  n = v->size() == 1<<nb
        int n = 1 << nb;
        const auto& br = BIT_REVERSE[nb];
        for (int i = 0; i < n; ++i) if (i < br[i]) {
                swap((*v)[i], (*v)[br[i]]);
        }
        vector<Value> w0(nb);
        w0[0] = w_base;
        for (int i = 1; i < nb; ++i) w0[i] = Multiply(w0[i-1], w0[i-1]);
        for (int b = nb-1; b >= 0; --b) {
                int step = 1 << (nb-b);
                for (int s = 0; s < n; s += step) {
                        int h = step >> 1;
                        Value w(1);
                        for (int i = 0; i < h; ++i, w = Multiply(w, w0[b])) {
                                Value wv = Multiply(w, (*v)[s+i+h]);
                                (*v)[s+i+h] = (*v)[s+i] - wv;
                                (*v)[s+i] += wv;
                        }
                }
        }
}

void MultiplyViaFft(const Polynomial &a, const Polynomial &b, Polynomial *r) {
    int n, nb;
    for (n = 1, nb = 0; n < a.Length()+b.Length(); n *= 2, ++nb);
    vector<Value> va(n), vb(n);
    for (int i = 0; i < a.Length(); ++i) va[i] = a.coef[i];
    for (int i = 0; i < b.Length(); ++i) vb[i] = b.coef[i];
    Value w;
    SetBase(n, &w);
    DoFft(nb, w, &va);
    DoFft(nb, w, &vb);
    for (int i = 0; i < n; ++i) {
            va[i] *= vb[i];
    }
    DoFft(nb, Inverse(w), &va);
    for (int i = a.Length()+b.Length()-2; i >= 0; --i) {
            r->SetCoef(i, ToInt(va[i])/n);
    }
}
}
