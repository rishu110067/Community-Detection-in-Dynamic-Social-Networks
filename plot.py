# code for coloring and plotting communities of graph

import matplotlib.pyplot as plt
import networkx as nx

def plot_comm(graph, path, label):

    # reading file given by path
    f = open(path, 'r')
    Lines = f.readlines()
    
    # storing the communities from file to the dictionary `communities`
    # and assignig each node color of it's community
    print("Communities: ")
    comm_color = {}
    communities = {}
    i=0
    for line in Lines:
        a = list(map(int,line.split()))
        communities[i] = a
        print(a)
        for node in a:
            comm_color[node] = i
        i = i+1

    # assigning color to nodes not in any community
    print("Nodes not in any community:", end = " ")
    for node in graph:
        if node not in comm_color:
            comm_color[node] = i
            print(node, end = " ")
        
    # setting parameters for plotting the colored graph
    fig, ax = plt.subplots(figsize=(12, 8))
    fig.tight_layout()
    node_color = [comm_color[node] for node in graph]
    pos = nx.spring_layout(graph, k=0.2, seed=4572321)

    # plotting the colored graph
    nx.draw_networkx(
        graph,
        ax = ax,
        pos = pos,
        with_labels = True,
        node_color = node_color,
        node_size = 500,
        edge_color = "#adadad",
        alpha = 0.4,
        font_weight = "bold",
    )

    font = {"color": "k", "fontsize": 20, "weight": "light"}
    ax.set_title(label, font)
    plt.show()
