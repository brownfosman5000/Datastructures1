#include <iostream>
#include <fstream>
using namespace std;


struct Customer{
	int customernumber;
	string name;
	double balance;
};

struct Record{
	string transaction_type;
	int transaction_num;
	int num_item;
	string item; 
	double amount;

};
void checkOpen(const ifstream& ,const string file);
int getNumberLines(ifstream&);
void getAllCustomerInfo(ifstream&,Customer[],const int);

void processTransactions(Customer, Record);
void printRecordInfo(Record);


void processCustomer(ifstream&, const int,Customer[], Record);

int main(){
	ifstream master_in,transaction_in;
	string cpmaster = "cpmaster.txt";string transaction = "transaction.txt";
	int num_customers;
	Record customer_record;
	
	master_in.open(cpmaster.c_str()); 	
	transaction_in.open(transaction.c_str()); 	

	checkOpen(master_in,cpmaster);
	checkOpen(transaction_in,transaction);		
	
	//Get number of customers	
	num_customers = getNumberLines(master_in);

	Customer customer_arr[num_customers];	

	getAllCustomerInfo(master_in,customer_arr,num_customers);

	for(int current_customer = 0;current_customer<num_customers;current_customer++)
		processCustomer(transaction_in,current_customer,customer_arr,customer_record); 
}

//Get number of lines in file
int getNumberLines(ifstream& fin){
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
void getAllCustomerInfo(ifstream& fin,Customer customer_arr[],const int num_customers){
	for(int i = 0;i<num_customers;i++){
		fin >> customer_arr[i].customernumber;
		fin >> customer_arr[i].name;
		fin >> customer_arr[i].balance;	
	}
}

void processCustomer(ifstream& fin, const int current_customer,Customer customer_arr[], Record customer_record){
	int customer_number = 0;	
	//Get number of transactions
	int transactions = getNumberLines(fin);
	fin >> customer_number;
	
	//Var to pass around meaning current customer	
	Customer customer = customer_arr[current_customer];

	if (customer_number == customer.customernumber); 
		for(int i = 0; i<transactions;i++){
			fin >> customer_record.transaction_type;
			fin >> customer_record.transaction_num;
			fin >> customer_record.item;
			fin >> customer_record.num_item;
			fin >> customer_record.amount;
			
			processTransactions(customer,customer_record);	
			printRecordInfo(customer_record);
		}
	}

void processTransactions(Customer customer, Record customer_record){
	cout<<customer_record.transaction_type<<endl;
	if (customer_record.transaction_type == "O")
		customer.balance += customer_record.amount;

	else if(customer_record.transaction_type == "P")
		customer.balance -= customer_record.amount;		

	else
		cerr<<"Error Processing customer record type ---  Needs to be either 'O' or 'P'"<<endl;

}

void printRecordInfo(Record current_record ){
	cout<<"Transaction #"<< current_record.transaction_num << current_record.item <<"$"<<current_record.amount << endl; 



}
//void customerInvoice(){}
//bool checkDuplicate(){}
//bool checkRecords(){}
