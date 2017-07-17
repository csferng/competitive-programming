#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

//#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#define DEBUG(...) {};

typedef long long int64;

int T;
int N, SCCID[500001];
vector<int> ADJ[500001], RADJ[500001];

int X, IDX[500001], LOW[500001], IN[500001];
char ON[500001];
vector<int> S;

void mergeScc(int x) {
	DEBUG("merge %d\n", x);
	unordered_set<int> v;
	while (!S.empty() && S.back() != x) {
		int x2 = S.back();
		v.insert(x2);
		ON[x2] = 0;
		S.pop_back();
		DEBUG("pop %d\n", x2);
	}
	S.pop_back();
	ON[x] = 0;
	if (v.empty()) {
		bool loop = false;
		for (int c : ADJ[x]) if (c == x) {
			loop = true;
			break;
		}
		if (!loop) return;
	}
	v.insert(x);
	for (int i = 0; i < ADJ[x].size(); ++i) if (v.find(ADJ[x][i]) != v.end()) {
		ADJ[x][i] = ADJ[x][ADJ[x].size()-1];
		--i;
		ADJ[x].pop_back();
	}
	unordered_set<int> e(ADJ[x].begin(), ADJ[x].end());
	DEBUG("scc [%d]", x);
	for (int x2 : v) {
		DEBUG(" %d", x2);
		SCCID[x2] = x;
		for (int c : ADJ[x2]) if (v.find(c) == v.end() && e.find(c) == e.end()) {
			ADJ[x].push_back(c);
			e.insert(c);
		}
	}
	DEBUG("\n");
	SCCID[x] = x;
}

void findScc(int x, int p) {
	DEBUG("push %d\n", x);
	IDX[x] = X;
	LOW[x] = X;
	X++;
	S.push_back(x);
	ON[x] = 1;
	for (const int c : ADJ[x]) {
		if (IDX[c] == -1) {
			findScc(c, x);
			LOW[x] = min(LOW[x], LOW[c]);
		} else if (ON[c]) {
			LOW[x] = min(LOW[x], IDX[c]);
		}
	}
	if (LOW[x] == IDX[x]) {
		mergeScc(x);
	}
	DEBUG("x %d %d %d\n", x, IDX[x], LOW[x]);
}

void dfs(int x, int p) {
	ON[x] |= 1;
	for (const int c : ADJ[x]) if ((ON[c]&1) == 0 && c != p && (SCCID[c]==-1 || SCCID[c]==c)) {
		dfs(c, x);
	}
}

void rdfs(int x, int p) {
	ON[x] |= 2;
	for (const int c : RADJ[x]) if ((ON[c]&2) == 0 && c != p && (SCCID[c]==-1 || SCCID[c]==c)) {
		rdfs(c, x);
	}
}

vector<int> solve() {
	X = 0;
	memset(IDX, -1, sizeof(IDX));
	memset(SCCID, -1, sizeof(SCCID));
	memset(ON, 0, sizeof(ON));
	S.clear();
	DEBUG("scc\n");
	findScc(1, -1);
	memset(ON, 0, sizeof(ON));
	for (int i = 1; i <= N; ++i) {
		unordered_set<int> e;
		for (int c : ADJ[i]) e.insert(SCCID[c] == -1 ? c : SCCID[c]);
		ADJ[i] = vector<int>(e.begin(), e.end());
	}
	DEBUG("dfs/rdfs\n");
	dfs(1, -1);
	for (int i = 1; i <= N; ++i) RADJ[i].clear();
	for (int i = 1; i <= N; ++i) if (SCCID[i]==-1 || SCCID[i]==i) {
		for (int c : ADJ[i]) {
			RADJ[c].push_back(i);
		}
	}
	rdfs(SCCID[N]==-1 ? N : SCCID[N], -1);
	memset(IN, 0, sizeof(IN));
	DEBUG("topsort\n");
	for (int i = 1; i <= N; ++i) if (ON[i] == 3) {
		for (int c : ADJ[i]) if (ON[c] == 3) {
			IN[c] += 1;
		}
	}
	for (int i = 1; i <= N; ++i) DEBUG("%d %d %d\n", i, ON[i], IN[i]);
	vector<int> top;
	for (int i = 1; i <= N; ++i) if (ON[i] == 3 && IN[i] == 0) {
		top.push_back(i);
	}
	for (int i = 0; i < top.size(); ++i) {
		int x = top[i];
		for (int c : ADJ[x]) if (ON[c] == 3) {
			IN[c] -= 1;
			if (IN[c] == 0) top.push_back(c);
		}
	}
	vector<int> ans;
	unordered_map<int, int> ord;
	DEBUG("ord\n");
	for (int i = 0; i < top.size(); ++i) {
		ord[top[i]] = i;
		DEBUG("%d ", top[i]);
	}
	DEBUG("\n");
	int rt = -1;
	for (int i = 0; i < top.size(); ++i) {
		int x = top[i];
		if (rt <= i && SCCID[x] == -1) ans.push_back(x);
		for (int c : ADJ[x]) if (ON[c] == 3) {
			rt = max(rt, ord[c]);
		}
	}
	return ans;
}

int main() {
	for(scanf("%d", &T); T > 0; --T) {
		fprintf(stderr, "%d\n", T);
		int e;
		scanf("%d %d", &N, &e);
		for (int i = 1; i <= N; ++i) ADJ[i].clear();
		for (int i = 0, a, b; i < e; ++i) {
			scanf("%d %d", &a, &b);
			ADJ[a].push_back(b);
		}
		vector<int> a = solve();
		printf("%d\n", a.size());
		for (int i = 0; i < a.size(); ++i) {
			if (i > 0) printf(" ");
			printf("%d", a[i]);
		}
		printf("\n");
	}
}
