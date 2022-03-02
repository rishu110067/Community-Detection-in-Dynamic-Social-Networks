// Cascade Information Diffusion based Label Propagation Algorithm(CIDLPA) for Community Detection in Dynamic Graphs
// Normalize and remove labels functions need attention
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

7 1
10
1 2
1 4
2 3
2 4
3 4
1 5
1 7
5 6
5 7
6 7


*/

#include<bits/stdc++.h>
using namespace std;

const int N = 1000, T = 100;
set<int> adj[T][N];
set<pair<int,int>> edge[T];
set<int> G[T];
map<int,double> Label[N];
map<int,double> b[T][N];
double S[2][N];

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


double find_strength(int i, int j, int t)
{
    int set_div = 0;
    for(auto x: adj[t][i])
    {
        if(x!=j && adj[t][j].find(x) == adj[t][j].end())
            set_div++;
    }
    double val = (double)set_div/adj[t][j].size();
    return val;
}

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

void find_belonging(int i, vector<double> &strength)
{
    double sum_strength = 0;
    for(auto it:strength) sum_strength+=it;
    S[1][i] = sum_strength/strength.size();
    S[0][i] = 1.00 - S[1][i];
}

set<int> find_nodes(set<pair<int,int>> e)
{
    set<int> nodes;
    for(auto &[x,y]: e)
    {
        nodes.insert(x);
        nodes.insert(y);
    }
    return nodes;
}

vector<int> get_labels(vector<int> &neighb)
{
    vector<int> labels;
    for(auto x: neighb)
    {
        double mx_bf = 0;
        int mx_label = x;
        for(auto &[l,bf]: Label[x])
        {
            if(bf > mx_bf)
            {
                mx_label = l;
                mx_bf = bf;
            }
        }
        labels.push_back(mx_label);
    }
    return labels;
}

vector<double> compute_vote(vector<int> &candidateLabels, vector<int> &neighb)
{
    vector<double> vote;
    for(int i = 0; i < neighb.size(); i++)
    {
        int j = neighb[i];
        int sl = candidateLabels[i];
        double v = (double)S[0][j] * Label[j][sl] + (double)S[1][j] * (1 - Label[j][sl]) / 3.0;
        vote.push_back(v);
    }
    return vote;
}

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

void normalize(int x, int t)
{
    for(auto &[c,bf]: Label[x])
    {
        double new_bf = 0;
        for(auto y: adj[t][x])
        {
            new_bf += b[t][c][y];
        }
        new_bf /= adj[t][x].size();
        Label[x][c] = new_bf;
        b[t+1][c][x] = new_bf;
    }
}

void remove_labels(int t, int r, set<int> &set_changedNodes)
{
    for(auto x: set_changedNodes)
    {
        vector<int> remove;
        double sum = 0;
        for(auto &[l,bf]: Label[x])
        {
            if(bf < r){ remove.push_back(l); sum+=bf; }
        }

        for(auto l: remove){  Label[x].erase(l); b[t+1][l][x]=0.00; }
        if(remove.size()==Label[x].size()){  /// random or max
            srand(time(0));
            int l =  rand()%((int)Label[x].size());
            Label[x][remove[l]]=1.00;
            b[t+1][remove[l]][x]=1.00;
        }
        else{
            int val = sum/remove.size();
            for(auto &[l,bf]: Label[x])
            {
                Label[x][l]+=val;
                b[t+1][l][x]+=val;
            }
        }
    }
}


int main()
{
    // Input
    int n, ts, r = 0.5;
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
        }
    }

    // Part 1
    set<int> v = G[0];
    for(int t = 0; t < ts; t++)
    {
        for(auto x: v)
        {
            Label[x][x] = 1;
            b[0][x][x] = 1;
        }
        if(t != ts-1) v = v_change(t+1, t);
    }

    // Part 2
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

    for(int i=1;i<=n;i++) cout<<S[0][i]<<" "<<S[1][i]<<endl;


    // Part 3
    set<pair<int,int>> e = edge[0];
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
            for(auto it:changedNodes) cout<<it<<" ";
            for(auto x: changedNodes)
            {
                vector<int> neighb;
                for(auto i: adj[t][x]) neighb.push_back(i);
                vector<int> candidateLabels = get_labels(neighb);
                vector<double> vote = compute_vote(candidateLabels, neighb);
                int mx_vote_label = get_maximum_vote(vote, candidateLabels);
                Label[x][mx_vote_label] = 0;
                normalize(x,t);
            }
        }
        remove_labels(t, r, set_changedNodes);
        if(t != ts-1) e = e_change(t);
    }

    // Output
    set<int> res[n+1];
    for(int i = 0; i <= n; i++)
    {
        for(auto &[l, bf]: Label[i])
        {
            res[l].insert(i);
        }
    }

    for(int i = 0; i <= n; i++)
    {
        if(res[i].size() == 0) continue;
        cout << "Community " << i << " -> ";
        for(auto x: res[i]) cout << x << " ";
        cout << endl;
    }

    return 0;
}
