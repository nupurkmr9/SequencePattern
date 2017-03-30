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

	int isEqual(itemset a){
		for (int k=0; k< size; k++){
			if (k >= a.size){
				return 0;
			}
			else{
				if (!(items[k] == a.items[k])){
					return 0;
				}
			}
		}
		return 1;
	}

	void remove(int itemIndex){
		items.erase (items.begin()+itemIndex);
	}

	itemset copy(){
		itemset i;
		for (int k=0;k<size;k++){
			i.addItem(items[k]);
		}
		return i;
	}

	int isSubset(itemset ii){
		int prev =0;
		for (int i=0;i<ii.size;i++){
			int j;
			for (j=prev; j< size;j++){
				if (ii.items[i] == items[j]){
					prev = j+1;
					break;
				}
				else
					continue;
			}
			if (j>=size && i < ii.size )
				return 0; 
		}
		return 1;
	}

};


class sequence {
public:
	int size=0;
	vector<itemset>  itemsets;
	int id;

	void addItemSet(itemset i ){
		sort(i.items.begin(), i.items.end());
		itemset p = i.copy();
		itemsets.push_back(p);
		size += 1;
	}

	void removeSet(){
		itemsets.pop_back();
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
			// cout <<'m';
			// cout <<'\n';
			vector<int> A = it1->items;
			while(k < size){
				vector<int> B = itemsets[k].items;
				if ( includes(B.begin(), B.end(), A.begin(), A.end()) ) {
					// cout <<'j';
					// cout <<'\n';
					index = k+1;
					break;
				}
				else{
					// cout <<'h';
					// cout <<'\n';
					k +=1;
				}

			}
			if (k >= size){
				// cout << 'i';
				// cout << '\n';
				return 0;
			}
		}
		return 1;
	}

	int isEqual(sequence a){
		int k=0;
		for ( ; k< size; k++){
			if (k>= a.size){
				return 0;
			}
			else{
				if (itemsets[k].isEqual(a.itemsets[k])){
					continue;
				}
				else
					return 0;
			}
		}
		return 1;
	}

	void remove(int itemsetIndex, int itemIndex){
		if (itemsets[itemsetIndex].size > 1)
			itemsets[itemsetIndex].remove(itemIndex);
		else
			itemsets.erase (itemsets.begin()+itemsetIndex);
	}

	sequence copy(){
		sequence c;
		for ( vector<itemset>::iterator it1 = itemsets.begin() ; it1 < itemsets.end(); it1++ ){
			itemset i = it1->copy();
			c.addItemSet(i);
		}
		return c;
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
sequence join(sequence,sequence);
int check(sequence, int );

int main(int argc, char const *argv[]){
	readInput();
	output(Data);
	
	GSP();
	cout << "output";
	cout << '\n';
	for (int k=0; k<F.size(); k++ ){
		cout << "length";
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
	while( !F[k-2].empty() ){

		//////////// generate candidate sequence of k length using F[k-2] ///////////// 
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
	for ( vector<sequence>::iterator ii = F[k-2].begin() ; ii < F[k-2].end(); ii++ ){
		for ( vector<sequence>::iterator tt = F[k-2].begin() ; tt < F[k-2].end(); tt++ ){
			sequence candidate;
			if (k==2){
				sequence c = ii->copy();
				c.addItemSet(tt->itemsets[0]);
				if (!pruned(c,k))
					candidates.push_back(c);
				c = ii->copy();
				c.addItem(tt->itemsets[0].items[0]);
				if (!pruned(c,k))
					candidates.push_back(c);

			}
			else{
				candidate = join(*ii,*tt);
				if (!pruned(candidate,k))
						candidates.push_back(candidate);
			}
		}
	}
	return candidates;
}

sequence join(sequence c1, sequence c2){
	sequence final;
	if (c1.itemsets[0].size == 1 && c1.size == c2.size){
		int k;
		for (k=1 ; k< c1.size; k++ ){
			if (c1.itemsets[k].isEqual(c2.itemsets[k-1]))
				continue;
			else{
				return final;
			}
		}
		sequence c = c1.copy();
		c.addItemSet(c2.itemsets[k-1]);
		return c;

	}
	else if (c1.itemsets[0].size == 1 && c1.size == c2.size+1){
		int k;
		for (k=1 ; k< c1.size-1; k++ ){
			if (c1.itemsets[k].isEqual(c2.itemsets[k-1]))
				continue;
			else{
				return final;
			}
		}
		if (!(c1.itemsets[k].isSubset(c2.itemsets[k-1])))
			return final;

		sequence c = c1.copy();
		c.removeSet();
		c.addItemSet(c2.itemsets[k-1]);
		return c;

	}
	else if (c1.itemsets[0].size > 1 && c1.size == c2.size){
		if (c2.itemsets[0].isSubset(c1.itemsets[0]) && c1.itemsets[0].size +1 == c2.itemsets[0].size ){
			int k;
			for (k=1 ; k< c1.size-1; k++ ){
				if (c1.itemsets[k].isEqual(c2.itemsets[k]))
					continue;
				else{
					return final;
				}
			if (!(c1.itemsets[k].isSubset(c2.itemsets[k])))
				return final;
			}
			sequence c = c1.copy();
			c.removeSet();
			c.addItemSet(c2.itemsets[k]);
			return c;
		}
		else
			return final;

	}
	else if (c1.itemsets[0].size > 1 && c1.size+1 == c2.size){
		if (c2.itemsets[0].isSubset(c1.itemsets[0]) && c1.itemsets[0].size +1 == c2.itemsets[0].size ){
			int k;
			for (k=1 ; k< c1.size; k++ ){
				if (c1.itemsets[k].isEqual(c2.itemsets[k]))
					continue;
				else{
					return final;
				}
			if (!(c1.itemsets[k].isSubset(c2.itemsets[k])))
				return final;
			}
			sequence c = c1.copy();
			c.addItemSet(c2.itemsets[k]);
			return c;
		}
		else
			return final;

	}
	else 
		return final;
}

int pruned(sequence c,int level){

	vector<itemset> i = c.itemsets;
	for ( int k=0; k< i.size(); k++ ){
		if (k==0 || k == i.size()){
			for (int l =0; l< i[k].size; l++){
				sequence c1 = c.copy();
				c1.remove(k,l);
				if (!check(c1,level))
					return 0;
			}
		}
		else{
			if (i[k].size > 1){
				for (int l =0; l< i[k].size; l++){
					sequence c1 = c.copy();
					c1.remove(k,l);
					if (!check(c1,level))
						return 0;
				}
			}
		}
	}
	return 1;
}

int check(sequence c, int level){
	for ( vector<sequence>::iterator ii = F[level-2].begin() ; ii < F[level-2].end(); ii++ ){
		if (!(ii->isEqual(c)))
			return 0;
	}
	return 1;
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
