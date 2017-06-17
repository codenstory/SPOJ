#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
using namespace std;

#define MX 300
#define INF 100000000

int n;
bool vis[MX];
int cap[MX][MX];
vector<int> ver[MX];


int aug(int source,int sink)
{
	int where,path_cap,next,prev,chk;
	queue<int> Q;
	Q.push(source);
	vis[source]=true;	
	int from[MX]={-1};
	chk=0;
	while(!Q.empty())
	{
		where = Q.front();
		Q.pop(); 
		for(int i=0;i<ver[where].size();i++)	
		{	
			next=ver[where][i];
			if(!vis[next] && cap[where][next]>0)
			{
				Q.push(next);
				vis[next]=true; 
				from[next] = where;
				if(next==sink)
				{
					chk=1;
					break;
				}
			}
		}
		if(chk)
			break;
	}
	where = sink;
	path_cap = INF;
	while(from[where] > -1)
	{
		prev = from[where];
		path_cap = min(path_cap, cap[prev][where]);
		where = prev;
	}
	if(where==source)
	{
		where = sink;
		while(from[where] > -1)
		{
			prev = from[where];
			cap[prev][where] -= path_cap;
			cap[where][prev] += path_cap;
			where = prev;
		}
	}
	if(path_cap==INF)
		return 0;
	else 
		return path_cap;
}



int maxFlow(int S,int T)
{
	int flow = 0;
	memset(vis,false,sizeof vis);
	while(1)
	{
		int f = aug(S,T);
		if(f==0) break;
		flow+=f;
		memset(vis,false,sizeof vis);
	}
	return flow;
}



int main()
{
	char a,b;
	int c;
	while ( 1 == scanf("%d",&n) ) {
	memset(cap,0,sizeof cap);
	n+=2;
	for(int i=0;i<n-2;i++)
	{
		scanf("%d",&c);
		cap[0][i+1]+=c;
		ver[0].push_back(i+1);
	}
	for(int i=0;i<n-2;i++)
	{
		scanf("%d",&c);
		cap[i+1][n-1]+=c;
		ver[i+1].push_back(n-1);
	}
	for(int i=0;i<n-2;i++)
	{
		for(int j=0;j<n-2;j++)
		{	
			scanf("%d",&c);
			cap[i+1][j+1]=c;
			ver[i+1].push_back(j+1);
		}
	}
	printf("%d\n",maxFlow(0,n-1));
	}
	return 0;
}






