#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

void writeToQuery(int var)
{
	fstream query;
	query.open("query_atoms", fstream::out);
	query<<"1";
	query<<" ";
	query<<var;
	query.close();
}

int main()
{

int it_t, to_t;

fstream file;
file.open("mln", fstream::in);

int vars, i = 1;
timeval t1, t2;
gettimeofday(&t1, 0);

file>> vars;
file.close();

file.open("all_inference_time", fstream::out);

cout<< "==Vars: " << vars;

	//while(i <= vars)
	//{
		gettimeofday(&t1, 0);
		
		writeToQuery(i);
		system("./evidence_solver");
		i++;
		
		gettimeofday(&t2, 0);
		
		int t1_s = (t1.tv_sec *1000) +t1.tv_usec/1000;
		int t2_s = (t2.tv_sec *1000) +t2.tv_usec/1000;
		
		it_t = t2_s - t1_s;
		cout<< "tijd voor p(" << i << "): " << it_t << "\n";
		
		to_t += it_t;
		
	//}
	file << it_t * vars<< "\n";
}
