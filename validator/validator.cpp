// To be modified
#include "testlib.h"
#include <assert.h>
using namespace std;

#define MAXN 100000
#define MAXM 1000000
/*
 * Testcase format:
 * <N> <M> <type>
 * <u_1> <v_1>
 * <u_2> <v_2>
 * ...
 * <u_M> <v_M>
 */
int main() {
	registerValidation();
	int n = inf.readInt(1, MAXN, "n");
	inf.readSpace();
	int m = inf.readInt(n - 1, MAXM, "m");
	inf.readSpace();
	inf.readInt(1, 2, "type");
	inf.readEoln();
	set<pair<int, int>> edges;
	for (int i=0; i<m; i++){
		int u = inf.readInt(0, n - 1, "u");
		inf.readSpace();
		int v = inf.readInt(0, n - 1, "v");
		if (u > v){
			swap(u, v);
		}
		if (u == v){
			inf.quitf(_fail, "Self loop");
		}
		if (edges.count(make_pair(u, v))){
			inf.quitf(_fail, "Duplicate edge");
		}
		inf.readEoln();
		edges.insert(make_pair(u, v));
	}

	inf.readEof();
	return 0;
}
