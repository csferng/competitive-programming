#include <stdio.h>
#include <vector>
using namespace std;

#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#define DEBUG(...) {};

typedef long long int64;

const int BASE = 1000000009;
const int MX = 100000;

int T;
int N, A[131072];
int64 FAC[131072], IFAC[131072], CYC[131072];

inline int64 mul(int64 a, int64 b) { return (a*b) % BASE; }

int64 inv(int64 a, int64 b) {
	// 1 = a*x+b*y = a*x+(q*a+r)*y
	if (a == 1) return 1;
	int64 r = b % a;
	int64 y = inv(r, a);
	int64 x = ((1-y*b) / a) % b;
	return x < 0 ? x+b : x;
}

inline int64 C(int n, int k) { return mul(mul(FAC[n], IFAC[k]), IFAC[n-k]); }

void build() {
	FAC[0] = 1;
	IFAC[0] = 1;
	for (int i = 1; i <= MX; ++i) {
		FAC[i] = mul(FAC[i-1], i);
		IFAC[i] = inv(FAC[i], BASE);
	}
	CYC[1] = 1;
	CYC[2] = 1;
	for (int n = 3, m; n <= MX; ++n) {
		int64 s = 0;
		for (m = 1; m < n-m; ++m) {
			s += mul(mul(CYC[m], CYC[n-m]), C(n-2, m-1));
		}
		s *= 2;
		if (m == n-m) {
			s += mul(mul(CYC[m], CYC[n-m]), C(n-2, m-1));
		}
		s = mul(mul(s%BASE, n), IFAC[2]);
		CYC[n] = s;
	}
	for (int i = 1; i <= 10; ++i) DEBUG("%d\t%lld\n", i, CYC[i]);
	DEBUG("%d\t%lld\n", MX, CYC[MX]);
}

int64 solve() {
	for (int i = 0; i < N; ++i) A[i]--;
	int64 ans = 1, cnt = 0;
	vector<int> cyc;
	for (int i = 0; i < N; ++i) {
		cyc.clear();
		cyc.push_back(i);
		for (int j = A[i]; j != i; j = A[j]) {
			cyc.push_back(j);
		}
		if (cyc.size() > 1) {
			int l = cyc.size();
			ans = mul(mul(ans, CYC[l]), C(cnt+l-1, l-1));
			cnt += l-1;
			DEBUG("i=%d l=%d ans=%lld cnt=%lld\n", i, l, ans, cnt);
			for (int j : cyc) {
				A[j] = j;
			}
		}
	}
	return ans;
}

int main() {
	build();
	for(scanf("%d", &T); T > 0; --T) {
		scanf("%d", &N);
		for (int i = 0; i < N; ++i) {
			scanf("%d", &A[i]);
		}
		printf("%lld\n", solve());
	}
}
