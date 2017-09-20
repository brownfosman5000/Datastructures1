#include <iostream>
#include <fstream>
#include <cstdlib>
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
bool checkOpen(const ifstream& ,const string file);
bool checkDuplicate(ifstream&,string);

int getNumberLines(ifstream&);
void getAllCustomerInfo(ifstream&,Customer[],const int);

void processTransactions(double&, Record);
void customerInvoice(Customer,Record[],double);
bool match(ifstream&,ifstream&);

void processCustomer(ifstream&,Customer, Record[]);

int main(){
	ifstream master_in,transaction_in;
	string master = "master.txt";string transaction = "transaction.txt";
	int num_customers, num_transactions;
	
	master_in.open(master.c_str()); 	
	transaction_in.open(transaction.c_str()); 	
	


	if(!match(transaction_in,master_in))
		exit(0);
	if(!checkOpen(master_in,master))
		exit(0);		
	if(!checkOpen(transaction_in,transaction))		
		exit(0); 
 	
	if(checkDuplicate(master_in,master))
		exit(0);

	//Get number of customers	
	num_customers = getNumberLines(master_in);
	num_transactions = getNumberLines(transaction_in);
	Customer customer_arr[num_customers];	
	Record customer_record[num_transactions];

	getAllCustomerInfo(master_in,customer_arr,num_customers);

	for(int current_customer = 0;current_customer<num_customers;current_customer++)
		processCustomer(transaction_in,customer_arr[current_customer],customer_record); 
	
}

//Get number of lines in file or use upto to specify where to stop
int getNumberLines(ifstream& fin){
	int numlines=0;	
	string line;

	while(fin){
		//Make sure not to read an extra line
		getline(fin,line);
		numlines++;
	}

	fin.clear();
	fin.seekg(ios_base::beg);
	//getline will return an extra line because of \n in input stream
	return numlines-1;

}

//Check if File is open
bool checkOpen(const ifstream &fin,const string file){
	
	if(fin){
		cout<<file<<": Opened Successfully "<<endl;
		return true;
	}
	
	else{	
		cout<<file<<": Failed to open Successfully "<<endl;
		return false;
	}
}

//Gets info from file to be stored in stored in each customer
void getAllCustomerInfo(ifstream& fin,Customer customer_arr[],const int num_customers){
	for(int i = 0;i<num_customers;i++){
		fin >> customer_arr[i].customernumber;
		fin >> customer_arr[i].name;
		fin >> customer_arr[i].balance;	
	}
}

//Reads in customer transaction records processes and prints an invoice for that customer
void processCustomer(ifstream& fin, Customer customer, Record customer_record[]){
	int customer_number;
	double prev_balance = customer.balance;	
	//Get number of transactions
	//int num_transactions = getNumberLines(fin);
	fin >> customer_number;
	
	//If current customer number is equal to customer number just read in	
	if (customer_number == customer.customernumber){ 
		//Since it's one customer just use the file in the condtion
		for(int i = 0; i<5;i++){	
		
			fin >> (customer_record[i]).transaction_type;
			fin >> (customer_record[i]).transaction_num;

			if (customer_record[i].transaction_type == "P")
				fin >> (customer_record[i]).amount;
			else if(customer_record[i].transaction_type == "O"){
				fin >> (customer_record[i]).item;
				fin >> (customer_record[i]).num_item;
				fin >> (customer_record[i]).amount;
			}
			processTransactions(customer.balance,(customer_record[i]));	
		
		}
		
	}
	else
		cerr<<"Error customer number doesn't match customer number on the transaction record"<<endl;
	
	customerInvoice(customer,customer_record,prev_balance);
}


// Subtracts or add to balance depends on type of transaction
void processTransactions(double& balance , Record customer_record){
	if (customer_record.transaction_type == "O")
		balance += customer_record.amount;

	else if(customer_record.transaction_type == "P")
		balance -= customer_record.amount;		

	else
		cerr<<"Error Processing customer record type ---  Needs to be either 'O' or 'P'"<<endl;

}

//Prints invoice to screen
void customerInvoice(Customer customer,Record current_record[],double prev_balance ){
	cout<<customer.name<<'\t'<<customer.customernumber<<endl;
	cout<<"\t\tPrevious Balance\t$"<<prev_balance<<endl<<endl;
	
	for(int i = 0;i<5;i++){
		cout<<"Transaction#: "<< (current_record[i]).transaction_num<<"\t";
		
		if ((current_record[i]).transaction_type == "O")
			cout<<current_record[i].item<<"\t";	

		else if((current_record[i]).transaction_type == "P")
			cout<<"Payment:";

		cout<<"\t$"<<current_record[i].amount<<endl;
	}
	
	cout<<"\t\t"<<"Balance Due: \t$"<<customer.balance<<endl;	
	
	cout<<endl<<endl<<endl;	

}

//Check for duplicate master files
bool checkDuplicate(ifstream& fin,const string file){
	string line,check;
	bool isduplicate = false;	
	getline(fin,line);
	while(fin){
		
		getline(fin,check);
		if(check == line){
			cerr<<"Error duplicate records in "<<file<<endl;
			isduplicate = true;
		}
		else{		
			line = check; 	
			isduplicate = false;
		}
	}
	fin.clear();
	fin.seekg(ios_base::beg);
	return isduplicate; 


}


bool match(ifstream& transactionin, ifstream& masterin){
	string line,customer_num, customer_record;
	int numbercustomers = getNumberLines(masterin);	
	string customer_records[numbercustomers];
	string customer_records_check[numbercustomers];

		
	int checkforint;


	//Get all of customer numbers in an array
	for(int i = 0; i<numbercustomers;i++){	
		masterin >> customer_num;
		customer_records[i] = customer_num;
		getline(masterin,line);
	}

	
		
	int i = 0;
	while(transactionin){
		transactionin >> line;
		try{	
			
			checkforint= stoi(line);
			
					
		}
		catch(invalid_argument &e){
			getline(transactionin,line);
			continue;
		}

		customer_records_check[i] = line;
		i++;
		

		if(i == numbercustomers) break;

	}


	transactionin.clear();
	transactionin.seekg(ios_base::beg);

	masterin.clear();
	masterin.seekg(ios_base::beg);

	//Cross check arrays
	for(int i = 0; i<numbercustomers;i++)
		if(customer_records_check[i] == customer_records[i])
			cout<<"Match"<<endl;
		else{
			cout<<"No Match for "<<customer_records_check[i]<<":"<< customer_records[i]<<endl;
			return false;	
		}
			
	return true;			
		


}
