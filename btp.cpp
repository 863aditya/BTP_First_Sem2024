#include<bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
//typedef __int128 li;
#define f(i,a,b) for(ll i=a;i<b;i++)
#define w(t) while(t--)
#define IOS ios::sync_with_stdio(0);cin.tie(0);
#define all(x) (x).begin(),(x).end()
#define pb push_back
#define ff first
#define ss second
#define sz(a) (ll)a.size()
#define fa(ele, x) for (auto ele : x)
#define ld long double
#define enl  "\n"
#define sp " "
#define mem1(a) memset(a,-1,sizeof(a))
#define mem0(a) memset(a,0,sizeof(a))

#ifdef LOCAL
#include "debug.h"
#else
#define dbg(...) 4000
#endif

const int N = 2e5 + 5;
const ll mod = 1e9 + 7;
template <typename A, typename B> ostream& operator<< (ostream &cout, pair<A, B> const &p) { return cout << "(" << p.ff << ", " << p.ss << ")"; }
template <typename A, typename B> istream& operator>> (istream& cin, pair<A, B> &p) {cin >> p.ff; return cin >> p.ss;}
template <typename A> ostream& operator<< (ostream &cout, vector<A> const &v) {cout << "["; for(int i = 0; i < (int)v.size(); i++) {if (i) cout << ", "; cout << v[i];} return cout << "]";}
template <typename A> istream& operator>> (istream& cin, vector<A> &x){for(int i = 0; i < (int) x.size()-1; i++) cin >> x[i]; return cin >> x[x.size()-1];}
template <typename A, typename B> A amax (A &a, B b){ if (b > a) a = b ; return a; }
template <typename A, typename B> A amin (A &a, B b){ if (b < a) a = b ; return a; }
// template<typename... Args>void see(Args&... args){((cin >> args), ...);}
// template<typename... Args>void out(Args&... args){((cout << args << ""), ...);}
// template<typename... Args>void err(Args&... args){((cerr << args << " "), ...);}
typedef tree<int, null_type, less<int>, rb_tree_tag,tree_order_statistics_node_update> ordered_set;
clock_t TSTART = clock();
auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 mt(seed);
int myrand(int mod) {return mt()%mod;}
void setIO(string fname="input"){
    IOS;
    freopen((fname+".in").c_str(),"r",stdin);
    freopen((fname+".out").c_str(),"w",stdout);
    freopen("err.txt","w",stderr);
}
const int MAXN=1e4,MAXLayers=10;
using db = double;
ll n;
map<pair<pair<ll, ll>, double>,vector<pair<pair<pair<ll, ll>, double>, double>>>graph;
pair<pair<ll, ll>, double>nodeData[MAXN][MAXLayers];
pair<db,int> influenceValue(vector<pair<pair<ll, ll>, double>>&v){
    queue<pair<pair<ll, ll>, double>>q;
    // fa(ele,v){
    //     cerr<<"bhola\n";
    //     cerr<<ele.number<<sp;
    // }cerr<<enl;
    // map<pair<pair<pair<ll, ll>, double> ,db>curInfluenceValue;
    map<pair<pair<ll, ll>, double>, double>curInfluenceValue;
    map<pair<pair<ll, ll>, double>,db>vis;
    for(auto &e:v){
        q.push(e);
        vis[e]=true;
    }
    // cerr<<"queue printing\n";
    while(!q.empty()){
        pair<pair<ll, ll>, double> v=q.front();
        q.pop();
        vis[v]=true;
        // cerr<<v<<enl;
        // cerr<<"curNode "<<v.number<<sp<<v.ff.ss<<sp<<v.ss<<enl;
        for(auto x:graph[v]){
            curInfluenceValue[x.ff]+=x.ss;
            if(curInfluenceValue[x.ff]>=x.ff.ss && !vis[x.ff]){
                q.push(x.ff);
                // dbg(x.ff);dbg(curInfluenceValue[x.ff]);

                // cerr<<x.ff.number<<sp<<x.ff.threshold<<sp<<curInfluenceValue[x.ff]<<sp<<x.ss<<enl;
                vis[x.ff]=true;
            }
        }
    }
    // dbg(curInfluenceValue);
    // cerr<<"\n";
    set<ll>s;
    db totalInfluenceValue=0;
    int activeVertexCount=0;
    for(auto e:vis){
        if(e.ss){
            // activeVertexCount+=1;
            s.insert(e.ff.ff.ff);
            for(auto u:graph[e.ff]){
                totalInfluenceValue+=u.ss;
            }
        }
    }
    activeVertexCount=sz(s);
    // dbg(s);
    // dbg(activeVertexCount);
    db factor=2.0;totalInfluenceValue/=factor;
    return {totalInfluenceValue,activeVertexCount};
}

