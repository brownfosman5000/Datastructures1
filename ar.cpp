#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


ifstream master_in,transaction_in;

struct Record{
	string transaction_type;
	int transaction_num;
	int num_item;
	string item; 
	double amount;

};

struct Customer{
	int customernumber;
	string name;
	double balance;
	string address;
	
	Record *record;
};

bool checkOpen(const ifstream& ,const string file);
bool checkDuplicate(ifstream&,string);

int getNumberLines(ifstream&);
void getAllCustomerInfo(ifstream&,Customer[],const int);

void processTransactions(Customer,int,double&);
void customerInvoice(Customer,double);
bool match();

void processCustomer(ifstream&,Customer);
void initRecords(Customer[]);
int main(){
	ifstream master_in,transaction_in;
	string master = "master.txt";string transaction = "transaction.txt";
	int num_customers, num_transactions;
	
	master_in.open(master.c_str()); 	
	transaction_in.open(transaction.c_str()); 	
	


	if(!match())
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
	initRecords(customer_arr);
	getAllCustomerInfo(master_in,customer_arr,num_customers);

	for(int current_customer = 0;current_customer<num_customers;current_customer++)
		processCustomer(transaction_in,customer_arr[current_customer]); 
	
}

void initRecords(Customer arr[]){
	for(int i=0;i<7;i++)
		arr[i].record = new Record[5];
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
void processCustomer(ifstream& fin, Customer customer){
	int customer_number;
	double prev_balance = customer.balance;	
	//Get number of transactions
	fin >> customer_number;
	fin >> customer.address;

	//If current customer number is equal to customer number just read in	
	if (customer_number == customer.customernumber){ 
		//Since it's one customer just use the file in the condtion
		for(int i = 0; i<5;i++){	
				
			fin >> (customer.record[i]).transaction_type;
			fin >> (customer.record[i]).transaction_num;

			if (customer.record[i].transaction_type == "P")
				fin >> (customer.record[i]).amount;
			else if(customer.record[i].transaction_type == "O"){
				fin >> (customer.record[i]).item;
				fin >> (customer.record[i]).num_item;
				fin >> (customer.record[i]).amount;
			}
			processTransactions(customer,i, customer.balance);	
		
		}
		
	}
	else
		cerr<<"Error customer number doesn't match customer number on the transaction record"<<endl;
	
	customerInvoice(customer,prev_balance);
}


// Subtracts or add to balance depends on type of transaction
void processTransactions(Customer customer,int current,double& balance){
	if (customer.record[current].transaction_type== "O")
		balance += customer.record[current].amount;
	else if(customer.record[current].transaction_type == "P")
		balance -= customer.record[current].amount;		

	else
		cerr<<"Error Processing customer record type ---  Needs to be either 'O' or 'P'"<<endl;

}

//Prints invoice to screen
void customerInvoice(Customer customer,double prev_balance ){
	cout<<customer.name<<'\t'<<customer.customernumber<<endl;
	cout<<"\t\tPrevious Balance\t$"<<prev_balance<<endl<<endl;
	
	for(int i = 0;i<5;i++){
		cout<<"Transaction#: "<< (customer.record[i]).transaction_num<<"\t";
		
		if ((customer.record[i]).transaction_type == "O")
			cout<<customer.record[i].item<<"\t";	

		else if((customer.record[i]).transaction_type == "P")
			cout<<"Payment:";

		cout<<"\t$"<<customer.record[i].amount<<endl;
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


bool match(){
	string line,customer_num, currentrecord;
	int numbercustomers = getNumberLines(master_in);	
	string currentrecords[numbercustomers];
	string currentrecords_check[numbercustomers];

		
	int checkforint;


	//Get all of customer numbers in an array
	for(int i = 0; i<numbercustomers;i++){	
		master_in >> customer_num;
		currentrecords[i] = customer_num;
		getline(master_in,line);
	}

	
		
	int i = 0;
	while(transaction_in){
		transaction_in >> line;
		try{	
			
			checkforint= stoi(line);
			
					
		}
		catch(invalid_argument &e){
			getline(transaction_in,line);
			continue;
		}

		currentrecords_check[i] = line;
		i++;
		

		if(i == numbercustomers) break;

	}


	transaction_in.clear();
	transaction_in.seekg(ios_base::beg);

	master_in.clear();
	master_in.seekg(ios_base::beg);

	//Cross check arrays
	for(int i = 0; i<numbercustomers;i++)
		if(currentrecords_check[i] == currentrecords[i])
			cout<<"Match"<<endl;
		else{
			cout<<"No Match for "<<currentrecords_check[i]<<":"<< currentrecords[i]<<endl;
			return false;	
		}
			
	return true;			
		


}
