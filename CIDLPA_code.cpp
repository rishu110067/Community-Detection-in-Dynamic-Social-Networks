// Cascade Information Diffusion based Label Propagation Algorithm(CIDLPA) for Community Detection in Dynamic Graphs

/*

Input Format
N T

M1
x1 - y2
x2 - y2
x3 - y3
.
.

M2
x1 - y1
x2 - y2
x3 - y3
.
.
....

*/


#include <bits/stdc++.h>
using namespace std;

const int N = 10000;
const int T = 100;          // setting maximum no. of nodes 10000 and max no. of timestamps 100
set<int> adj[T][N];         // adj[T] is adjacency list representation of graph at timestamp t
set<pair<int,int>> edge[T]; // edge[t] stores the edge list for timestamp t
set<int> G[T];              // G[t] is vertex set of nodes of Graph at timestamp t
map<int,double> Label[N];   // Label[x] is labelset of node x
                            // label[x][l] tells belonging factor of node x in community l
map<int,double> b[T][N];    // b[t][x][l] tells belonging factor of node x in community l at timestamp t
double S[2][N];             // S[0][x] and S[1][x] tells S0 and S1 belonging of node x


// finds change in vertex set from timestamp t1 to t2
set<int> v_change(int t1, int t2)
{
    set<int> s;
    for(auto x: G[t1])
    {
        if(G[t2].find(x) == G[t2].end())
            s.insert(x);
    }
    return s;
}

// finds change in edge set from timestamp t to t+1 
set<pair<int,int>> e_change(int t)
{
    set<pair<int,int>> s;
    for(auto e: edge[t])
    {
        if(edge[t+1].find(e) == edge[t+1].end())
            s.insert(e);
    }
    for(auto e: edge[t+1])
    {
        if(edge[t].find(e) == edge[t].end())
            s.insert(e);
    }
    return s;
}

// finds value strength (i -> j)
double find_strength(int i, int j, int t)
{
    int set_div = 0;
    for(auto x: adj[t][j])
    {
        if(x!=i && adj[t][i].find(x) == adj[t][i].end())
            set_div++;
    }
    double val = (double)set_div/adj[t][j].size();
    return val;
}

// finds value strength from x to all neighbours of x  
vector<double> cal_strength(int x, vector<int> neighb, int t)
{
    vector<double> strength;
    for(auto i: neighb)
    {
        double val = find_strength(i, x, t);
        strength.push_back(val);
    }
    return strength;
}

// finds s1 and s0 belonging based on the value strengths
void find_belonging(int i, vector<double> &strength)
{
    double sum_strength = 0;
    for(auto it:strength) sum_strength+=it;
    S[1][i] = sum_strength/strength.size();
    S[0][i] = 1.00 - S[1][i];
}

// finds vertex/nodes set given edge set e
set<int> find_nodes(set<pair<int,int>> e)
{
    set<int> nodes;
    for(auto it: e)
    {
        auto x = it.first;
        auto y = it.second;
        nodes.insert(x);
        nodes.insert(y);
    }
    return nodes;
}

// finds candidate label set for each neighbour node
vector<int> get_labels(vector<int> &neighb)
{
    vector<int> labels;
    for(auto x: neighb) {
        double mx_bf = 0;
        int mx_label = x;
        for(auto it: Label[x]) {
            auto l = it.first;
            auto bf = it.second;
            if(bf > mx_bf) {
                mx_label = l;
                mx_bf = bf;
            }
        }
        labels.push_back(mx_label);
    }
    return labels;
}

// finds vote for each node in candidate label set
vector<double> compute_vote(vector<int> &candidateLabels, vector<int> &neighb)
{
    // each neighbour chooses a label and candidateLabels set and votes it
    
    vector<double> vote;
    for(int i = 0; i < candidateLabels.size(); i++)
    {
        double v = 0.00;
        for(int k = 0; k < neighb.size(); k++) {
            int j = neighb[k];
            int sl = candidateLabels[i];
            if(Label[j].find(sl) != Label[j].end()) {
                v += (double)S[0][j] * Label[j][sl] + (double)S[1][j] * ((1 - Label[j][sl]) / 3.0);
            }
        }
        vote.push_back(v);
    }
    return vote;
}

// returns the label in candidate label set with the maximum vote
int get_maximum_vote(vector<double> &vote, vector<int> &candidateLabels)
{
    double mx_vote = 0;
    int mx_vote_label;
    for(int j = 0; j < vote.size(); j++)
    {
        if(vote[j] > mx_vote)
        {
            mx_vote = vote[j];
            mx_vote_label = candidateLabels[j];
        }
    }
    return mx_vote_label;
}

// normalizes and updates the belonging factor of label set of node x
void normalize(int x, int t)
{
    vector<pair<int,int>> remove;
    for(auto it: Label[x])
    {
        auto c = it.first;
        auto bf = it.second;
        
        // Finding new belonging factor for the next timestamp
        double new_bf = 0;
        for(auto y: adj[t][x])
        {
            // If c community is present in the label set of y and has non zero belongiong factor
            if(b[t][y].find(c) != b[t][y].end())
                new_bf += b[t][y][c];
        }

        new_bf /= adj[t][x].size();
        Label[x][c] = new_bf;
        b[t+1][x][c] = new_bf;
        if(new_bf == 0.00)
        {
            // If new belonging factor is 0 then we will remove c from label set of x
            remove.push_back({x,c});
        }
    }
    
    // Removing all the communities with new belonging factor 0 from their corresponding node's label set
    for(auto it: remove)
    {
        auto x = it.first;
        auto c = it.second;
        Label[x].erase(c);
        b[t+1][x].erase(c);
    }

    // Normalize sum of bf to 1
    double sum = 0;
    for(auto it: b[t+1][x])
    {
        auto l = it.first;
        auto bf = it.second;
        sum += bf;
    }
    if(sum == 0){ Label[x][x] = 1; b[t+1][x][x] = 1; }
    else
    {
        double add_val = (1.00 - sum)/(b[t+1][x].size());
        for(auto it: b[t+1][x])
        {
            auto l = it.first;
            auto bf = it.second;
            b[t+1][x][l] += add_val;
            Label[x][l] = b[t+1][x][l];
        }
    }
}