db marginalGainFunction(vector<pair<pair<ll, ll>, double>>v,pair<pair<ll, ll>, double> u){
    db initialGain=influenceValue(v).ff;
    v.pb(u);
    // fa(ele,v){
    //     cerr<<"hola\n";
    //     cerr<<ele.number<<sp;
    // }cerr<<enl;
    db finalGain=influenceValue(v).ff;
    return finalGain-initialGain;
}

void debugHeap(priority_queue<pair<db,int>>H){
    cerr<<"[\n";
    while(!H.empty()){
        cerr<<H.top()<<enl;
        H.pop();
    }
    cerr<<"]\n";
}

vector<int> ImprovedGreedy(db beta,int T,int R){
    priority_queue<pair<db,int>>H;
    for(int i=1;i<n;i++){
        H.push({0,i});
    }
    int Counter=0;
    int lastActiveVertexCount=0;
    vector<int>I;
    db tTh=beta*n;dbg(tTh);
    while(lastActiveVertexCount<tTh){
        Counter+=1;
        if(Counter%R==0){
            priority_queue<pair<db,int>>UpdatedHeap;
            vector<pair<pair<ll, ll>, double>>temp;
            for(auto e:I){
                temp.pb(nodeData[e][0]);
            }
            while(!H.empty()){
                pair<db,int>t=H.top();
                H.pop();
                UpdatedHeap.push({marginalGainFunction(temp,nodeData[t.ss][0]),t.ss});
            }
            H=UpdatedHeap;
        }
        else{
            vector<int>A;
            for(int i=0;i<T;i++){
                A.pb(H.top().ss);
                H.pop();
            }
            vector<pair<pair<ll, ll>, double>>temp;
            for(auto e:I)temp.pb(nodeData[e][0]);
            for(auto u:A){
                // dbg(marginalGainFunction(temp,nodeData[u]))
                H.push({marginalGainFunction(temp,nodeData[u][0]),u});
            }
            // debugHeap(H);
            // dbg(A);
            pair<db,int>t=H.top();H.pop();
            I.pb(t.ss);
            temp.pb(nodeData[t.ss][0]);
            lastActiveVertexCount=influenceValue(temp).ss;
        }
        // dbg(I);//4000;
        dbg(lastActiveVertexCount);
    }
    cerr<<enl;
    return I;
}

void precalc(){
    f(i,1,MAXN){
        // nodedata
        nodeData[i][0].ff.ff=i,nodeData[i][0].ff.ss=0,nodeData[i][0].ss=1.0;
        graph[nodeData[i][0]].clear();
    }
}

