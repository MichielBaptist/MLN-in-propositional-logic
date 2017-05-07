#include <bits/stdc++.h>

using namespace std;

struct w_clause
{
    double weight;
    vector<int> variables;
    w_clause(){};
    w_clause(double _weight, vector<int> _variables)
    {
        weight=_weight;
        variables=_variables;
    }
};

double find_wmc_in_file(string f_name)
{
    fstream file;
    file.open(f_name.c_str(), fstream::in);
    double d1=-1.0;
    double d2=-1.0;
    string s;
    int ctr=0;
    while(getline(file, s) && ctr != 3)
    {
        if(s.size()==0) continue;
        stringstream ss(s);
        string word;
        ss >> word;
        if(word=="Count")
        {
            ctr++;
            if(ctr==3)
            {
                ss >> d1;
            }
        }
    }
    return d1;
}

pair<double, double> get_wmcs(int nb_v, int nb_c, int new_nb_var, int new_nb_c, vector<w_clause> clauses, vector<int> &query_atoms, vector< pair<double, double> > &machten)
{
    fstream file;
    file.open("c2d/input_for_c2d", fstream::out);
    file << "c weights";
    
    // als de gewichten negatief zijn, moeten we ze positief maken en een passende factor aftrekken.
    double smallest=0;
    for(int i=0; i<machten.size(); i++)
    {
    	smallest=min(smallest, min(machten[i].first, machten[i].second));
    }
    for(int i=0; i<machten.size(); i++)
    {
    	machten[i].first -= smallest;
    	machten[i].second -= smallest;
    }
    
    for(int i=0; i<new_nb_var; i++)
    {
    	double g1 = machten[i].first;
    	double g2 = machten[i].second;
    	file << " " << fixed << setprecision(25) << g1 << " " << g2;
    }
    file << "\n";
    file << "p cnf " << new_nb_var << " " << new_nb_c << "\n";
  
    for(int i=0; i<nb_c; i++)
    {
    	file << -(nb_v+i+1);
        for(int j=0; j<clauses[i].variables.size(); j++)
        {
        	file << " " << clauses[i].variables[j];
        }
        file << " 0\n";
        for(int j=0; j<clauses[i].variables.size(); j++)
        {
        	file << -clauses[i].variables[j] << " " << nb_v+i+1 << " 0\n";
        }

    }
    file.close();

    file.open("c2d/vtree_in_name", fstream::in);
    string vtree_in_name;
    file >> vtree_in_name;
    file.close();
    
    file.open("c2d/vtree_out_name", fstream::in);
    string vtree_out_name;
    file >> vtree_out_name;
    file.close();

    string command="./c2d/miniC2D -c c2d/input_for_c2d ";
    if(vtree_in_name != "")
    {
        command += "-v ";
        command += vtree_in_name;
    }
    else
    {
        command += "-o ";
        command += vtree_out_name;
    }
    command += "c2d/_A -W > c2d/output_for_c2d";
    system(command.c_str());

	double p1 = find_wmc_in_file("c2d/output_for_c2d");
	
    file.open("c2d/input_for_c2d2", fstream::out);
    new_nb_c += query_atoms.size();
    file << "c weights";
    for(int i=0; i<new_nb_var; i++)
    {
    	double g1 = machten[i].first;
    	double g2 = machten[i].second;
    	file << " " << fixed << setprecision(25) << g1 << " " << g2;
    }
    file << "\n";
    file << "p cnf " << new_nb_var << " " << new_nb_c << "\n";
  
    for(int i=0; i<nb_c; i++)
    {
    	file << -(nb_v+i+1);
        for(int j=0; j<clauses[i].variables.size(); j++)
        {
        	file << " " << clauses[i].variables[j];
        }
        file << " 0\n";
        for(int j=0; j<clauses[i].variables.size(); j++)
        {
        	file << -clauses[i].variables[j] << " " << nb_v+i+1 << " 0\n";
        }

    }
    for(int i=0; i<query_atoms.size(); i++)
    {
        file << query_atoms[i] << " 0\n";
    }
    file.close();
    
    command="./c2d/miniC2D -c c2d/input_for_c2d2 ";
    if(vtree_in_name != "")
    {
        command += "-v ";
        command += vtree_in_name;
    }
    else
    {
        command += "-o ";
        command += vtree_out_name;
    }
    command += "c2d/_B -W > c2d/output_for_c2d2";
    system(command.c_str());
    double p2 = find_wmc_in_file("c2d/output_for_c2d2");
    // schaal voor negatieve gewichten de uitkomst
    p1 -= new_nb_var*smallest;
    p2 -= new_nb_var*smallest;
    return make_pair(p1, p2);
}

int main()
{
    fstream file;
	file.open("mln", fstream::in);
	int nb_v, nb_c;
	file >> nb_v >> nb_c;
    vector<w_clause> clauses;
    for(int i=0; i<nb_c; i++)
    {
        double w;
        file >> w;
        vector<int> tmp;
        while(true)
        {
            int x;
            file >> x;
            if(x==0) break;
            tmp.push_back(x);
        }
        clauses.push_back(w_clause(w, tmp));
    }
    file.close();
    
    vector<int> query_atoms;
    file.open("query_atoms", fstream::in);
    int nb_atoms;
    if(file>>nb_atoms)
    {
        for(int i=0; i<nb_atoms; i++)
        {
            int x;
            file >> x;
            query_atoms.push_back(x);
        }
    }
    file.close();

    int new_nb_var = nb_v+nb_c; //the newly introduced variables will have index nb_v+1 ... nb_v+nb_c
    int new_nb_c=0;
    for(int i=0; i<clauses.size(); i++)
    {
        new_nb_c += clauses[i].variables.size()+1;
    }
    
    vector< pair<double, double> > machten(new_nb_var, make_pair(0, 0));
    for(int i=0; i<nb_c; i++)
   	{
   		machten[nb_v+i]=make_pair(clauses[i].weight, 0);
   	}
    
    pair<double, double> wmcs = get_wmcs(nb_v, nb_c, new_nb_var, new_nb_c, clauses, query_atoms, machten);
    
    double e_macht = wmcs.second-wmcs.first;
    double ans;
    if(abs(e_macht) >= 300)
    {
        ans=0.0;
    }
    else
    {
        ans=exp(e_macht);
    }
    file.open("marg_output", fstream::out);
    file << fixed << setprecision(25) << ans << "\n";
    file.close();

    return 0;
}