// remove labels with belonging factor less than threshold r in the label set of each node
void remove_labels(int t, int r, set<int> &set_changedNodes)
{
    for(auto x: set_changedNodes)
    {
        // Remove labels with belonging factor less than r
        vector<int> remove;
        double sum = 0;
        int mx_label = x;
        double mx_bf = 0;
        for(auto it: Label[x])
        {
            auto l = it.first;
            auto bf = it.second;
            if(bf < r)
            {
                remove.push_back(l);
                Label[x].erase(l);
                b[t+1][x].erase(l);
                sum += bf;
                if(bf > mx_bf)
                {
                    mx_bf = bf;
                    mx_label =l;
                }
            }
        }

        // If label set of node x becomes empty then pick the label with max belonging factor removed, and set it bf = 1
        if(Label[x].empty())
        {
            Label[x][mx_label] = 1.00;
            b[t+1][x][mx_label] = 1.00;
        }
        else
        {
            // Add the val to belonging factor of all the labels of node x remaining so that sum of bf of labels remain 1
            double val = (1.00-sum) / Label[x].size();
            for(auto it: Label[x])
            {
                auto l = it.first;
                auto bf = it.second;
                Label[x][l] += val;
                b[t+1][x][l] += val;
            }
        }
    }
}


int main()
{
    // Files for taking input and printing output
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    // Input: n = no of nodes, ts = no of timestamps
    int n, ts;
    double r = 0.5;
    cin >> n >> ts;
    for(int t = 0; t < ts; t++)
    {
        int m;
        cin >> m;
        for(int i = 0; i < m; i++)
        {
            int x, y;
            cin >> x >> y;
            edge[t].insert({x, y});
            adj[t][x].insert(y);
            adj[t][y].insert(x);
            G[t].insert(x);
            G[t].insert(y);
            if(t == ts-1)
            {
                edge[ts].insert({x, y});
                adj[ts][x].insert(y);
                adj[ts][y].insert(x);
                G[ts].insert(x);
                G[ts].insert(y);
            }
        }
    }
    ts++;

    
    // Part 1: Initialization
    set<int> v = G[0];
    for(int t = 0; t < ts; t++)
    {
        for(auto x: v)
        {
            Label[x][x] = 1;
            b[t][x][x] = 1;
        }
        if(t != ts-1) v = v_change(t+1, t);
    }


    // Part 2: CID 
    v = G[0];
    for(int t = 0; t < ts; t++)
    {
        for(auto x: v)
        {
            vector<int> neighb;
            for(auto i: adj[t][x]) neighb.push_back(i);
            vector<double> strength = cal_strength(x, neighb, t);
            find_belonging(x, strength);
        }
        if(t != ts-1) v = v_change(t+1, t);
    }


    // Part 3: Label Propagation
    set<pair<int,int>> e = edge[0];
    set<int> set_changedNodes = find_nodes(e);
    for(int t = 0; t < ts; t++)
    {
        set<int> set_changedNodes = find_nodes(e);
        set<int> Vold;
        if(t!=ts-1) Vold = v_change(t, t+1);
        for(auto x: Vold) set_changedNodes.erase(x);

        for(int it = 0; it < ts; it++)
        {
            vector<int> changedNodes;
            for(auto x: set_changedNodes) changedNodes.push_back(x);
            shuffle(changedNodes.begin(), changedNodes.end(), default_random_engine(0));

            for(auto x: changedNodes)
            {
                vector<int> neighb;
                for(auto i: adj[t][x]) neighb.push_back(i);
                vector<int> candidateLabels = get_labels(neighb);
                vector<double> vote = compute_vote(candidateLabels, neighb);
                int mx_vote_label = get_maximum_vote(vote, candidateLabels);
                if(Label[x].find(mx_vote_label) == Label[x].end())
                    Label[x][mx_vote_label] = 0;
                normalize(x,t);
            }
        }
        remove_labels(t, r, set_changedNodes);
    }
    
  
    // Findig and Printing Result

    // Finding communities from label
    set<int> res[n+1];
    for(int i = 0; i <= n; i++)
    {
        for(auto it: Label[i])
        {
            auto l = it.first;
            auto bf = it.second;
            res[l].insert(i);
        }
    }

    // Storing communities
    vector<set<int>> comm_set;
    for(int i = 0; i <= n; i++)
    {
        if(res[i].size() == 0) continue;
        set<int> s;
        for(auto x: res[i]) s.insert(x);
        comm_set.push_back(s);
    }

    // Removing communities which are subset of other community
    vector<set<int>> communities;
    for(int i = 0; i < comm_set.size(); i++)
    {
        int is_subset = 0;
        for(int j = 0; j < comm_set.size(); j++)
        {
            if(i == j) continue;
            // check if community i is present in community j
            is_subset = 1;
            for(auto x: comm_set[i])
            {
                if(comm_set[j].find(x) == comm_set[j].end())
                {
                    is_subset = 0;
                    break;
                }
            }
            if(is_subset) break;
        }
        if(!is_subset) communities.push_back(comm_set[i]);
    }

    // Output
    for(auto s: communities)
    {
        for(auto x: s) cout << x << " ";
        cout << endl;
    }
    
    return 0;
}

