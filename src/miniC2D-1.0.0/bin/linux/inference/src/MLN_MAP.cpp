#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

struct w_clause
{
	long long weight;
	vector<long long> variables;
	w_clause() {};
	w_clause(long long _weight, vector<long long> _variables)
	{
		weight=_weight;
		variables=_variables;
	}
};
 
int main()
{	
	timeval t1, t2;
	gettimeofday(&t1, 0);

	//Read evidence atoms
	set<long long> evidence_atoms;
	freopen("evidence", "r", stdin);
	int nb_e;
	if(scanf("%d", &nb_e) == 1)
	{
		for(int i=0; i<nb_e; i++)
		{
			long long atom;
			scanf("%lld", &atom);
			evidence_atoms.insert(atom);
		}
	}
	fclose(stdin);

	//Schrijf de aangepast MLN naar CCLS_invoer (het bestand dat CCLS_to_akmaxsat gebruikt
	freopen("mln", "r", stdin);
	freopen("CCLS_invoer", "w", stdout);
	vector<w_clause> mln_old; 
	int nb_v, nb_c;
	scanf("%d %d", &nb_v, &nb_c);
	
	for(int i=0; i<nb_c; i++)
	{
		double w;
		scanf("%lf", &w);
		//cerr << "w: " << w << endl;
		long long w_integral = (long long)(100000*w+0.5);
		bool needed=true;
		vector<long long> tmp;
		while(true)
		{
			long long var;
			scanf("%lld", &var);
			if(var==0) break;
			if(evidence_atoms.find(var) != evidence_atoms.end())
			{
				needed=false;
			}
			if(evidence_atoms.find(-var) == evidence_atoms.end())
			{
				//cerr << -var << " " << evidence_atoms.size() << endl;
				tmp.push_back(var);
			}
		}
		if(tmp.size() == 0) needed=false;
		if(!needed)
		{
			continue;
		}
		mln_old.push_back(w_clause(w_integral, tmp));
	}
	
	int ctr=1;
	map<long long, long long> mln_old_to_mln_new;
	map<long long, long long> mln_new_to_mln_old;
	for(int i=0; i<mln_old.size(); i++)
	{
		for(int j=0; j<mln_old[i].variables.size(); j++)
		{
			long long var = mln_old[i].variables[j];
			if(mln_old_to_mln_new.find(var) == mln_old_to_mln_new.end())
			{
				mln_old_to_mln_new[var]=ctr;
				mln_new_to_mln_old[ctr]=var;
				mln_old_to_mln_new[-var]=-ctr;
				mln_new_to_mln_old[-ctr]=-var;
				ctr++;
			}
		}
	}
	fclose(stdin);
	
	if(ctr != 1)
	{
		printf("p wcnf %lld %lld\n", ctr-1LL, 0LL+mln_old.size());
		for(int i=0; i<mln_old.size(); i++)
		{
			printf("%lld", mln_old[i].weight);
			for(int j=0; j<mln_old[i].variables.size(); j++)
			{
				printf(" %lld", mln_old_to_mln_new[mln_old[i].variables[j]]);
			}
			printf(" 0\n");
		}
		fclose(stdout);

		std::system("./CCLS_to_akmaxsat CCLS_invoer > CCLS_uitvoer");
	}
	else
	{
		fclose(stdout);
		freopen("CCLS_uitvoer", "w", stdout);
		printf("v");
		for(long long i=1; i<=nb_v; i++)
		{
			if(evidence_atoms.find(-i) != evidence_atoms.end())
			{
				printf(" -%lld", i);
			}
			else
			{
				printf(" %lld", i);
			}
			mln_old_to_mln_new[i]=mln_new_to_mln_old[i]=i;
			mln_old_to_mln_new[-i]=mln_new_to_mln_old[-i]=-i;
		}
		printf("\n");
		fclose(stdout);
	}
	

	freopen("CCLS_uitvoer", "r", stdin);
	freopen("MAP", "w", stdout);
	string v_line = "";
	
	string s="";
	while(getline(cin, s))
	{
		if(s.size() == 0) continue;
		if(s[0] == 'v') v_line = s;
	}
	if(v_line.size() >= 2)
	{
		v_line=v_line.substr(2, v_line.size()-2);
	}
	vector<long long> ans(nb_v+1, 0);
	for(int i=1; i<=nb_v; i++)
	{
		ans[i]=i;
		if(evidence_atoms.find(-i) != evidence_atoms.end())
		{
			ans[i]=-i;
		}
	}
	stringstream ss(v_line);
	long long val;
	while(ss>>val)
	{
		ans[abs(mln_new_to_mln_old[val])] = mln_new_to_mln_old[val];
	}
	for(int i=1; i<=nb_v; i++)
	{
		if(i != 1)
		{
			printf(" ");
		}
		printf("%lld", ans[i]);
	}
	
	printf("\n");	
	fclose(stdout);
	fclose(stdin);
	
	//cout << "Gedaan"<<endl;
	
	gettimeofday(&t2, 0);	
	int t1_s = (t1.tv_sec *1000) +t1.tv_usec/1000;
	int t2_s = (t2.tv_sec *1000) +t2.tv_usec/1000;	
	int ti = t2_s - t1_s;

	fstream time;
	time.open("map_time");
	time<<ti<<endl;
	time.close();
	
	return 0;
}


