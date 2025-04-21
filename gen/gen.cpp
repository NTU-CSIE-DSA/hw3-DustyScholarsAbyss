#include <iostream>
#include "jngen.h"
using namespace std;
/*
 * Testcase format:
 * <N> <M> <type>
 * <u_1> <v_1>
 * <u_2> <v_2>
 * ...
 * <u_M> <v_M>
 */

/*
 * Testcase type:
 * 1. a random graph
 * 2. a cycle
 * 3. a complete graph
 * 4. a bipartite graph
 * 5. a complete bipartite graph
 * 6. a star
 * 7. two vertices connected by an edge
 * 8. a path
 * 9. a caterpillar
 * 10. a binary tree
 * 11. a random tree
 */
#define MAXN 100000
#define MAXM 1000000

// Usage: gen <minN> <maxN> <minM> <maxM> <type>
int main(int argc, char* argv[]){
	registerGen(argc, argv);
	int minN = (atoi(argv[1]) < 0) ? 2 : atoi(argv[1]);
	int maxN = (atoi(argv[2]) < 0) ? 100000 : atoi(argv[2]);
	int minM = (atoi(argv[3]) < 0) ? 1 : atoi(argv[3]);
	int maxM = (atoi(argv[4]) < 0) ? 100000  : atoi(argv[4]);
	int type = (atoi(argv[5]) < 0) ? 1 : atoi(argv[5]);
	
	int n = rnd.next(minN, maxN);
	int m = rnd.next(minM, maxM);

	if (type == 1){
		// Random graph
		cout << n << " " << m << " 1\n";
		cout << Graph::random(n, m).connected().g().shuffled() << endl;
	}
	else if (type == 2){
		// a cycle
		m = n;
		cout << n << " " << m << " 1\n";
		cout << Graph::cycle(n).g().shuffled() << endl;
	}
	else if (type == 3){
		// a complete graph
		if (n > 1414){
			n = 1414;
		}
		m = n * (n - 1) / 2;
		cout << n << " " << m << " 1\n";
		cout << Graph::complete(n).g().shuffled() << endl;
	}
	else if (type == 4){
		// a bipartite graph
		int n1 = rnd.next(1, n - 1);
		int n2 = n - n1;
		m = rnd.next(n - 1, min(n1 * n2, m));
		cout << n << " " << m << " 1\n";
		cout << Graph::randomBipartite(n1, n2, m).connected().g().shuffled() << endl;
	}
	else if (type == 5){
		// a complete bipartite graph
		int n1 = rnd.next(1, n - 1);
		int n2 = min(MAXM / n1, n - n1);
		while (n2 == 0){
			n1 = rnd.next(1, n - 1);
			n2 = min(MAXM * 2 / n1, n - n1);
		}
		m = n1 * n2;
		cout << n << " " << m << " 1\n";
		cout << Graph::completeBipartite(n1, n2).g().shuffled() << endl;
	}
	else if (type == 6){
		// a star
		m = n - 1;
		cout << n << " " << m << " 1\n";
		cout << Tree::star(n).shuffled() << endl;
	}
	else if (type == 7){
		// two vertices connected by an edge
		cout << "2 1 1\n";
		cout << "0 1\n";
	}
	else if (type == 8){
		// a path
		m = n - 1;
		cout << n << " " << m << " 1\n";
		cout << Tree::bamboo(n).shuffled() << endl;
	}
	else if (type == 9){
		// a caterpillar
		m = n - 1;
		int length = rnd.next(1, n - 1);
		cout << n << " " << m << " 1\n";
		cout << Tree::caterpillar(n, length).shuffled() << endl;
	}
	else if (type == 10){
		// a binary tree
		m = n - 1;
		cout << n << " " << m << " 1\n";
		cout << Tree::binary(n).shuffled() << endl;
	}
	else if (type == 11){
		// a random tree
		m = n - 1;
		cout << n << " " << m << " 1\n";
		cout << Tree::random(n).shuffled() << endl;
	}
	
	return 0;
}
