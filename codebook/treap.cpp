#include <stdlib.h>

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
