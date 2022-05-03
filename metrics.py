import numpy as np
import networkx as nx

def overlapping_permanence(graph):
    communities={}
    i=0
    f = open('test/output.txt', 'r')
    Lines = f.readlines()
    n = graph.number_of_nodes()

    comm = [0 for i in range(n)]

    for line in Lines:
        a = list(map(int,line.split()))
        communities[i]=a
        for nodes in a:
            comm[nodes] += 1
        i=i+1
    
    ic = []
    for i in range(0, len(communities)):
        ic.append(list(np.zeros(n, dtype=int)))

    for i in range(len(communities)):
        for v1 in communities[i]:
            for v2 in communities[i]:
                if v1==v2:
                    continue
                if graph.has_edge(v1,v2):
                    ic[i][v1] += 1/(comm[v2])
    

    Emax = [0 for i in range(n)]
    mark = [0 for i in range(n)]

    for v in range(n):
        maxx = 0
        for i in range(len(communities)):
            cnt = 0
            if v not in communities[i]:
                for nodes in communities[i]:
                    if graph.has_edge(v,nodes):
                        cnt += 1
            maxx = max(maxx,cnt)
        if maxx == 0:
            mark[v] = 1
        else:
            Emax[v] = maxx
    

    Cin = []
    for i in range(0, len(communities)):
        Cin.append(list(np.zeros(n, dtype=int)))
    
    deg = [0 for i in range(n)]

    for v in range(n):
        deg[v] = graph.degree[v]

    for i in range(len(communities)):
        for v in communities[i]:
            cnt = 0
            indeg = 0
            for v1 in communities[i]:
                if graph.has_edge(v,v1):
                    indeg += 1
                    for v2 in range(n):
                        if graph.has_edge(v1,v2) and (v2 in communities[i]) and graph.has_edge(v,v2):
                            cnt += 1
            if indeg > 1:
                # print(indeg,cnt,v)
                Cin[i][v] = (cnt)/(indeg * (indeg-1))


    I = [0.00 for i in range(n)]
    mark1 = []
    for i in range(0, n):
        mark1.append(list(np.zeros(n, dtype=int)))

    for i in range(len(communities)):
        for v1 in communities[i]:
            for v2 in communities[i]:
                if v1==v2:
                    continue
                if graph.has_edge(v1,v2) and mark1[v1][v2] == 0:
                    I[v1] += 1.00
                    mark1[v1][v2] = 1
    
    # print(deg)
    # print(I)
    # print(Emax)
    # print(Cin)
    # print(ic)
    oPerm = [0 for i in range(n)]

    for i in range(len(communities)):
        for v in communities[i]:
            if mark[v] == 0 and I[v] > 0.00:
                oPerm[v] += ((ic[i][v]/Emax[v]) * (1.00/deg[v])) - ((1.00-Cin[i][v])*(ic[i][v]/I[v]))
            else:
                oPerm[v] += Cin[i][v]

    print(oPerm)
    final_oPerm = sum(oPerm) / (n)

    print("Overlapping permanence: ", final_oPerm)
