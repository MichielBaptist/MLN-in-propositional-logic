#include <bits/stdc++.h>

using namespace std;

string to_string(int num)
{
    stringstream ss;
    ss << num;
    string ret;
    ss >> ret;
    return ret;
}

int main()
{

	//fstream i_expected, i_actual;
	//i_actual.open("iteraties_actual");
	//i_expected.open("iteraties_expected");
	
	fstream file;
	file.open("observed_worlds", fstream::in);
	int nb_observed_worlds;
	file>>nb_observed_worlds;
	int nb_variables_per_world;
	file>>nb_variables_per_world;
	vector< vector<int> > worlds(nb_observed_worlds, vector<int>(nb_variables_per_world));
    vector<bool> incomplete(nb_observed_worlds, false);
	for(int i=0; i<nb_observed_worlds; i++)
	{
		for(int j=0; j<nb_variables_per_world; j++)
		{
			file>>worlds[i][j];
            if(worlds[i][j] != 0 && worlds[i][j] != 1) incomplete[i]=true;
		}
	}
	file.close();
	
	
	file.open("clauses", fstream::in);
	int nb_clauses;
	file>>nb_clauses;
	vector< vector<int> > clauses(nb_clauses);
	for(int i=0; i<nb_clauses; i++)
	{
		int nb_vars;
		file >> nb_vars;
		vector<int> clause(nb_vars);
		for(int j=0; j<nb_vars; j++)
		{
			file>>clause[j];
		}
		sort(clause.begin(), clause.end());
		clauses[i]=clause;
	}
	file.close();
	
	vector<double> weights(nb_clauses, 1.0);

	double learning_rate=-1.0/nb_observed_worlds;
	int max_iteraties=100;
	for(int iteratie=0; iteratie<max_iteraties; iteratie++)
	{
		learning_rate *= 0.95;
		cerr << "iteratie: " << iteratie << endl;	
		file.open("mln", fstream::out);
		file<<nb_variables_per_world<<" "<<nb_clauses<<"\n";
		for(int i=0; i<nb_clauses; i++)
		{
			file<<fixed<<setprecision(12)<<weights[i];
			for(int j=0; j<clauses[i].size(); j++)
			{
				file<<" "<<clauses[i][j];
			}
			file<<" 0\n";
		}
		file.close();
		
		
		vector<double> expected_nb_true(nb_clauses);
		for(int i=0; i<nb_clauses; i++) // bereken voor elke clause hoe vaak we verwachten dat hij waar is met de huidige mln.
		{
			file.open("query_atoms", fstream::out);
			int sz = clauses[i].size();
			file<<sz;
			//cerr << sz;
			for(int j=0; j<clauses[i].size(); j++) // kans op (a v b v c) = 1-kans op(-a ^ -b ^ -c)
			{
				file << " " << -clauses[i][j];
				//cerr << " " << -clauses[i][j];
			}
			file.close();
			
            if(iteratie==0) //vtree nog niet berekend.
            {
                string vtree_naam="vtree"+to_string(i);
                file.open("vtree_in_name", fstream::out);
                file<<"";
                file.close();
                file.open("vtree_out_name", fstream::out);
                file<<vtree_naam;
                file.close();
            }
            else //vtree al berekend.
            {
                string vtree_naam="vtree"+to_string(i);
                file.open("vtree_in_name", fstream::out);
                file<<vtree_naam;
                file.close();
                file.open("vtree_out_name", fstream::out);
                file<<"";
                file.close();
            }

			system("./no_evidence_solver");
			file.open("marg_output", fstream::in);
			string s;
			getline(file, s);
			file.close();
			
			double kans_op_clause = 1-atof(s.c_str());
			//cerr << "kans: " << kans_op_clause << endl;
			expected_nb_true[i]=kans_op_clause*nb_observed_worlds;
		}
		
		printf("expected volgens huidige mln: \n");
		for(int i=0; i<nb_clauses; i++)
		{
			printf("%f\n", expected_nb_true[i]);
			//i_expected << expected_nb_true[i] << " ";
		}
		//i_expected << endl;

        // vul de onvolledige data zo waarschijnlijk mogelijk aan
        vector< vector<int> > completed_worlds(nb_observed_worlds, vector<int>(nb_variables_per_world));
        for(int i=0; i<nb_observed_worlds; i++)
        {
            if(incomplete[i])
            {
                vector<int> evidence_for_map;
                for(int j=0; j<nb_variables_per_world; j++)
                {
                    if(worlds[i][j]==0) evidence_for_map.push_back(-(j+1));
                    else if(worlds[i][j]==1) evidence_for_map.push_back(j+1);
                }
                file.open("evidence", fstream::out);
                file<<evidence_for_map.size()<<"\n";
                for(int j=0; j<evidence_for_map.size(); j++)
                {
                    if(j != 0) file << " ";
                    file << evidence_for_map[j];
                }
                file << "\n";
                file.close();

                system("./MAP_solver");

                file.open("MAP", fstream::in);
                for(int j=0; j<nb_variables_per_world; j++)
                {
                    int found_variable;
                    file >> found_variable;
                    if(found_variable > 0) completed_worlds[i][found_variable-1]=1;
                    else completed_worlds[i][-found_variable-1]=0;
                }
                file.close();
            }
            else completed_worlds[i]=worlds[i];
        }
        
        // bereken voor elke clause hoe vaak hij waar is volgens de nieuwe invulling.
        vector<int> nb_true(nb_clauses, 0);
	    for(int i=0; i<nb_clauses; i++)
	    {
		    for(int j=0; j<nb_observed_worlds; j++)
		    {
			    bool clause_voldaan=false;
			    for(int k=0; k<clauses[i].size() && !clause_voldaan; k++)
			    {
				    int var = clauses[i][k];
				    if(var>0) // positieve literal
				    {
					    if(completed_worlds[j][var-1]==1) clause_voldaan=true;
				    }
				    else
				    {
					    if(completed_worlds[j][-var-1]==0) clause_voldaan=true;
				    }
			    }
			    if(clause_voldaan) nb_true[i]++;
			}
	    }
	
	    cerr << "nb_true: \n";
	    for(int i=0; i<nb_clauses; i++)
	    {
		    cerr << nb_true[i] << " ";
		    //i_actual << nb_true[i] << " ";
	    }
	    cerr << "\n";
	    //i_actual << endl;

	    for(int i=0; i<nb_clauses; i++)
	    {
		    if(nb_true[i]==nb_observed_worlds)
		    {
			    weights[i]=100.0;
		    }
		    else if(nb_true[i]==0)
		    {
			    weights[i]=-100.0;
		    }
	    }

        // pas de gewichten aan
		for(int i=0; i<nb_clauses; i++)
		{
			weights[i] -= learning_rate*(nb_true[i]-expected_nb_true[i]);
			weights[i] = max(-100.0, min(weights[i], 100.0));
		}

        // schrijf de nieuwe mln weg
        file.open("mln", fstream::out);
	    file<<nb_variables_per_world<<" "<<nb_clauses<<"\n";
	    for(int i=0; i<nb_clauses; i++)
	    {
		    file<<weights[i];
		    for(int j=0; j<clauses[i].size(); j++)
		    {
			    file<<" "<<clauses[i][j];
		    }
		    file<<" 0\n";
	    }
	    file.close();
	}
	//i_actual.close();
	//i_expected.close();
			
	return 0;
}
