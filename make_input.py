# code for creating input file in correct format from gml file of dataset

import networkx as nx
import numpy as np
import random

def file_write(A, m, l, r, f):
    f.write(str(m) + '\n')
    for i in range(l, r+1):
        for j in range(i, r+1):
            if A[i][j] == 1:
                f.write(str(i) + ' ' + str(j) + '\n')
          
def make_input(G, label, T):

    n = G.number_of_nodes()
    m = G.number_of_edges()

    A = []
    for i in range(0, n+1):
        A.append(list(np.zeros(n+1, dtype=int)))

    V = G.nodes()

    path = "inputs/input_" + label + ".txt"
    f = open(path, "w")
    
    l = 1
    r = n

    if 0 in V:
        l = 0
        r = n-1

    for i in V:
        for j in V:
            if G.has_edge(i,j):
                A[i][j]+=1
    
    f.write(str(n) + ' ' + str(T) + '\n')
    file_write(A, m, l, r, f)

    for t in range(1, T):
        changes = random.randint(4, 7)
        for k in range(0, changes):
            while True:
                i = random.randint(l,r)
                j = random.randint(l,r)
                if A[i][j] == 1:
                    A[i][j] -= 1
                    break
            while True:
                i = random.randint(l,r)
                j = random.randint(l,r)
                if A[i][j] == 0:
                    A[i][j] += 1
                    break
        file_write(A, m, l, r, f)

    f.close()
    
    with open(path) as f1:
        with open("test/input.txt", "w") as f2:
            for line in f1:
                f2.write(line)

