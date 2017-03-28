
using namespace std;

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <list>


class itemset{
	int size;
	vector<int> items;

public:
	void addItem(int a){
		items.push_back(a);
		size += 1;
	}

	void Clear(){
		items.clear()
	}

	int notExists(int a){
		
	}
};


class sequence {
	int size;
	vector<itemset>  itemsets;
	int id;

public:
	void addItemSet(itemset i ){
		itemsets.push_back(i);
		size += i.size
	}

	void addItem(int a){
		itemset i = itemsets.pop_back();
		i.addItem(a);
		itemsets.push_back(i);
		size +=1
	}
};


vector<sequence> Data;

void readInput();

int main(int argc, char const *argv[]){
	readInput();
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
	while (fin>>n){

		if (n== -1){
			s.addItemSet(i);
			i.Clear();
		}

		if (n== -2){
			s.addItemSet(i);
			Data.push_back(s)
			i.Clear();
			s.Clear();
			continue;
		}

		if (i.notExists()){	
			i.addItem(n)
		}
	}

}
