#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <map>
using namespace std;

//#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#define DEBUG(...) {};

typedef long long int64;
const int BASE = 1000000009;
const int MIN_X = -1010000000;  // -10^9 - 10^7
const int MAX_X = 1010000000;  // 10^9 + 10^7

int T;
int N;

namespace treap {

struct Treap {
	int key, val, prio, sum;
	Treap *lf, *rt;
};

Treap _nodes[5000000*4];
int n_node;

Treap* newTreap(int k, int v) {
	Treap* r = _nodes + n_node;
	++n_node;
	r->key = k;
	r->val = v;
	r->sum = v;
	r->prio = rand();
	r->lf = r->rt = nullptr;
	return r;
}

inline int getSum(Treap *t) {
	return t == nullptr ? 0 : t->sum;
}

void split(Treap *t, int k, Treap** a, Treap** b) {
	if (t == nullptr) {
		*a = *b = nullptr;
		return;
	}
	if (t->key < k) {
		*a = t;
		split(t->rt, k, &((*a)->rt), b);
		(*a)->sum = (*a)->val + getSum((*a)->lf) + getSum((*a)->rt);
	} else {
		*b = t;
		split(t->lf, k, a, &((*b)->lf));
		(*b)->sum = (*b)->val + getSum((*b)->lf) + getSum((*b)->rt);
	}
}

void merge(Treap* a, Treap* b, Treap** r) {
	if (a == nullptr) *r = b;
	else if (b == nullptr) *r = a;
	else {
		if (a->prio > b->prio) {
			*r = a;
			merge(a->rt, b, &((*r)->rt));
		} else {
			*r = b;
			merge(a, b->lf, &((*r)->lf));
		}
		(*r)->sum = (*r)->val + getSum((*r)->lf) + getSum((*r)->rt);
	}
}

}  // namespace treap

treap::Treap* root;

void init() {
	root = nullptr;
	treap::n_node = 0;
}

int prefixSum(int k) {
	int s = 0;
	treap::Treap* t = root;
	while (t != nullptr) {
		if (t->key > k) {
			t = t->lf;
		} else {
			s += t->val + treap::getSum(t->lf);
			t = t->rt;
		}
	}
	return s;
}

int findKeyLE(int k) {
	treap::Treap* t = root;
	int mx = MIN_X;
	while (t != nullptr) {
		if (t->key > k) {
			t = t->lf;
		} else {
			mx = max(mx, t->key);
			t = t->rt;
		}
	}
	return mx;
}
int findKeyGE(int k) {
	treap::Treap* t = root;
	int mn = MAX_X;
	while (t != nullptr) {
		if (t->key < k) {
			t = t->rt;
		} else {
			mn = min(mn, t->key);
			t = t->lf;
		}
	}
	return mn;
}

void debugAcc(treap::Treap* t) {
	if (t == nullptr) return;
	DEBUG("<")
	debugAcc(t->lf);
	DEBUG(" %d:%d:%d", t->key, t->val, t->sum);
	debugAcc(t->rt);
	DEBUG(">")
}

int debugSlope(treap::Treap* t, int a) {
	if (t == nullptr) return a;
	a = debugSlope(t->lf, a);
	a += t->val;
	DEBUG(" %d:%d", t->key, a);
	a = debugSlope(t->rt, a);
	return a;
}

void update(const map<int, int>& ch) {
	for (const auto& itr : ch) {
		treap::Treap *lf = nullptr, *md = nullptr, *rt = nullptr, *tmp = nullptr;
		treap::split(root, itr.first, &lf, &tmp);
		treap::split(tmp, itr.first+1, &md, &rt);
		if (md == nullptr) {
			md  = treap::newTreap(itr.first, itr.second);
		} else {
			md->val += itr.second;
			md->sum += itr.second;
			if (md->val == 0) md = nullptr;
		}
		/*DEBUG("lf:"); debugAcc(lf); DEBUG("\n");
		DEBUG("md:"); debugAcc(md); DEBUG("\n");
		DEBUG("rt:"); debugAcc(rt); DEBUG("\n");*/
		treap::merge(lf, md, &tmp);
		//DEBUG("tmp:"); debugAcc(tmp); DEBUG("\n");
		treap::merge(tmp, rt, &root);
	}
}

int64 operate(int x, int d) {
	map<int, int> ch;
	int lf, rt;
	if (prefixSum(x-1) != d) {
		lf = x;
	} else {
		lf = findKeyLE(x-1);
	}
	ch[lf-1] += d;
	ch[lf] += -d;
	if (prefixSum(x) != -d) {
		rt = x;
	} else {
		rt = findKeyGE(x+1);
	}
	ch[rt] += -d;
	ch[rt+1] += d;
	DEBUG("ch: "); for (const auto& itr : ch) DEBUG(" %d:%d", itr.first, itr.second); DEBUG("\n");
	update(ch);
	//DEBUG("acc: "); debugAcc(root); DEBUG("\n");
	//DEBUG("slope: "); debugSlope(root, 0); DEBUG("\n");
	return rt - lf + 1;
}

int main() {
	for(scanf("%d", &T); T > 0; --T) {
		fprintf(stderr, "%d\n", T);
		scanf("%d", &N);
		init();
		int64 ans = 0;
		for (int i = 1, x, d; i <= N; ++i) {
			scanf("%d %d", &x, &d);
			int64 a = operate(x, d);
			ans = (ans+i*a) % BASE;
			if (i % 10000 == 0) fprintf(stderr, "\t%d time=%lu\n", N-i, clock()/CLOCKS_PER_SEC);
		}
		printf("%lld\n", ans);
	}
}
