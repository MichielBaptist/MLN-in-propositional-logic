#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream file;
    file.open("evidence_atoms", fstream::in);
    set<int> evidence_atoms;
    int nb_evidence_atoms;
    if(file>>nb_evidence_atoms)
    {
        for(int i=0; i<nb_evidence_atoms; i++)
        {
            int x;
            file>>x;
            evidence_atoms.insert(x);
        }
    }
    file.close();

    set<int> query_and_evidence_atoms;
    for(set<int>::iterator it = evidence_atoms.begin(); it != evidence_atoms.end(); it++)
    {
        query_and_evidence_atoms.insert(*it);
    }
    
    set<int> query_atoms;
    file.open("query_atoms", fstream::in);
    int nb_query_atoms;
    if(file>>nb_query_atoms)
    {
        for(int i=0; i<nb_query_atoms; i++)
        {
            int x;
            file>>x;
            query_and_evidence_atoms.insert(x);
            query_atoms.insert(x);
        }
    }
    file.close();

    file.open("query_atoms", fstream::out);
    file<<query_and_evidence_atoms.size();
    for(set<int>::iterator it = query_and_evidence_atoms.begin(); it != query_and_evidence_atoms.end(); it++)
    {
        file<<" ";
        file<<(*it);
    }
    file<<"\n";
    file.close();
    
    file.open("vtree_in_name", fstream::out);
    file<<"";
    file.close();

    file.open("vtree_out_name", fstream::out);
    file<<"bekomen_vtree1";
    file.close();

    system("./no_evidence_solver");

    file.open("marg_output", fstream::in);
    string s;
    getline(file, s);
    double pqe=1.0;
    pqe = atof(s.c_str());
    file.close();


    double pe=1.0;
    file.open("query_atoms", fstream::out);
    file<<evidence_atoms.size();
    for(set<int>::iterator it = evidence_atoms.begin(); it != evidence_atoms.end(); it++)
    {
        file<<" ";
        file<<(*it);
        //cerr << " " << *it<<endl;
    }
    file<<"\n";
    file.close();

    file.open("vtree_in_name", fstream::out);
    file<<"";
    file.close();

    file.open("vtree_out_name", fstream::out);
    file<<"bekomen_vtree2";
    file.close();

    system("./no_evidence_solver");
    file.open("marg_output", fstream::in);
    getline(file, s);
    pe = atof(s.c_str());
    file.close();
    

    //cerr << pqe << " " << pe << endl;

    file.open("marg_output", fstream::out);
    file<<fixed<<setprecision(12)<< pqe/pe<<"\n";
    file.close();

    // restore the query and evidence files.
    file.open("evidence_atoms", fstream::out);
    file<<nb_evidence_atoms;
    for(set<int>::iterator it = evidence_atoms.begin(); it != evidence_atoms.end(); it++)
    {
        file<<" "<<(*it);
    }
    file.close();

    file.open("query_atoms", fstream::out);
    file<<nb_query_atoms;
    for(set<int>::iterator it = query_atoms.begin(); it != query_atoms.end(); it++)
    {
        file<<" "<<(*it);
    }
    file.close();
    return 0;
}
