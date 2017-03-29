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
		sort(i.items.begin(), i.items.end());
		itemsets.push_back(i);
		size += 1;
	}

	void addItem(int a){
		itemset i = itemsets.back();
		i.addItem(a);
		itemsets.pop_back();
		itemsets.push_back(i);
		//size +=1;
	}

	void Clear(){
		size = 0;
		itemsets.clear();
	}

	int subsequence(vector<itemset> i){
		int index = 0;
		for ( vector<itemset>::iterator it1 = i.begin() ; it1 < i.end(); it1++ ){
			int k = index;
			cout <<'m';
			cout <<'\n';
			vector<int> A = it1->items;
			while(k < itemsets.size()){
				vector<int> B = itemsets[k].items;
				if ( includes(B.begin(), B.end(), A.begin(), A.end()) ) {
					cout <<'j';
					cout <<'\n';
					index = k+1;
					break;
				}
				else{
					cout <<'h';
					cout <<'\n';
					k +=1;
				}

			}
			if (k >= itemsets.size()){
				cout << 'i';
				cout << '\n';
				return 0;
			}
		}
		return 1;
	}

};


#define structure map<int, int>
vector<sequence> Data;
vector<vector<sequence> > F;	
structure Allitems;
int minSupport = 2;

void readInput();
void output(vector<sequence>);
void GSP();
vector<sequence> generateCandidte(int);
vector<sequence> frequent1Seq();
int pruned(sequence,int);
vector<sequence> join(sequence,sequence);

int main(int argc, char const *argv[]){
	readInput();
	output(Data);
	//int m = Data[7].subsequence(Data[1].itemsets);
	//cout << m;
	//cout << '\n';
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
	structure done;
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
			done.clear();
			continue;
		}

		if (i.notExists(n)){	
			i.addItem(n);
			//cout << n ;
		}
		
		if (!(done.count(n) > 0)){
			if(Allitems.count(n)>0)
				Allitems[n]++;
			else
				Allitems[n]=1;
			done[n]=1;
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
	
	//////// k frequent sequence while loop //////////////
	int k = 2;
	while( !F[k-1].empty() ){
		// cout << 'k';
		// cout << '\n';

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

		///////////// if count > minSupport keep it as frequent sequence //////////////////
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
	vector<sequence> candidates;
	for ( vector<sequence>::iterator ii = F[k-1].begin() ; ii < F[k-1].end(); ii++ ){
		for ( vector<sequence>::iterator tt = F[k-1].begin() ; tt < F[k-1].end(); tt++ ){
			vector<sequence> candidate = join(*ii,*tt);
			if (candidate.size() > 0){
				for ( vector<sequence>::iterator cc = candidate.begin() ; cc < candidate.end(); cc++ ){
					if (!pruned(*cc,k)){
						candidates.push_back(*cc);
					}
				}
			}
		}
	}
	return candidates;
}

vector<sequence> join(sequence c1, sequence c2){
	// final 
	// temp c11 = c1
	//if first itemset has >1 element of c11 :
	//		for each item in c11.itemset[0]:
	//			c11 = remove item from c11.itemset[0]: 
	//				match = 1
	//				for i =0 to c1.itemset.size -2 :
	//					if 	c1.itemset[i] == c2.itemset[i-1]:
	//						continue
	//					else:
	//						match = 0
	//						break
	//				if match and (c11.itemset[i] is subset of c2.itemset[i])
	//					final.push_back( merge of c1 c2)		
	//			
	//				
	//else:
	//	match = 1
	//	for i =1 to c1.itemset.size -2 :
	//		if 	c1.itemset[i] == c2.itemset[i-1]:
	//			continue
	//		else:
	//			match = 0
	//			break
	//	if match and (c11.itemset[i] is subset of c2.itemset[i])
	//		final.push_back(merge of c1 and c2)
}

int pruned(sequence c,int k){
	sequence temp;
	//define equality operator for sequence object
	//define remove item for sequence class
	//define remove item for itemset class
	//for loop on each itemset of c 
	//	first and last itemset of c ignore the >2 size ow check if itemset size >2
	//	remove each element of itemset one by one
	//	check if the resulting sequence is contained in F[k-1]
	//	if not return 1
	//	else go on
	//return 0 
}

vector<sequence> frequent1Seq(){
	vector <sequence> F1 ;
	for ( structure::iterator ii = Allitems.begin() ; ii != Allitems.end(); ii++ ){
		if (ii->second >= minSupport){
			itemset i;
			i.addItem(ii->first);
			sequence s;
			s.addItemSet(i);
			F1.push_back(s);
		}
	}
	return F1;

}
