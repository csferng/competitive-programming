#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

//#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#define DEBUG(...) {};

typedef long long int64;

const int64 BASE = 1000000009;
int EVENTS_PER_BATCH = 6000 * 6000;

typedef struct {
	int t, a, b;
} Event;

int T;
int N, Q, N_CYC;
int PARENT[5000001], ROOT[5000001], CYC[5000001], DEPTH[5000001];
int PWRPARENT[5000001 * 32];

int T_LOOKUP;
int LOOKUP[5000001];

inline void initLookUp() {
	++T_LOOKUP;
	if (T_LOOKUP == 0) {
		++T_LOOKUP;
		memset(LOOKUP, 0, sizeof(LOOKUP));
	}
}
inline void addLookUp(int x) { LOOKUP[x] = T_LOOKUP; }
inline bool hasLookUp(int x) { return LOOKUP[x] == T_LOOKUP; }

void build(const unordered_set<int>& key) {
	DEBUG("build: ");
	for (const int k : key) DEBUG(" %d", k);
	DEBUG("\n");
	memset(PWRPARENT, -1, sizeof(PWRPARENT));
	memset(ROOT, 0, sizeof(ROOT));
	memset(CYC, 0, sizeof(CYC));
	N_CYC = 0;
	vector<int> q;
	for (int i = 1, j, k; i <= N; ++i) if (!ROOT[i]) {
		if (key.find(i) != key.end()) {
			ROOT[i] = i;
			DEPTH[i] = 0;
			continue;
		}
		initLookUp();
		q.clear();
		addLookUp(i);
		q.push_back(i);
		for (j = PARENT[i]; j != 0 && !ROOT[j] && key.find(j) == key.end() && !hasLookUp(j); j = PARENT[j]) {
			addLookUp(j);
			q.push_back(j);
		}
		if (j == 0) {  // new tree root
			j = q.back();
			q.pop_back();
			ROOT[j] = j;
			DEPTH[j] = 0;
		} else if (key.find(j) != key.end()) {  // new tree root by key node
			ROOT[j] = j;
			DEPTH[j] = 0;
		} else if (ROOT[j] > 0) {  // existing tree/cycle root
		} else {  // new cycle
			++N_CYC;
			do {
				k = q.back();
				q.pop_back();
				ROOT[k] = k;
				CYC[k] = N_CYC;
				DEPTH[k] = 0;
			} while (k != j);
		}
		int l = q.size();
		for (int k = l-1; k >= 0; --k) {
			int x = q[k];
			ROOT[x] = ROOT[j];
			DEPTH[x] =  DEPTH[j] + l - k;
		}
	}
	for (int i = 1; i <= N; ++i) DEBUG("i=%d p=%d r=%d d=%d c=%d\n", i, PARENT[i], ROOT[i], DEPTH[i], CYC[i]);
}

int getParentPower(int key) {
	int k2 = key;
	while ((k2&0x1f) > 0 && PWRPARENT[k2] == -1) --k2;
	if ((k2&0x1f) == 0 && PWRPARENT[k2] == -1) {
		PWRPARENT[k2] = PARENT[PARENT[key>>5]];
	}
	int p = PWRPARENT[k2];
	for (; k2 < key; ++k2) {
		p = PWRPARENT[k2+1] = getParentPower((p<<5) | (k2&0x1f));
	}
	return PWRPARENT[key];
}

int moveUp(int x, int d) {
	if (d & 1) {
		x = PARENT[x];
		d ^= 1;
	}
	for (int b = 1; d > 0; ++b) if (d & (1<<b)) {
		int k = (x<<5) | (b-1);
		x = PWRPARENT[k]==-1 ? getParentPower(k) : PWRPARENT[k];
		d ^= 1 << b;
	}
	return x;
}

bool reachable(int fr, int to, const unordered_set<int>& key) {
	DEBUG("reachable(%d,%d)\n", fr, to);
	fr = PARENT[fr];  // shoot once, to differentiate x~>x and x->0 when fr=to=x
	DEBUG("   fr -> %d\n", fr);
	if (fr == 0) return false;
	DEBUG("   r[fr]=%d r[to]=%d\n", ROOT[fr], ROOT[to]);
	initLookUp();
	do {
		addLookUp(fr);
		if (ROOT[fr] == ROOT[to]) {
			int df = DEPTH[fr], dt = DEPTH[to];
			DEBUG("   d[fr]=%d d[to]=%d\n", df, dt);
			if (df > dt) {
				fr = moveUp(fr, df-dt);
				DEBUG("   fr -> %d  reach=%d\n", fr, fr==to);
			}
			if (fr == to) return true;
		} else if (key.find(ROOT[fr]) == key.end()) {
			DEBUG("  c[r[fr]]=%d, c[to]=%d\n", CYC[ROOT[fr]], CYC[to]);
			return CYC[ROOT[fr]] > 0 && ROOT[to] == to && CYC[ROOT[fr]] == CYC[to];
		}
		fr = PARENT[ROOT[fr]];
		DEBUG("   fr -> %d\n", fr);
	} while(fr > 0 && !hasLookUp(fr));
	return false;
}

void processBatch(const vector<Event> &ve, int *ans) {
	unordered_set<int> key;
	for (const auto &e : ve) if (e.t == 1) {
		key.insert(e.a);
	}
	build(key);
	for (const auto &e : ve) {
		if (e.t == 1) {
			PARENT[e.a] = e.b;
		} else {
			*ans = *ans*2 + reachable(e.a, e.b, key);
			if (*ans >= BASE) *ans -= BASE;
			DEBUG("%d->%d %d\n", e.a, e.b, (*ans)&1);
		}
	}
}

int main(int argc, char **argv) {
	if (argc > 1) {
		sscanf(argv[1], "%d", &EVENTS_PER_BATCH);
	}
	for(T = 1; T > 0; --T) {
		scanf("%d %d", &N, &Q);
		memset(PARENT, 0, sizeof(PARENT));
		int ans = 0;
		vector<Event> v;
		while (Q > 0) {
			v.clear();
			int t1 = 0, t2 = 0;
			for (; Q > 0 && v.size() < EVENTS_PER_BATCH; --Q) {
				v.push_back(Event());
				Event &e = v.back();
				scanf("%d %d %d", &e.t, &e.a, &e.b);
				if (e.t == 1 && t2 == 0) {
					PARENT[e.a] = e.b;
					v.pop_back();
				} else {
					if (e.t == 1) t1++;
					else t2++;
				}
			}
			fprintf(stderr, "doing t1=%d t2=%d remaining Q=%d time=%lu\n", t1, t2, Q, clock()/CLOCKS_PER_SEC);
			processBatch(v, &ans);
		}
		printf("%d\n", ans);
	}
}
