#include <bits/stdc++.h>

using namespace std;

// fstream file;
// file.open("uitvoerbestand", fstream::out);

int main()
{
    int nb_worlds, nb_variables, nb_clauses, nb_vars_per_clause;
    scanf("%d %d %d %d", &nb_worlds, &nb_variables, &nb_clauses, &nb_vars_per_clause);
    if(nb_vars_per_clause > nb_variables) cerr << "nb_vars_per_clause is bigger than nb_variables" << endl;
    fstream file;
    file.open("observed_worlds", fstream::out);
    file << nb_worlds << " " << nb_variables << endl;
    for(int world=0; world<nb_worlds; world++)
    {
        for(int i=0; i<nb_variables; i++)
        {
            if(i != 0) file << " ";
            int ra = rand()%2;
            file << ra;
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
        for(int j=0; j<nb_vars_per_clause; j++) // generate variable which wasn't picked before.
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
            file<<nb_vars_per_clause;
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
