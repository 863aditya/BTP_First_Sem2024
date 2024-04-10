from collections import deque
import sys
import random

MAXN = 150
MAXLayers = 10
n=0
class Node:
    def __init__(self, layer=MAXLayers, number=MAXN, threshold=1.0):
        self.layer = layer
        self.number = number
        self.threshold = threshold

    def __lt__(self, other):
        return self.number < other.number

    def __eq__(self, other):
        return self.number == other.number and self.layer == other.layer

    def __hash__(self):
        return hash((self.layer, self.number, self.threshold))

    def __repr__(self):
        return f"Node(layer={self.layer}, number={self.number}, threshold={self.threshold})"

    def __str__(self):
        return f"{self.layer}-{self.number}-{self.threshold}"

nodedata = [[Node() for _ in range(MAXLayers)] for _ in range(MAXN)]
graph = {}

def influence_value(v):
    q = deque()
    cur_influence_value = {}
    vis = {}
    for e in v:
        q.append(e)
        vis[e] = True
    while q:
        v = q.popleft()
        vis[v] = True
        for x, weight in graph[v]:
            cur_influence_value[x] = cur_influence_value.get(x, 0) + weight
            if cur_influence_value[x] >= x.threshold and not vis.get(x, False):
                q.append(x)
                vis[x] = True
    total_influence_value = 0
    active_vertex_count = 0
    for e, is_active in vis.items():
        if is_active:
            for u, _ in graph[e]:
                total_influence_value += _
    return total_influence_value / 2.0, sum(vis.values())

def marginal_gain_function(v, u):
    initial_gain = influence_value(v)[0]
    v.append(u)
    final_gain = influence_value(v)[0]
    return final_gain - initial_gain

def improved_greedy(beta, T, R):
    H = []
    for i in range(1, n):
        H.append((0, i))
    random.shuffle(H)
    Counter = 0
    last_active_vertex_count = 0
    I = []
    tTh = beta * n
    while last_active_vertex_count < tTh:
        Counter += 1
        if Counter % R == 0:
            UpdatedHeap = []
            temp = [nodedata[e][0] for e in I]
            for t in H:
                UpdatedHeap.append((marginal_gain_function(temp, nodedata[t[1]][0]), t[1]))
            H = UpdatedHeap
            H.sort(reverse=True)
        else:
            A = [H.pop(0)[1] for _ in range(T)]
            temp = [nodedata[e][0] for e in I]
            for u in A:
                H.append((marginal_gain_function(temp, nodedata[u][0]), u))
            H.sort(reverse=True)
            t = H.pop(0)
            I.append(t[1])
            temp.append(nodedata[t[1]][0])
            last_active_vertex_count = influence_value(temp)[1]
    return I

def precalc():
    for i in range(1, MAXN):
        nodedata[i][0] = Node(i, 0, 1.0)
        graph[nodedata[i][0]] = []
    for i in range(1, MAXN):
        for j in range(1, MAXLayers):
            graph[nodedata[i][j]] = []
    for i in range(1, n + 1):
        graph[nodedata[i][0]] = []
    for i in range(1, n + 1):
        for j in range(1, MAXLayers):
            graph[nodedata[i][j]].append((nodedata[i][0], 1.0))
            graph[nodedata[i][0]].append((nodedata[i][j], nodedata[i][j].threshold))

def solve():
    n, mxlayer = map(int, input().split())
    for i in range(1, mxlayer + 1):
        m = int(input())
        for j in range(1, n + 1):
            r, t = map(float, input().split())
            nodedata[int(r)][i] = Node(i, int(r), t)
        for _ in range(m):
            u, v, w = map(float, input().split())
            graph[nodedata[int(u)][i]].append((nodedata[int(v)][i], w))
    for i in range(1, n + 1):
        graph[nodedata[i][0]] = []
    for i in range(1, mxlayer + 1):
        for j in range(1, n + 1):
            graph[nodedata[j][i]].append((nodedata[j][0], 1.0))
            graph[nodedata[j][0]].append((nodedata[j][i], nodedata[j][i].threshold))

precalc()
solve()

