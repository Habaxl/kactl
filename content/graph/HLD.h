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
// Cay con u lien tiep: [pos[u],pos[u]+size[u]).
// Cap nhat duong di: tach cung cac doan, dung LazySeg muc 06.
// Tong/min/max khong phu thuoc thu tu. Noi chuoi/ma tran can
// xu ly huong hai nua duong di, KHONG doi phep cong may moc.
