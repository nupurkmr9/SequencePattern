using namespace std;

#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<algorithm>

#define structure map<vector<int>, int>
#define FOR_MAP(ii,T) for(structure::iterator (ii)=(T).begin();(ii)!=(T).end();(ii)++)
#define FOR_next_MAP(jj,ii,T) for(structure::iterator (jj)=(ii);(jj)!=(T).end();(jj)++)
#define VI vector<vector<int>>
#define V2 vector<int>

const int MIN_SUP = 2; //Minimum Support

structure C;
structure L;

void C1();
void L1();
void generate_C();
void generate_L();
void output(structure );
void scan_D();
void prune();
bool check_compatibility(VI ,VI );
void set_count(VI );

int main(int argc, char const *argv[])
{
	C.clear();
	L.clear();

	bool mv=true;
	int index=2;
	while(true)
	{
		if (mv) //this if is for initialising. Once initialized, the else will run.
		{
			C1(); //Load input from file and get candidate 1 itemsets.
			cout<<"C1\n";
			output(C);

			L1(); //Get large(frequent) 1-itemsets by comparing with min sup.
			cout<<"L1\n";
			output(L);
			
			mv=!mv;
		}else
		{
			generate_C(); //Generate (k+1) candidate itemset
			if(C.size()==0)
				break;
			cout<<"\nC"<<index<<"\n";
			output(C);
			prune();  //Prune the itemset
			if (C.size()==0)
			{
				break;
			}
			cout<<"\nC"<<index<<" after prune \n";
			output(C);
			scan_D(); //Do a scan of dataset
			cout<<"\nC"<<index<<"after scaning dataset \n";
			output(C); 
			generate_L(); //Find large itemsets from given candidates
			if (L.size()==0)
			{
				break;
			}
			cout<<"\nL"<<index<<"\n";
			output(L);
			index++;
		}
	}
	return 0;
}


/* This Function Loads the input into structure C.
The input contains many transactions with integer values. -1 denotes end of a transaction */

void C1()
{
	ifstream fin;
	fin.open("input.txt");
	if(!fin)
		{
			cout<<"Input file opening error\n";
			exit(0);
		}

	int n;
	VI v;
	V2 t;
	while(fin>>n)
	{
		v.clear();
		t.clear();
		if (n==-1)
		{
			v.push_back(t)
		}
		if (n==-2)
		{
			v.push_back(t)
			continue
		}

		t.push_back(n);
		
		if(C.count(v)>0)
			C[v]++;
		else
			C[v]=1;
	}
	fin.close();
}

/* This function prints any structure. 
This means printing all the members and their frequency in any candidate or large itemset.
Run the code to get a feel if not clear */
void output(structure T)
{
	cout<<"\n";
	VI v;
	FOR_MAP(ii,T)
	{
		v.clear();
		v=ii->first;
		for (int i = 0; i < v.size(); ++i)
		{
			cout<<v[i]<<" ";
		}

		cout<<" ---(frequency)----->> "<<ii->second;
		cout<<"\n";

	}
}


/*
This function generates the large itemsets from candidate 1-itemset by comparing them with minimum support. For multisets, the function is generate_L()
*/
void L1()
{

	FOR_MAP(ii,C)
	{
		if (ii->second >= MIN_SUP)
		{
			L[ii->first]=ii->second;
		}
	}

}


/*
This function generates (N+1) candidate itemsets from (N) frequent itemsets.
*/
void generate_C()
{
	//clean(C);
	C.clear();
	FOR_MAP(ii,L)
	{

		FOR_next_MAP(jj,ii,L)
		{
			if(jj==ii)
				continue;
			VI a,b;
			a.clear();
			b.clear();
			a=ii->first;
			b=jj->first;
			if(check_compatibility(a,b))	
			{
				a.push_back(b.back());
				sort(a.begin(), a.end());
				C[a]=0;
			}
		}

	}


}

/* This function is basically comparing two vectors for equality */
bool check_compatibility(VI a,VI b)
{
	bool compatible=true;
	for (int i = 0; i < a.size()-1; ++i)
	{
		if (a[i]!=b[i])
		{
			compatible=false;
			break;
		}
	}

	return compatible;
}

/* Performs the standard pruning method used in apriori algorithm */
void prune()
{
	VI a,b;
	
	FOR_MAP(ii,C)
	{
		a.clear();
		b.clear();

		a=ii->first;
		for(int i = 0;i<a.size();i++)
		{
			b.clear();
			for (int j = 0; j < a.size(); ++j)
			{
				if(j==i)
					continue;
				b.push_back(a[j]);
			}
			if(L.find(b)==L.end())
				{
					ii->second=-1;
					break;
				}
			
		}

		
	}

	structure temp;
	temp.clear();
	FOR_MAP(ii,C)
	{
		if (ii->second != -1)
		{
			temp[ii->first]=ii->second;
		}
	}
	
	C.clear();
	C=temp;
	temp.clear();
}

/* Scans the data and updates count of all candidate pairs left after pruning */
void scan_D()
{
	ifstream fin;
	fin.open("input.txt");
	if(!fin)
		{
			cout<<"Input file opening error\n";
			exit(0);
		}

	int n;
	VI a;
	while(fin>>n)
	{
		if(n==-1 && a.size()>0)
		{
			set_count(a); // Update the count of candidates
			a.clear();
		}else if(n!=-1)
		{
			a.push_back(n);
		}
		
	}
	fin.close();
}

/* This sets the count of all candidates to the actual value of count*/
void set_count(VI a)
{
	FOR_MAP(ii,C)
	{
		VI b;
		b.clear();
		b=ii->first;
		int true_count=0;
		if (b.size()<=a.size())
		{
			for (int i = 0; i < b.size(); ++i)
			{
				for (int j = 0; j < a.size(); ++j)
				{
					if(b[i]==a[j])
					{
						true_count++;
						break;
					}
				}
			}
		}

		if (true_count==b.size())
		{
			ii->second++;
		}
	}
}

/* Generates large itemsets from candidates by comparing with minsup */
void generate_L()
{
	L.clear();

	FOR_MAP(ii,C)
	{
		if(ii->second >= MIN_SUP)
		{
			L[ii->first]=ii->second;
		}
	}
}
