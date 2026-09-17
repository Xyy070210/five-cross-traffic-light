#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<set>
using namespace std;
int main()
{
	vector<string> routes={"A->B","A->C","A->D","B->A","B->C","B->D","D->A","D->B","D->C","E->A","E->C","E->B","E->D"};
	int n=routes.size();
	unordered_map <string,int> routeIdx;
	for(int i=0;i<n;i++)
	{
		routeIdx[routes[i]]=i;
	}
	vector<vector<int>> conflict(n,vector<int>(n,0));
	int a_b=routeIdx["A->B"];
	int e_a=routeIdx["E->A"];
	int d_a=routeIdx["D->A"];
	int b_c=routeIdx["B->C"];
	int b_d=routeIdx["B->D"];
	int a_c=routeIdx["A->C"];
	int e_b=routeIdx["E->B"];
	int d_b=routeIdx["D->B"];
	int a_d=routeIdx["A->D"];
	int e_c=routeIdx["E->C"];
  //========冲突矩阵========
  //与 AB 冲突：EA DA BC BD
	conflict[a_b][e_a]=1; conflict[e_a][a_b]=1;
	conflict[a_b][d_a]=1; conflict[d_a][a_b]=1;
	conflict[a_b][b_c]=1; conflict[b_c][a_b]=1;
	conflict[a_b][b_d]=1; conflict[b_d][a_b]=1;
	//与 AC 冲突：DA EA EB DB BD
	conflict[a_c][d_a]=1; conflict[d_a][a_c]=1;
	conflict[a_c][e_a]=1; conflict[e_a][a_c]=1;
	conflict[a_c][e_b]=1; conflict[e_b][a_c]=1;
	conflict[a_c][d_b]=1; conflict[d_b][a_c]=1;
	conflict[a_c][b_d]=1; conflict[b_d][a_c]=1;
  //与 AD 冲突:EA EB EC 	
	conflict[a_d][e_a]=1; conflict[e_a][a_d]=1;
	conflict[a_d][e_b]=1;	conflict[e_b][a_d]=1;
	conflict[a_d][e_c]=1; conflict[e_c][a_d]=1;
	//与 BC 冲突：EB DB
	conflict[b_c][e_b]=1;	conflict[e_b][b_c]=1;
	conflict[b_c][d_b]=1;	conflict[d_b][b_c]=1;
  //与 BD 冲突: DA EB EC 
	conflict[b_d][d_a]=1;	conflict[d_a][b_d]=1;
	conflict[b_d][e_b]=1;	conflict[e_b][b_d]=1;
	conflict[b_d][e_c]=1;	conflict[e_c][b_d]=1;
  //与 DA 冲突: EB EC
	conflict[d_a][e_b]=1;	conflict[e_b][d_a]=1;
	conflict[d_a][e_c]=1;	conflict[e_c][d_a]=1;
  //与 DB 冲突：EC
	conflict[d_b][e_c]=1;	conflict[e_c][d_b]=1;

  //========固定永久长绿路线=======
	vector<string> everGreen={"B->A","D->C","E->D"};
	unordered_map<string,bool> isEverGreen;
	for(auto &s:everGreen)
	{
		isEverGreen[s]= true;
	}
  //===筛选需要参与图着色的路线（剔除常绿）======
	vector<string> workRoutes;
	for(auto &r: routes)
	{
		if(!isEverGreen[r])
		  workRoutes.push_back(r);
	}
  //=====建立子图索引=====
	unordered_map<string, int> workIdx;
	int m = workRoutes.size();
	for(int i=0;i<m;i++)
	{
		workIdx[workRoutes[i]]=i;
	}
	vector<vector<int>> workConf(m,vector<int> (m,0));
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			int ori_i=routeIdx[workRoutes[i]];
			int ori_j=routeIdx[workRoutes[j]];
			workConf[i][j]=conflict[ori_i][ori_j];
		}
	}
  //========贪心图着色算法=========
    vector<int> color(m,-1);
    for(int u=0;u<m;++u)
    {
    	set<int> used;
    	for(int v=0;v<m;++v)
    	{
    		if(workConf[u][v]==1&&color[v]!=-1)
    		used.insert(color[v]);
		}
	int c=0;
	while(used.count(c))
	c++;
	color[u]=c;
    }
  //=======按颜色分组成各个相位=======
    vector<vector<string>> phases;
    int maxPhase=0;
    for(int c:color)
      if(c>maxPhase)  maxPhase=c;
      phases.resize(maxPhase+1);
      for(int i=0;i<m;++i)
      {
      	phases[color[i]].push_back(workRoutes[i]);
	  }
	  for(auto &phase :phases)
	  {
	  	bool hasAroute=false;
	  	bool hasEA=false;
	  	for(auto &s :phase)
	  	{
	  		if(s=="A->B"||s=="A->C"||s=="A->D")
	  		{
	  			hasAroute=true;
			  }
			  if(s=="E->A")
		    {
		    	hasEA=true;
		    }
		  }
		  if(!hasAroute&&!hasEA)
		  {
		  	phase.push_back("E->A");
		  }
	  }
	  cout<<"五岔路口信号灯相位分配结果"<<endl;
	  cout<<"【永久常绿路线（全程放行，不参与相位轮换）】：";
	  for(auto &s: everGreen)
	  cout<<s<<" ";
	  cout<<"\n"<<endl;
	  for(int p=0;p<phases.size();++p)
	  {
	  	cout<<"相位"<<p+1<<"（路灯放行路线）：";
	  	for(auto &s:phases[p])
	  	{
	  		cout<<s<<" ";
		  }
		  cout<<"\n";
	  }
	  return 0;
}
