/**
 * Author: Benjamin Qi, Oleksandr Kulkov, chilli
 * Date: 2020-01-12
 * License: CC0
 * Source: https://codeforces.com/blog/entry/53170, https://github.com/bqi343/USACO/blob/master/Implementations/content/graphs%20(12)/Trees%20(10)/HLD%20(10.3).h
 * Description: Decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. Code does additive modifications and max queries, but can
 * support commutative segtree modifications/queries on paths and subtrees.
 * Takes as input the full adjacency list. VALS\_EDGES being true means that
 * values are stored in the edges, as opposed to the nodes. All values
 * initialized to the segtree default. Root must be 0.
 * Time: O((\log N)^2)
 * Status: stress-tested against old HLD
 */
#pragma once

#include "../data-structures/LazySegmentTree.h"

struct HLD {
    vector<int> parent,depth,size,heavy,head,pos;
    HLD(const vector<vector<int>>& g,int root=0) {
        int n=g.size(); parent.assign(n,-1); depth.assign(n,0);
        size.assign(n,1); heavy.assign(n,-1);
        head.resize(n); pos.resize(n);
        vector<int> order={root}; parent[root]=root;
        for(int i=0;i<n;++i) {
            int u=order[i];
            for(int v:g[u]) if(parent[v]==-1) {
                parent[v]=u; depth[v]=depth[u]+1; order.push_back(v);
            }
        }
        for(int i=n-1;i>0;--i) {
            int u=order[i],p=parent[u]; size[p]+=size[u];
            if(heavy[p]==-1 || size[u]>size[heavy[p]]) heavy[p]=u;
        }
        int timer=0;
        vector<pair<int,int>> tasks={{root,root}};
        while(!tasks.empty()) {
            auto [start,h]=tasks.back(); tasks.pop_back();
            for(int u=start;u!=-1;u=heavy[u]) {
                head[u]=h; pos[u]=timer++;
                for(int v:g[u]) if(parent[v]==u && v!=heavy[u])
                    tasks.push_back({v,v});
            }
        }
    }
    ll pathSum(int u,int v,const SegTree& st) const {
        ll answer=0;
        while(head[u]!=head[v]) {
            if(depth[head[u]]<depth[head[v]]) swap(u,v);
            answer+=st.query(pos[head[u]],pos[u]+1);
            u=parent[head[u]];
        }
        if(depth[u]>depth[v]) swap(u,v);
        return answer+st.query(pos[u],pos[v]+1);
    }
};
