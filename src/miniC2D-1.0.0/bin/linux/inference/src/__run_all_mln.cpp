#include <bits/stdc++.h>
#include <string>

using namespace std;

string readMLN(ifstream* in_mln)
{
	string res = "", line;
	while(getline(*in_mln, line))
	{
		res += line + "\n";
	}

return res;
}

int main()
{
 fstream in_time;
 fstream out_time;
 ifstream in_mln;
 fstream out_mln;
 int lim = 1000;
 int var = 20;
 int clause = 15;
 int step = 20;
 int time;
 string content;
 
 out_time.open("all_times"); 
 
 while( var <= lim)
 {
 	if(var >=50)
 		step = 20;
 		
 	in_mln.open("mlndir/" + to_string(clause) + "_var_" + to_string(var) + "_clauses.mln", fstream::in);
 	content = readMLN(&in_mln);
 	in_mln.close();
 	
 	out_mln.open("mln", fstream::out);
 	out_mln << content;
 	out_mln.close();
 	
 	system("./MAP_solver");
 	
 	in_time.open("map_time");
 	in_time>>time;
 	in_time.close();
 	
 	cout << time;
 	
 	out_time<< to_string(var) + "_var_" + to_string(clause) + "_clauses.mln" << " " << time<< "\n";
 	var+=step;
 }
 out_time.close();
 return 0;
}
