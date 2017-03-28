using namespace std;

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <list>


class itemset{

public:

	int size=0;
	vector<int> items;

	void addItem(int a){
		items.push_back(a);
		size += 1;
	}

	void Clear(){
		size = 0;
		items.clear();
	}

	int notExists(int a){
		if (std::find(items.begin(), items.end(),a)!=items.end()){
			return -1;
		}
		else{
			return 1;
		}
	}

};


class sequence {
public:
	int size=0;
	vector<itemset>  itemsets;
	int id;

	void addItemSet(itemset i ){
		itemsets.push_back(i);
		size += i.size;
	}

	void addItem(int a){
		itemset i = itemsets.back();
		i.addItem(a);
		itemsets.pop_back();
		itemsets.push_back(i);
		size +=1;
	}

	void Clear(){
		size = 0;
		itemsets.clear();
	}

	int subsequence(vector<itemset> i){
		//// to do 
		return 1;
	}

};


vector<sequence> Data;
vector<vector<sequence> > F;	
int minSupport = 2;

void readInput();
void output(vector<sequence>);
void GSP();
vector<sequence> generateCandidte(int);
vector<sequence> frequent1Seq();

int main(int argc, char const *argv[]){
	readInput();
	output(Data);
	GSP();

	for (int k=0; k<F.size(); k++ ){
		cout << k;
		cout << '\n';
		output(F[k]);
	}
}


void readInput(){
	ifstream fin;
	fin.open("input.txt");
	if(!fin){
		cout<<"Input file opening error\n";
		exit(0);
	}

	sequence s;
	itemset i;
	int n;
	while (fin>>n){

		if (n== -1){
			s.addItemSet(i);
			i.Clear();
			continue;
		}

		if (n== -2){
			s.addItemSet(i);
			Data.push_back(s);
			i.Clear();
			s.Clear();
			continue;
		}

		if (i.notExists(n)){	
			i.addItem(n);
			//cout << n ;
		}

	}

}

void output(vector<sequence> d){
	for ( vector<sequence>::iterator it = d.begin() ; it < d.end(); it++ ){
		vector<itemset> s = it->itemsets;
    	for ( vector<itemset>::iterator it1 =  s.begin(); it1 < s.end() ; it1++ ){
    		vector<int>	i = it1->items;
    		for (vector<int>::iterator it2 = i.begin(); it2 < i.end(); it2++ ){
    			cout << *it2 ;
    			cout << ' ';
    		}
    		cout << ',';
    	}
    	cout << '\n';
    }

}


void GSP(){

	///////// count most frequent items and intialise F[1] with it /////////
	F.push_back(frequent1Seq());
	
	//////// k frequent sequences while loop //////////////
	int k = 2;
	while( !F[k-1].empty() ){

		//////////// generate candidate sequence of k length using F[k-1] ///////////// 
		vector<sequence> C = generateCandidte(k);
		vector<int> supportCount ( C.size(),0);

		/////////// count the support of each candidate sequence ///////////////
		if (!C.empty()){
			for ( vector<sequence>::iterator it = Data.begin() ; it < Data.end(); it++ ){
				int counter = 0;
				for ( vector<sequence>::iterator c1 = C.begin() ; c1 < C.end(); c1++ ){
					if (it->subsequence(c1->itemsets)){
						supportCount[counter] +=1;
					}
					counter +=1;
				}
			}
		}

		///////////// if count > minsupport keep it as frequent sequence //////////////////
		vector<sequence> FK ;
		int counter =0;
		for ( vector<sequence>::iterator c1 = C.begin() ; c1 < C.end(); c1++ ){
				if (supportCount[counter]  >= minSupport){
					FK.push_back(*c1);
				}
				counter +=1;
			}

		F.push_back(FK);

		k+=1;

	}

}



vector<sequence> generateCandidte(int k){

}

vector<sequence> frequent1Seq(){

}