void solve(bool ttt=false){
    //PROOF THE ALGO THEN START
    int mxlayer;//see(n,mxlayer);
    cin>>n>>mxlayer;
    // n=100,mxlayer=1;
    // fa(ele,graph[nodedata[99][0]]){
    //     cerr<<ele.ff.number<<sp<<ele.ss<<enl;
    // }
    // cerr<<n<<sp<<mxlayer<<enl;
    for(int i=1;i<=mxlayer;i++){
        int m;//see(m);
        cin>>m;
        f(j,1,n+1){
            //number, threshold
            ll r;db t;//see(r,t);
            cin>>r>>t;
            // cerr<<r<<sp<<t<<enl;
            // cerr<<t<<enl;
            // if(t==0)t=1.0;
            nodeData[r][i].ff.ff=r;nodeData[r][i].ff.ss=i;nodeData[r][i].ss=t;
            // cerr<<r<<sp<<t<<sp<<nodeData[r][i].ss<<sp<<nodeData[r][i].ff.ff<<sp<<nodeData[r][i].ff.ss<<enl;
        }
        // cerr<<"h "<< sz(graph[nodeData[99][0]])<<enl;
        //edges
        f(j,0,m){
            ll u,v;db w;//see(u,v,w);
            cin>>u>>v>>w;
            // cerr<<u<<sp<<v<<sp<<w<<enl;
            // if(u==45583)w=0;
            // cerr<<nodeData[u][i].ss <<sp<<nodeData[v][i].ss <<sp<<w<<enl;
            graph[nodeData[u][i]].pb({nodeData[v][i],w});
        }
        // cerr<<"h "<< sz(graph[nodeData[99][0]])<<enl;
    }
    // cerr<<"h "<< sz(graph[nodeData[99][0]])<<enl;
    f(i,1,n+1){
        graph[nodeData[i][0]].clear();
    }
    // cerr<<"h "<< sz(graph[nodeData[99][0]])<<enl;
    // f(i,1,n+1){
    // 	nodeData[i][0].ff.ff=i,nodeData[i][0].ff.ss=1,nodeData[i][0].ss=1.0;
    // 	cerr<<nodeData[i][0]<<enl;
    // }
    f(i,1,mxlayer+1){
        f(j,1,n+1){
            graph[nodeData[j][i]].pb({nodeData[j][0],1.0});
            graph[nodeData[j][0]].pb({nodeData[j][i],nodeData[j][i].ss});
        }
    }
    f(i,1,mxlayer+1){
    	f(j,1,n+1){
    		db total=0;
    		fa(ele,graph[nodeData[j][i]]){
    			total+=ele.ss;
    		}
    		for(auto &ele:graph[nodeData[j][i]]){
    			ele.ss/=total;
    		}
    	}
    }
    // cerr<<nodeData[1][0]<<enl;
    // fa(ele,graph[nodeData[1][0]]){
    //     cerr<<ele.ff.ff<<sp<<ele.ff.ss<<sp<<ele.ss<<enl;
    // }
    // f(i,1,mxlayer+1){
    //     f(j,1,n+1){
    //         db total=0;
    //         fa(ele,graph[nodeData[j][i]]){
    //             total+=ele.ss;
    //         }
    //         for(auto &ele:graph[nodeData[j][i]]){
    //             ele.ss/=total;
    //         }
    //     }
    // }
    // f(i,1,n+1){
    //     // cerr<<sz(graph[nodeData[i][0]])<<enl;
    //     // fa(ele,graph[nodeData[i][0]]){
    //     //     cerr<<ele.ff.ff.ff<<sp<<ele.ss<<enl;
    //     // }
    // }
    // fa(ele,graph){
    //     cerr<<ele.ff.ff.ff<<": ";
    //     fa(x,ele.ss){
    //         cerr<<x.ff.ff.ff<<sp<<x.ss<<enl;
    //     }
    // }
    // fa(ele,graph[nodeData[1][1]]){
    // 	cerr<<ele<<enl;
    // }
    // cout<<ImprovedGreedy(0.6,5,4)<<enl;
    for(db x=0.1;x<=0.6;x+=0.1){
        cout<<sz(ImprovedGreedy(x,(0.1)*n,(0.2)*n))<<enl;
    }
}

int main(){
    IOS;
    // #ifndef ONLINE_JUDGE
    // setIO();
    // #endif
    ll Tests=1;
    // cin>>Tests;
    precalc();
    for(int i=1;i<=Tests;i++){
        solve();
    }
    cerr<<(double)(clock() - TSTART)/CLOCKS_PER_SEC<<"\n";
    return 0;
}