#include <iostream>
#include <fstream>
using namespace std;


struct customer{
	int customernumber;
	string name;
	double balance;

};



void checkOpen(const ifstream& ,const string file);
int getNumberCustomers(ifstream&);
void getAllCustomerInfo(ifstream&,customer[],const int);

int main(){
	ifstream master_in,transaction_in;
	string cpmaster = "cpmaster.txt";string transaction = "transaction.txt";
	int num_customers;
	
	master_in.open(cpmaster.c_str()); 	
	transaction_in.open(transaction.c_str()); 	

	checkOpen(master_in,cpmaster);
	checkOpen(transaction_in,transaction);		
	
	
	num_customers = getNumberCustomers(master_in);
	customer customer_arr[num_customers];	

	getAllCustomerInfo(master_in,customer_arr,num_customers);

}
int getNumberCustomers(ifstream& fin){
	int numlines=0;	
	string line;

	while(fin){
		getline(fin,line);
		numlines++;
	}
	return numlines;

}

//Check if File is open
void checkOpen(const ifstream &fin,const string file){
	
	if(fin)
		cout<<file<<": Opened Successfully "<<endl;

	
	else	
		cout<<file<<": Failed to open Successfully "<<endl;

}

//Gets info from file to be stored in stored in each customer
void getAllCustomerInfo(ifstream& fin,customer customer_arr[],const int num_customers){
	for(int i = 0;i<num_customers;i++){
		fin >> customer_arr[i].customernumber;
		fin >> customer_arr[i].name;
		fin >> customer_arr[i].balance;	
	}
}

void 
//void customerInvoice(){
//}




