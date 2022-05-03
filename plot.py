import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

def plot_comm(n, path_input, path_result, label):

    # reading file given by path
    f = open(path_result, 'r')
    Lines = f.readlines()
    
    # storing the communities from file to the dictionary `communities`
    print("Communities: ")
    comm_color = {}
    communities = {}
    
    i=0
    for line in Lines:
        a = list(map(int,line.split()))
        print(a)
        communities[i] = a
        i += 1

    count1 = 0
    t=0
    A = []
    for i in range(0, n):
        A.append(list(np.zeros(n, dtype=int)))

    f1 = open(path_input, 'r')
    Lines1 = f1.readlines()
    kk = 0
    for line in Lines1:
        if kk == 0:
            kk = 1
            continue
        a = list(map(int,line.split()))
        if len(a) == 2:
            A[a[0]][a[1]] = 1
    
    f1.close()     
    f.close()

    # graph will be returned and later used in analysis part 
    graph = nx.Graph()
    for i in range(0,n):
        graph.add_node(i)

    for i in range(0,n):
        for j in range(0,n):
            if A[i][j] == 1:
                graph.add_edge(i,j)
                graph.add_edge(j,i)
    
    # set of distinct colors
    node_color = ['#e6194b', '#3cb44b', '#ffe119', '#4363d8', '#f58231', '#911eb4', '#46f0f0', '#f032e6', '#bcf60c', '#fabebe', '#008080', '#e6beff', '#9a6324', '#fffac8', '#800000', '#aaffc3', '#808000', '#ffd8b1', '#000075', '#808080', '#ffffff'] 
    fig, ax = plt.subplots(figsize=(12, 8))
    fig.tight_layout()
    pos = nx.spring_layout(graph, k=0.2, seed=4572321)
    n = graph.number_of_nodes()
    color = ['#ffffff' for i in range(n) ]
    
    # assignig each node color of it's community
    t=0
    vis = {}
    for com in range(len(communities)) :
        for nodes in communities[com]:
            if color[nodes] == '#ffffff':
                color[nodes] = node_color[t]
            elif nodes not in vis:
                color[nodes] = '#000000'
                vis[nodes] = True
        t=t+1
   
    # plotting colored graph using networkx library
    nx.draw_networkx(
        graph,
        ax = ax,
        pos = pos,
        with_labels = True,
        node_color = color,
        node_size = 500,
        edge_color = "#adadad",
        alpha = 0.5,
        font_weight = "bold",
    )

    font = {"color": "k", "fontsize": 20, "weight": "light"}
    ax.set_title(label, font)
    plt.show()
    
    return graph
