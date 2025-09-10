#include <bits/stdc++.h>
using namespace std;
using Move = pair<int, int>;

void printSet(const string& name, const set<int>& s){
    cout << name << ": { ";
    for (int i : s){
        cout << i << " ";
    }
    cout << "}" << endl;
}

class Allocator{
public:
    map<int, int> colors;
    set<int> spills;

    Allocator(int k_val, map<int, set<int>> g, list<Move> m)
        : K(k_val), adj(move(g)), moves(move(m)){
        for (const auto& p : adj){
            nodes.insert(p.first);
        }
    }

    void run(){
        build();
        makeWorklist();

        while (!smplWl.empty() || !coalWl.empty() || !frzWl.empty() || !splWl.empty()){
            if (!smplWl.empty()){
                simplify();
            }
            else if (!coalWl.empty()){
                coalesce();
            }
            else if (!frzWl.empty()){
                freeze();
            }
            else if (!splWl.empty()){
                selectSpill();
            }
        }

        assignColors();
    }

    void report(){
        if (!spills.empty()){
            printSet("Spilled Nodes", spills);
        }
        else{
            cout << "Successful allocation" << endl;
        }
        
        cout << "Register Assignments:" << endl;
        for (const auto& p : colors){
             if (spills.find(p.first) == spills.end()){
                cout << "  Var " << p.first << " -> Reg " << p.second << endl;
            }
        }
    }

private:
    int K;
    set<int> nodes;
    stack<int> stack;
    map<int, set<int>> adj;

    set<int> smplWl;
    set<int> splWl;
    set<int> frzWl;
    list<Move> coalWl;

    list<Move> constr;
    list<Move> frzn;
    list<Move> actv;
    list<Move> moves;

    map<int, int> alias;
    
    void build(){
        coalWl = moves;
    }

    void makeWorklist(){
        for (int n : nodes){
            if (adj[n].size() >= K){
                splWl.insert(n);
            }
            else if (isMoveRelated(n)){
                frzWl.insert(n);
            }
            else{
                smplWl.insert(n);
            }
        }
    }

    void simplify(){
        int n = *smplWl.begin();
        smplWl.erase(smplWl.begin());
        stack.push(n);
        for (int nb : adj[n]){
            decrementDegree(nb);
        }
    }

    void coalesce(){
        auto it = coalWl.begin();
        int u = getAlias(it->first);
        int v = getAlias(it->second);
        
        if (adj.count(v) == 0){
            swap(u, v);
        }

        coalWl.erase(it);

        if (u == v){
            return;
        }

        if (adj[u].count(v) || adj[v].count(u)){
            constr.push_back({u, v});
            addNodeToWorklist(u);
            addNodeToWorklist(v);
            return;
        }

        set<int> comb_nb = adj[u];
        comb_nb.insert(adj[v].begin(), adj[v].end());
        int hi_deg_nb = 0;
        for (int n : comb_nb){
            if (adj[n].size() >= K){
                hi_deg_nb++;
            }
        }

        if (hi_deg_nb < K){
            for (int nb : adj[v]){
                adj[u].insert(nb);
                adj[nb].insert(u);
                adj[nb].erase(v);
                decrementDegree(nb);
            }
            alias[v] = u;
            nodes.erase(v);
            addNodeToWorklist(u);
        }
        else{
            actv.push_back({u, v});
        }
    }

    void freeze(){
        int n = *frzWl.begin();
        frzWl.erase(frzWl.begin());
        smplWl.insert(n);
    }

    void selectSpill(){
        int spill_node = -1;
        int max_deg = -1;
        for (int n : splWl){
            if ((int)adj[n].size() > max_deg){
                max_deg = adj[n].size();
                spill_node = n;
            }
        }

        if (spill_node != -1){
            splWl.erase(spill_node);
            smplWl.insert(spill_node);
        }
    }

    void assignColors(){
        while (!stack.empty()){
            int n = stack.top();
            stack.pop();

            set<int> avail;
            for (int i = 1; i <= K; ++i){
                avail.insert(i);
            }

            for (int nb : adj[n]){
                int alias_nb = getAlias(nb);
                if (colors.count(alias_nb)){
                    avail.erase(colors[alias_nb]);
                }
            }
            
            if (avail.empty()){
                spills.insert(n);
            }
            else{
                colors[n] = *avail.begin();
            }
        }
        
        for (const auto& p : alias){
            colors[p.first] = colors[getAlias(p.second)];
        }
    }

    void decrementDegree(int n){
        if (nodes.find(n) == nodes.end()){
            return;
        }

        int old_deg = adj[n].size();
        int new_deg = old_deg - 1; 

        if (old_deg >= K && new_deg < K){
            splWl.erase(n);
            if (isMoveRelated(n)){
                frzWl.insert(n);
            }
            else{
                smplWl.insert(n);
            }
        }
    }
    
    bool isMoveRelated(int n){
        for (const auto& m : coalWl){
            if (getAlias(m.first) == n || getAlias(m.second) == n){
                return true;
            }
        }
        return false;
    }
    
    int getAlias(int n){
        if (alias.count(n)){
            return alias[n] = getAlias(alias[n]);
        }
        return n;
    }
    
    void addNodeToWorklist(int n){
        if (adj[n].size() >= K){
            splWl.insert(n);
        }
        else if (isMoveRelated(n)){
            frzWl.insert(n);
        }
        else{
            smplWl.insert(n);
        }
    }
};

