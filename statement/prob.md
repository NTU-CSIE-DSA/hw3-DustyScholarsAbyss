## Problem Description

**The description on [NTU COOL]() shall be considered the official one. This version is provided for reference only.**

### Story Version

DerTian, an archaeologist, has uncovered the ruins of an ancient civilization buried deep beneath the desert sands. These ruins comprise multiple underground cities, once interconnected by a vast network of tunnels that served as trade routes and pathways.
Over the centuries, many of these tunnels have collapsed, and some cities have become isolated. To guide zir excavation and preservation efforts, DerTian must map the network and uncover its structural weaknesses.
The network consists of $N$ cities and $M$ tunnels. Each city is represented by an integer from $0$ to $N - 1$, and each tunnel is a bidirectional connection between two cities.

DerTian wants to identify:

* **Critical Tunnels**: Tunnels whose collapse would disconnect parts of the network.
* **Critical Cities**: Cities whose loss would fragment the network into separate regions.
* **Independent Exploration Zones**: Zones of ruins that remains connected even if any single city is removed.

These discoveries will help DerTian decide which tunnels to reinforce and where to focus excavation efforts.

### Formal Version

You are given an undirected graph with $N$ vertices and $M$ edges. Determine the number of bridges, the number of articulation points, and the biconnected components. 


## Reference Reading
The following provides information about *Articulation Points*, *Bridges*, and *Biconnected Components* that may be helpful to solve this problem. You may also find these information in the older version of the textbook and/or the Internet. 

**Definitions.** 
Let $G = (V, E)$ be a connected, undirected graph. 
* An articulation point is a vertex whose removal increases the number of connected components.
* A bridge is an edge whose removal disconnects the graph.
* A biconnected component is a maximal subgraph that remains connected even after the removal of any single vertex (and its incident edges).

We can determine **articulation points**, **bridges**, and **biconnected components (BCCs)** using DFS, along with a few additional properties. Suppose that $G_{\pi} = G(V, E_{\pi})$ is the **depth-first search (DFS) tree** of $G$, i.e., the spanning tree generated according to the visit order and parent-child relationships in DFS. 
<!--
For each vertex $u$, we maintain two more values. 
* $t_{in}(u)$, the time at which $u$ is first visited during DFS, 
* $low(u) = \min{(t_{in}(v))}$, where $v$ ranges over all vertices reachable from $u$ by going down zero or more tree edges and then using at most one back edge.
-->
The root of $G_{\pi}$ is an articulation point of $G$ if and only if it has at least two children in $G_{\pi}$. For any non-root vertex $u$ of $G_{\pi}$, it is an articulation point if and only if it has a child $v$ such that no back edge exists from $v$ or any of its descendants to a proper ancestor of $u$. 
<!--
In other words, if there exists a child $v$ of $u$ such that $low(v) \geq t_{in}(u)$, then $u$ is an articulation point. 
-->
An edge of $(u, v)$ is a bridge if and only if it does not lie on any simple cycle of $G$. Articulation points divide the graph into biconnected components, which are the maximal subgraphs that remain connected after the removal of any single vertex.

![Articulation Points](https://hackmd.io/_uploads/S1xxz6oRJe.png)
![Bridges](https://hackmd.io/_uploads/SyxxfTiCJx.png)
![Biconnected Components](https://hackmd.io/_uploads/SJelMpoRyl.png)


## Input

The first line contains three integers $N$, $M$, and `mode`. $N$ is the number of cities, and the cities are numbered from $0$ to $N - 1$. $M$ is the number of tunnels. `mode` has a value of either $1$ or $2$.
Each of the next $M$ lines contains two integers $u_i$ and $v_i$, indicating the two cities connected by the $i$-th tunnel. The integers in the same line are separated by single white space. 


## Output

If `mode` is $1$, print the number of critical cities in the first line. In the second line, print the city IDs in ascending order.

If `mode` is $2$, print the number of critical tunnels in the first line. In the second line, print the number of independent exploration zones $Z$. In the following $Z$ lines, print the city IDs of each independent exploration zone. The groups must be printed in lexicographical order, and within each group, the city IDs should be sorted in ascending order.


## Constraints
* $1 \leq N \leq 10^5$
* $1 \leq M \leq \min\left(\dfrac{N(N-1)}{2}, 10^6\right)$
* $0 \leq u_i, v_i \leq N - 1$
* The graph is an undirected connected simple graph, i.e., it does not contain self loops or multiple edges.

## Subtasks

### Subtask 1 (15 pts)

* $1 \leq N \leq 10^3$
* `mode` = 1

### Subtask 2 (25 pts)

* $1 \leq N \leq 10^5$
* `mode` = 1

### Subtask 3 (15 pts)

* $1 \leq N \leq 10^3$
* `mode` = 2
* The test case in this subtask is the same as in subtask 1; the only difference is the `mode`.

### Subtask 4 (45 pts)

* $1 \leq N \leq 10^5$
* `mode` = 2
* The test case in this subtask is the same as in subtask 2; the only difference is the `mode`.


## Sample Testcases

### Sample Input 1
```
14 16 1
0 2
0 1
1 3
2 3
3 4
4 5
5 6
6 12
6 7
6 8
8 9
8 10
10 11
11 13
11 12
12 13
```
### Sample Output 1
```
5
3 4 5 6 8 
```

### Sample Input 2
```
14 16 2
0 2
0 1
1 3
2 3
3 4
4 5
5 6
6 12
6 7
6 8
8 9
8 10
10 11
11 13
11 12
12 13
```

### Sample Output 2
```
5
7
0 1 2 3
3 4
4 5
5 6
6 7
6 8 10 11 12 13
8 9
```

## Hint 
An independent exploration zone can be composed of only two cities with a tunnel connecting each other since removing one of them remains the connectivity of the other city. 