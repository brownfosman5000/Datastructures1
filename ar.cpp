#include <iostream>
#include <fstream>
using namespace std;


void checkOpen(ifstream &fin,string file);



int main(){
	ifstream master_in,transaction_in;
	string cpmaster = "cpmaster.txt";string transaction = "transaction.txt";
	
	master_in.open(cpmaster.c_str()); 	
	transaction_in.open(transaction.c_str()); 	
	
	checkOpen(master_in,cpmaster);
	checkOpen(transaction_in,transaction);		

		

}


//Check if File is open
void checkOpen(const ifstream &fin,const string file){
	
	if(fin)
		cout<<file<<": Opened Successfully "<<endl;

	
	else	
		cout<<file<<": Failed to open Successfully "<<endl;
	



}


void customerInvoice(){





}
