#include <bits/stdc++.h>

using namespace std;

// fstream file;
// file.open("uitvoerbestand", fstream::out);

int main()
{
    int nb_worlds, nb_variables, nb_clauses, av_nb_vars_per_clause;
    scanf("%d %d %d %d", &nb_worlds, &nb_variables, &nb_clauses, &av_nb_vars_per_clause);
    if(av_nb_vars_per_clause > nb_variables) cerr << "av_nb_vars_per_clause is bigger than nb_variables" << endl;
    fstream file;
    file.open("observed_worlds", fstream::out);
    file << nb_worlds << " " << nb_variables << endl;
    for(int world=0; world<nb_worlds; world++)
    {
        for(int i=0; i<nb_variables; i++)
        {
            if(i != 0) file << " ";
            int ra = rand()%11;
            if(ra <= 0)
            {
            	file << "2";
            }
            else if (ra < 6)
            {
            	file << "1";
            }
            else if (ra >= 6)
            {
            	file << "0";
            }
        }
        file << endl;
    }
    file.close();

    file.open("clauses", fstream::out);
    file << 2*nb_clauses << endl;
    vector< vector<int> > clauses;
    for(int i=0; i<nb_clauses; i++)
    {
        vector<int> current_clause;
        vector<bool> taken(nb_variables, false);

        // determine how many variables this clause will consist of.
        int max_delta=min(av_nb_vars_per_clause-1, nb_variables-av_nb_vars_per_clause); // you want at least 1 variable, at most nb_variables variables
        int delta=rand()%(max_delta+1);
        int sgn=rand()%2;
        if(sgn==0) sgn=-1;
        delta *= sgn;
        int current_vars=av_nb_vars_per_clause+delta;
        
        for(int j=0; j<current_vars; j++) // generate variable which wasn't picked before.
        {
            int vars_left = nb_variables-current_clause.size();
            int amount = (rand()%vars_left)+1;

            int ctr=0;
            int var=-1;
            for(int k=0; k<nb_variables; k++)
            {
                if(!taken[k])
                {
                    ctr++;
                    var=k;
                }
                if(ctr==amount) break;
            }
            taken[var]=true;
            int sgn=rand()%2;
            if(sgn==0) sgn=-1;
            var = (var+1)*sgn;
            current_clause.push_back(var);             
        }
        clauses.push_back(current_clause);
    }
    for(int times=0; times<2; times++)
    {
        for(int i=0; i<clauses.size(); i++)
        {
            file<<clauses[i].size();
            for(int j=0; j<clauses[i].size(); j++)
            {
                file << " " << clauses[i][j];
            }
            file << endl;
        }
    }
    file.close();
    return 0;
}
