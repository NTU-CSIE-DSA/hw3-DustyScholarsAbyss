# Solution

## Introduction

This problem asked to find articulation points, bridges, and binconnected components of a connected simple graph.

## Implementation
- Run DFS over the graph once, record the time visited, lowest descendent, maintain the edge stack, etc. (Time complexity: $O(m + n)$)
- Construct the BCCs when $u.d = u.low$ during the DFS process. (Time complexity: $O(m' + n')$, where $m'$ and $n'$ are the number of edges and vertices in the constructed BCC)
- Use the DFS spanning tree to determine the articulation points. (Time complexity: $O(m + n)$)
- Determine the bridges by checking every edge using the relation $v.low > u.d$ for edge $(u, v)$. (Time complexity: $O(m + n)$)
- Sort the result if needed (Time complexity: $O(n \log n)$)

## File Structure

```
public/
├── correct.c           # Main solution 
├── gpt.c               # Solution by ChatGPT 4o 
└── tle.c               # Unefficient solution 
```

## Common Mistakes 
1. $low = d$ case
   When finding articulation points, the determining condition includes equality. The result may be wrong if the case that equality holds is not considered. 
2. Sorting: `qsort` useage
   Using `int *` directly instead of `*(int **)` for a 2D array sorting is incorrect. 
