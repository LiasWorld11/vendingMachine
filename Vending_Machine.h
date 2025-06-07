#ifndef _VENDING_MACHINE_
#define _VENDING_MACHINE_

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <limits>
#include <windows.h>
#include <fstream>
#include "Item.h"

using namespace std;

class VendingMachine {
	private:
		Item* itemArray;   		//dynamic array of Items
		int itemArraySize;  	//user input size
		int maxSize;        	//maximum size of queues, 5
		int numQueue;      		//number of queues in the entire VM
		
		int totalStock;     	//total items inside VM, eg. 5 items * 20 stock = 100 items total
		double totalMoney;  	//the total amount of cash inside VM
	    string machineTitle;    //title of the vending machine
		
	public:
		VendingMachine(int size);                            //constructor
		~VendingMachine();                                   //destructor
		void addItem(const Item& item);                      //add item to vending machine
		
		void mainMenu(); 					   			     //menu to select 3 options
		
		//show Items 
		void printItems() const;                             //print the vending machine interface like in question
		void selectItem();                                   //select an item from the vending machine
		
		bool makePayment(int itemOpt);                       //for customers to pay for selected item
		void refundPayment(double &totalPaid);  		     //for unsuccessful transaction or return balance
		void moneyChecker(double &totalPaid);                //for checking the money
		
		//inventory Admin
		void logRegMenu();									 //menu for login and registration
		void login();										 //login 
		void registration();								 //register
		void exportFile();									 //export file 
		void adminMenu();       		       			     //admin menu for managing vending machine
		void replenishStock();                               //add additional stocks to an item
	    void dispSummary();                                  //display the summary of stock and available funds in the machine
	    void resetStock();								     //reset all the stock to zero (SOLD OUT)
	    void changePrice();                                  //change the price of an item
	    void changeName();		     						 //change the title and stock header name
	    void addFunds();   			                   	     //add funds to the vending machine
    	 
	    void reset();						  	 	         //clear screen purpose 

	    //methods to interact with item
	    string getItemName(const Item& item) const;          //get the name of an item
	    char getItemChar(const Item& item) const;            //get the character representation of an item
	    int getItemStock(const Item& item) const;            //get the stock of an item
	    
	    bool isEmpty() const; 					  	   	    //check if vending machine is empty
	    bool isFull() const;  					  		    //check if vending machine is full
};

//------------------------------------------------------------------constructor & destructor-----------------------------------------------------------------
VendingMachine::VendingMachine(int size) {
	//initialize number of item queue in the vending machine
	maxSize = 5;
	itemArraySize = size;
	itemArray = new Item[itemArraySize];
	numQueue = 0;
	
	//cannot run if size is over maximum 
	if (itemArraySize > maxSize) {
		throw invalid_argument ("Invalid Size!"); 
	}
	
	//initialize stock and money
	totalStock = 0;
	totalMoney = 0.00;
	machineTitle = "INTI Vending Machine";
} 

VendingMachine::~VendingMachine() {
	delete[] itemArray;
}

void VendingMachine::addItem(const Item& item) {
	
    if (isFull()) {
        cout << "Vending machine is full, cannot add more items!\n";
        return;
    }
    else {
    	itemArray[numQueue] = item;
    	numQueue++;
    	totalStock += item.getNumStockQ();
	}
}

//-----------------------------------------------------------------menu------------------------------------------------------------------
void VendingMachine::mainMenu() {
	int option1;

	do {
		cout << "\n\n";
		cout << "\t\t\t\t ====================\n";
		cout << "\t\t\t\t      WELCOME TO\n\n";
		cout << "\t\t\t\t IICP VENDING MACHINE\n";
		cout << "\t\t\t\t ====================\n\n";
		cout << "\t\t\t\t1 " << left << setw(18) << "[Show Items" << "]\n";
		cout << "\t\t\t\t2 " << left << setw(18) << "[Inventory-Admin" << "]\n";
		cout << "\t\t\t\t3 " << left << setw(18) << "[Exit" << "]\n\n";
		cout << "\t\t\tEnter Option: ";
		cin >> option1;
		cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
		
		switch(option1) {
			case 1:
				reset();
				printItems();
				selectItem();
				break;
			case 2:
				reset();
				logRegMenu();
				adminMenu(); 
				break;
			case 3:
				cout << "\n\n\n\t\t\t\t========================\n";
				cout << "\t\t\t\tEXITED. HAVE A NICE DAY!\n";
				cout << "\t\t\t\t========================\n\n";
				exit(0);
				break;
			default:
				//Case 1: if user enter double, char or string
				if (cin.fail()){
					reset();
					cout << "\t\t\t\t    INVALID OPTION\n";
					cout << "\t\t\t     PLEASE ENTER A VALID OPTION\n";
					cin.clear(); //clear failbit and ignore remaining input
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
				}
				//Case 2: if user enter int not which is not in range
				else{
					reset();
					cout << "\t\t\t\t    INVALID OPTION\n";
					cout << "\t\t\t     PLEASE ENTER A VALID OPTION\n";
				}
		}
	} while (option1 != 3);	
}

void VendingMachine::printItems() const {
	
	cout << "\n\n\t\t\t\t\t\t[" << machineTitle << "] \n\n\n";
	
    int maxSize = itemArray[0].getMaxSize(); //all items have same max stock size, 20
    int nLength[numQueue];                   //array to store lengths of item names
    int midpoint[numQueue];                  //array to store midpoints of item names
    int blank = 0;
    
    cout << "\t\t";
    for (int i=0; i<numQueue; i++) {
    	//get item name length and the midpoint of the name
    	string name = getItemName(itemArray[i]) + " [" + to_string(i+1) + "] ";
    	nLength[i] = name.length();
		midpoint[i] = name.length() / 2;
    	
    	//display item name
    	cout << left << setw(15) << name << string(5,' ');
	} cout << endl << "\t\t";
	
	//display dashes
	for (int i=0; i < 20*numQueue-10; i++) {
		cout << "-";
	} cout << endl;
	
	//print characters representing item stock
	for (int i=0; i<maxSize; i++) { //for number of rows
		cout << "\t\t";
		for (int j=0; j<numQueue; j++) { //for types of items
			int stock = getItemStock(itemArray[j]); //get current stock of the item
			blank = maxSize - stock; //calculate number of blanks
			
			//if there is no stock
			if (stock == 0){
				if (i == 0){
					cout << left << setw(15) << setfill(' ') << "<Out of Stock> " << string(5, ' ');
				}
				else {
					cout << string(20, ' ');
				}
			}
			//if in stock
			else {
				if (i < blank) {
					cout << string(midpoint[j], ' ') << " " << string(19-midpoint[j], ' ');
				}
				else {
					cout << string(midpoint[j],' ') << getItemChar(itemArray[j]) << string(19-midpoint[j], ' ');
				}				
			}
		} cout << endl;
	}
	cout << endl;  
}

void VendingMachine::selectItem() { 
	char ctnShopping;          //variable to store user input for continuing shopping
	bool transSuccess = false; //flag to track if transaction was successful
	
	cout << "\n\n\nChoose Item Index to Purchase [1 - " << numQueue << "]\n";
    cout << "Return to Main Menu [" << numQueue + 1 << "]\n\n";
	
	do {
		int itemOpt;            //variable to store user's menu choice
		bool validOpt = false;  //flag to validate user input
		
		do { 
			cout << "Enter Option" << ": ";
			cin >> itemOpt;
			
			//Case 1: enter a valid choice
			if (itemOpt >= 1 && itemOpt <= numQueue) {
				//Case 1a: if item is out of stock
				if (itemArray[itemOpt - 1].getNumStockQ() == 0) {
					validOpt = false;
					reset();
					printItems();
				    cout << "!!!OUT OF STOCK!!!\n\n";	
				}
				//Case 1b: if item is in stock, continue with making payment
				else {
					reset();
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n'); // Clear input buffer
                    transSuccess = makePayment(itemOpt); // Attempt to make payment for selected item
                    validOpt = true; // Valid option selected, exit loop
                }
            }
            //Case 2: exit to main menu
            else if (itemOpt == numQueue + 1) {
            	reset();
				mainMenu(); // Return to main menu
			}
			//Case 3: invalid choice
			else {
				//Case 3a: if user enters non-integer input
				if (cin.fail()) {
					reset();
					printItems();
					cout << "INVALID OPTION\n";
					cout << "PLEASE ENTER A VALID OPTION\n\n";
					cin.clear(); // Clear failbit and ignore remaining input
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
				}
				//Case 3b: if user enters integer not in range
				else {
					reset();
					printItems();
					cout << "INVALID OPTION\n";
					cout << "PLEASE ENTER A VALID OPTION\n\n";
				}
            }
		} while (!validOpt); //repeat until a valid option is chosen
		
		//if transaction was successful, prompt user to continue shopping
		if (transSuccess) {
            do {
                cout << "Buy Another Item (Y/N): ";
                cin >> ctnShopping;
                ctnShopping = toupper(ctnShopping);

                if (ctnShopping != 'Y' && ctnShopping != 'N') {
                	reset();
                    cout << "INVALID INPUT\n";
                    cout << "PLEASE ENTER ONLY 'Y' OR 'N'\n\n";
                }
            } while (ctnShopping != 'Y' && ctnShopping != 'N');

            if (ctnShopping == 'Y') {
                reset();
                printItems(); //display items again if user wants to buy another item
            }
        }
    } while (toupper(ctnShopping) == 'Y'); //repeat while user wants to buy another item
	
	//display thank you message if transaction was successful
	if (transSuccess) {
		reset();
    	cout << setw(23) << setfill('*') << "\t\t\t*" << " THANK YOU " << setw(20) << setfill('*') << "*" << setfill(' ') << endl;
	    cout << "\t\t\t ENJOY YOUR PURCHASED ITEM AND COME BACK NEXT TIME\n";
	    cout << setw(54) << setfill('*') << "\t\t\t*" << setfill(' ') << endl << endl;
	    Sleep(2000); //pause for 2 seconds
	    reset();
	    mainMenu(); //return to main menu after thanking the user
	}
}
	
bool VendingMachine::makePayment(int itemOpt) {
    double price = itemArray[itemOpt - 1].getPrice(); //get price of the selected item
    double totalPaid = 0.0;                           //initialize total amount paid by the user
    double money;                                     //variable to store each amount entered by the user
    
    printItems(); //display items to the user
    
    //continue loop until the total paid is sufficient to cover the price
    while (totalPaid < price) {
        cout << "Please Pay RM " << fixed << setprecision(2) << price - totalPaid << " to Purchase Item\n\n";
        cout << "Enter Amount: RM ";
        cin >> money;
        
        if (money <= 0) {
        	cin.clear(); // Clear failbit and ignore remaining input
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "\n\nINVALID AMOUNT! PLEASE ENTER A VALID VALUE!\n";
            Sleep(1000);
            reset();
            printItems();
            continue; //continue to prompt for valid input if neg value entered
        }
        
        totalPaid += money; //accumulate the total amount paid by the user
        reset(); 
        
        moneyChecker(totalPaid); //check the money paid so far
        
        if (totalPaid < price) {
            cout << "!!!INSUFFICIENT AMOUNT!!!\n";
            char reply;
            
            //prompt user to cancel transaction if amount is insufficient
            do {
                cout << "Cancel Transaction (Y/N): ";
                cin >> reply;
                reply = toupper(reply);
        
                if (reply != 'Y' && reply != 'N') {
                    reset();
                    cout << "INVALID INPUT\n";
                    cout << "PLEASE ENTER ONLY 'Y' OR 'N'\n\n";
                }
            } while (reply != 'Y' && reply != 'N'); 
        
            if (reply == 'Y') {
                reset();
                refundPayment(totalPaid); //refund the amount paid
                return false; //transaction cancelled
            }
            cout << endl;
        }
    }
    
    //payment successful, now check if change can be provided
    double change = totalPaid - price;
    
    if (change > totalMoney) {
        cout << "!!!SORRY, NOT ENOUGH CHANGE IN MACHINE!!!\n";
        Sleep(1800); 
        reset();
        refundPayment(totalPaid); //refund the amount paid
        return false; //cannot provide change, transaction failed
    }
    
    //able to provide change, proceed with the transaction
    totalMoney += price - change; //update the machine's total money
    
    reset(); 
    
    //update item stock and total stock count
    Item item;
    itemArray[itemOpt - 1].removeStockFromQ(item);
    totalStock--;
    
    printItems(); //display items after transaction
    
    cout << "!!!PAYMENT SUCCESSFUL!!!\n";

    if (change > 0) {
        cout << "Please Collect Your Change RM " << fixed << setprecision(2) << change << endl << endl;
    } 
    return true; //transaction successful
}

//refund payment if transaction is cancelled
void VendingMachine::refundPayment(double &totalPaid) {
    cout << setw(10) << "\t\t\t\t*********" << " REFUNDED " <<  setw(9) << setfill('*') << "*" << setfill(' ') << endl; 
	cout << setw(7) << "\t\t\t\t " << " TRANSACTION CANCELLED\n";
	cout << setw(4) << "\t\t\t\t " << " PLEASE COLLECT YOUR MONEY\n";
    cout << setw(32) << setfill('*') << "\t\t\t\t*" << setfill(' ') << endl << endl;
    
    Sleep(2000);
    reset();
    mainMenu();
    
    totalPaid = 0.0; //reset the total paid amount
}

//check the money inserted
void VendingMachine::moneyChecker(double &totalPaid) {                               //member variable to keep track of total money inserted
    int ringgitCount = static_cast<int>(totalPaid); 					             //extract whole part (ringgit)
    int centsCount = static_cast<int>(round((totalPaid - ringgitCount) * 100));      //extract cents part
	
	printItems();
	
    //print breakdown of inserted money
    cout << "SUMMARY OF INSERTED MONEY\n";
    cout << setw(30) << setfill('-') << "-" << setfill(' ') << endl;
    cout << setw(14) << "Ringgit" << ": " << ringgitCount << endl;
    cout << setw(14) << "Cents" << ": "<< centsCount << endl;
    cout << setw(30) << setfill('-') << "-" << setfill(' ') << endl;
    cout << "Total Inserted: RM " << fixed << setprecision(2) << totalPaid << endl;  //display total money inserted
    cout << setw(30) << setfill('-') << "-" << setfill(' ') << endl << endl;
}
 
//-----------------------------------------------------------------admin menu-----------------------------------------------------------------
void VendingMachine::logRegMenu() {
	int c;
	 
	do {
		cout << "\t\t\t\tPress 1 to LOGIN : " << endl;
		cout << "\t\t\t\tPress 2 to REGISTER : " << endl;
		cout << "\t\t\t\tChoice => ";
		cin >> c;
		
		cout<<"\n";
		
		switch(c) {
			case 1:
				reset();
				login();
				break;
			case 2:
				reset();
				registration();
				break;
			default:
				cout << "\t\t\t\tINVALID OPTION\n";
				cout << "\t\t\t\tPLEASE ENTER A VALID OPTION\n\n";
		}	  
	} while (c != 1 && c != 2);
}

void VendingMachine::registration() {
	string ruserid,rpassword;
	reset();
	cin.ignore();
	
	cout<<"\t\t\t\tEnter the Username: ";
	getline(cin,ruserid);
	
	cout<<"\t\t\t\tEnter the Password: ";
	getline(cin,rpassword);
	
	ofstream fl("records.txt");
	fl << ruserid << ' ' << rpassword << endl;
	fl.close();
	
	reset();
	cout << "\t\t\t\tRegistration is Successful!\n\n";	 
	logRegMenu();
}

void VendingMachine::login() {
	int count = 0;
	string password,userid,id,pass;
	
	cout << "\t\t\t\tPlease Enter the Username & Password \n\n";
	cin.ignore();
	
	cout << "\t\t\t\tUSERNAME: ";
	getline(cin,userid);
	
	cout << "\t\t\t\tPASSWORD: ";
	getline(cin,password);
	
	ifstream input("records.txt"); //read data from file
	
	while (input >> id >> pass) {  //check matching
		if (id == userid && pass == password) {
			count=1;
			reset();
		}
		 
	} input.close();
		
	if (count == 1) {
		cout << "\t\t\t\t\tWelcome, " << userid << "." << "\n" << "\t\t\t\tYour LOGIN is SUCCESSFUL!\n";
		exportFile();
		Sleep(2000);
		reset();
		adminMenu();
	}
	else {
		reset();
		cout << "\n" << "\t\t\t\tLOGIN ERROR" << "\n" << "\t\t\t\tPlease Check Again\n\n";
		Sleep(2000);
		reset();
		logRegMenu();
	}
}

void VendingMachine::exportFile() {
    ifstream input("records.txt");
    ofstream output("exported_records.txt");

    string id, pass;
    
    while (input >> id >> pass) {
        output << "Username: " << id << ", Password: " << pass << endl;
    }

    input.close();
    output.close();

    cout << "\t\tUser data has been successfully exported to 'exported_records.txt'." << endl;
    
}

void VendingMachine::adminMenu() {
	int option2;
	
	do {
		cout << "\n\n";
		cout << "\t\t\t\t    ==========\n";
		cout << "\t\t\t\t    Admin Menu\n";
		cout << "\t\t\t\t    ==========\n\n";
		cout << "\t\t\t1 " << left << setw(35) << "[Replenish Stock" << "]\n";
		cout << "\t\t\t2 " << left << setw(35) << "[Display Machine Summary" << "]\n";
		cout << "\t\t\t3 " << left << setw(35) << "[Reset Stock" << "]\n";
		cout << "\t\t\t4 " << left << setw(35) << "[Change Price" << "]\n";
		cout << "\t\t\t5 " << left << setw(35) << "[Change Title or Stock Header Name" << "]\n";
		cout << "\t\t\t6 " << left << setw(35) << "[Allocate Extra Funds" << "]\n";
		cout << "\t\t\t7 " << left << setw(35) << "[Return to Main Menu" << "]\n\n";
		cout << "\t\t\t\tEnter Option: ";
		cin >> option2;

		switch(option2) {
			case 1:
				reset();
				replenishStock(); 
				break;
			case 2:
				reset();
				dispSummary();
				break;
			case 3:
				reset();
				resetStock();
				break;
			case 4: 
				reset();
				changePrice();
				break; 
			case 5:
				reset();
				changeName();
				break; 
			case 6:
				reset();
				addFunds();
				break;
			case 7:
				reset();
				mainMenu();
				break;
			default:
				reset();
				cout << "\t\t\t\t      INVALID OPTION\n";
				cout << "\t\t\t\tPLEASE ENTER A VALID OPTION\n";
		} 
	} while (option2 != 7);	
}

void VendingMachine::replenishStock() {
    int itemIndex, stock;
    bool valid = false;
    
    do { //loop while the input for which item is invalid and exit after the input is valid
    	printItems();
        //choose item to replenish stock or exit to menu
        cout << "\t\t\t\tEnter Item Index to Replenish Stock [1-" << numQueue << "]\n";
        cout << "\t\t\t\tReturn to Admin Menu [" << numQueue + 1 << "]\n\n";
        cout << setw(32) << "\t\t\t\tEnter Option" << ": ";
        cin >> itemIndex;
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //same bug as menu
        
        //if the input is a char, double or string
        if (cin.fail()){
        	cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(),'\n');
            reset();
            cout << "\t\t\t\tINVALID OPTION\n";
            cout << "\t\t\t\tPLEASE ENTER A VALID OPTION\n\n";
        }//if the input is correct datatype but out of range
        else if (itemIndex < 1 || itemIndex > numQueue + 1) {
        	reset();
            cout << "\t\t\t\tINVALID OPTION\n";
            cout << "\t\t\t\tPLEASE ENTER A VALID OPTION\n\n";
            replenishStock();
        }//valid input
        else {
            valid = true;
        }
    } while (!valid);

    //Case 1: user choose to exit
    if (itemIndex == numQueue + 1) {
		reset();
        return adminMenu();
    }
    
    //Case 2: valid input for item index, prompt user to enter how much stock to add
    valid = true;
    do {
    	reset();
        cout << setw(32) << "\t\t\t\tEnter Additional Stock Quantity" << ": ";
        cin >> stock;
        
        //Case 1: invalid input -- double, char, string
        if (cin.fail()){
            reset();
            cout << "\t\t\t\tINVALID OPTION\n";
            cout << "\t\t\t\tPLEASE ENTER A VALID OPTION\n\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            valid = false; 
            Sleep(1500);
        }
        //Case 2: value input is negative number or 0
        else if (stock <= 0) {
            //Case 2a: value input is zero
            if (stock == 0) {
                cout << "\t\t\t\t!!!NOTHING ADDED!!!\n\n";
                valid = true;
                Sleep(1500);
            } 
            //Case 2b: value input is negative number
            else {
                cout << "\t\t\t\tINVALID QUANTITY\n";
                cout << "\t\t\t\tPLEASE ENTER A VALID QUANTITY\n\n";
                valid = false;
                Sleep(1500);
            }
        }
        //Case 3: Valid input for how many added items
        else {
            Item& item = itemArray[itemIndex - 1];
            int addedCount = item.addStockToQ(stock);  
            
            if (addedCount > 0) {
                cout << "\t\t\t\t!!!STOCK REPLENISHED SUCCESSFULLY!!!\n\n";
                totalStock += addedCount;
            }
            valid = true;
        }
    } while (!valid);

    Sleep(2000);
    reset();
    adminMenu();
}

void VendingMachine::dispSummary() {
    char exit; //variable to store user's exit choice

    cout << "MACHINE SUMMARY\n";
    cout << setw(35) << setfill('-') << "-" << setfill(' ') << endl;

    //display each item's name and current stock
    for (int i=0; i<numQueue; i++) {
        cout << left << setw(15) << getItemName(itemArray[i]) << ": " << right << setw(2) << getItemStock(itemArray[i]) << " in stock\n";
    }
    
    //calculate and display total stock across all items
    int calculatedTotalStock = 0;
    for (int i=0; i<numQueue; i++) {
        calculatedTotalStock += getItemStock(itemArray[i]);
    }
    
    //display total stock and total money in the machine
    cout << setw(35) << setfill('-') << "-" << setfill(' ') << endl;
    cout << left << setw(15) << "Total Stock" << ": " << calculatedTotalStock << endl;
    cout << left << setw(15) << "Total Money" << ": " << fixed << setprecision(2) << totalMoney << endl;
    cout << setw(35) << setfill('-') << "-" << setfill(' ') << endl << endl;

    //prompt user to return to admin menu
    do {
        cout << "Return to Admin Menu (Y): ";
        cin >> exit;
        exit = toupper(exit);

        if (exit != 'Y') {
            reset();
            cout << "PLEASE ENTER 'Y' TO EXIT\n\n";
            dispSummary(); //display summary again if invalid input
        }
        else {
            reset();
            adminMenu(); //return to admin menu
        }
    } while (exit != 'Y');
}

void VendingMachine::resetStock() {
	char confirmation; //variable to store users confirmation choice
	
	cout << "Reset All Stock to 0? (Y/N): ";
	cin >> confirmation;
	confirmation = toupper(confirmation); //convert input to uppercase

	if (confirmation == 'Y' || confirmation == 'N') {
		reset(); //clear screen and reset display
		
		if (confirmation != 'Y') {
			cout << "\t\t\t    !!!STOCK RESET OPERATION CANCELLED!!!\n\n";
		}
		else {
			//reset stock of all items to 0
			for (int i = 0; i < numQueue; i++) {
				itemArray[i].clearItemQ();
			}
			
			totalStock = 0; //reset total stock counter
			
			cout << "\t\t\t    !!!ALL STOCK HAS BEEN RESET TO 0!!!\n\n";
		}
	}
	else {
		reset(); 
		cout << "PLEASE ENTER ONLY 'Y' OR 'N'\n\n";
		resetStock(); //prompt user again if input is invalid
	}
	
	Sleep(2500);
    reset();
    adminMenu();
}

void VendingMachine::changePrice() {
	int itemIndex;   //variable to store user's item index choice
	double newPrice; //variable to store new price input
	
	printItems();    //display current items and their prices
	
	cout << "\t\t\t\tEnter Item Index to Change Price [1-" << numQueue << "]\n";
	cout << "\t\t\t\tReturn to Admin Menu [" << numQueue + 1 << "]\n\n";
	cout << setw(15) << "\t\t\t\tEnter Option" << ": ";
	cin >> itemIndex; //get user's choice for item index
	
	if (itemIndex == numQueue+1) {
		reset();
		adminMenu(); //return to admin menu if user chooses to exit
	}
	
	else if (itemIndex < 1 || itemIndex > numQueue) {
		reset();
        cout << "\t\t\t\t\t           INVALID OPTION\n";
		cout << "\t\t\t\t\t     PLEASE ENTER A VALID OPTION\n\n";
		changePrice(); //prompt user again if input is invalid
	}
	
	cout << setw(15) << "\t\t\t\tEnter New Price" << ": RM ";
	cin >> newPrice; //get userr input for new price
	
	Item& item = itemArray[itemIndex - 1]; //get reference to the selected item
    
    reset(); 
    
	if (newPrice <= 0) {
        cout << "\t\t\t\t\t            INVALID PRICE\n";
        cout << "\t\t\t\t\t      PLEASE ENTER A VALID PRICE\n\n";
        changePrice(); //prompt user again if price is invalid
    }
	
	else if (newPrice == item.getPrice()) {
		cout << "\t\t\t\t  !!!PRICE NO CHANGES!!!\n\n";
	}
	
	else {
		item.setPrice(newPrice); //set new price for the item
		cout << "\t\t\t    !!!PRICE CHANGED SUCCESSFULLY!!!\n\n";
	}
	
	Sleep(2500);
    reset();
    adminMenu();
}

void VendingMachine::changeName() {
	int choice; //variable to store user's choice
	
	cout << "1. Machine Title\n";
	cout << "2. Stock Header Name\n";
	cout << "3. Return to Admin Menu\n\n";
	cout << "Enter Choice" << ": ";
	cin >> choice;
	cin.ignore();
	
	string newName; //variable to store new name input
	
	if (choice == 1) {
		cout << "Enter New Name" << ": ";
		getline(cin, newName); //get new machine title from user input
		
		for (char &c : newName) {
			c = toupper(c); //convert all characters to uppercase
		}
		
		machineTitle = newName; //set new machine title
		
		reset(); //clear screen and reset display
		
		cout << "\t\t\t!!!MACHINE TITLE CHANGED SUCCESSFULLY!!!\n\n";
	}
	
	else if (choice == 2) {
		int itemIndex;
    	bool validInput = false;
    
    	printItems(); //display items to user

    	do {
        	cout << "\t\t\t\tEnter Item Index to Change [1-" << numQueue << "]: ";
        	cin >> itemIndex;
        	cin.ignore();
		
        	if (itemIndex <= 0 || itemIndex > numQueue) {
            	cout << "\n\t\t\t\tINVALID INDEX\n";
				cout << "\t\t\t\tPLEASE ENTER A VALID INDEX\n\n";
            	continue; //prompt user again if index is invalid
        	}

        	do {
            	cout << "\t\t\t\tEnter New Name: ";
            	getline(cin, newName);

            	if (newName.length() > 15) {
                	cout << "\n\t\t\t\t!!!ITEM NAME FAILED TO CHANGE!!!\n";
                	cout << "\t\t\t\t!!!MAXIMUM 15 CHARACTERS ONLY!!!\n\n";
            	} else {
                	validInput = true;
            	}
        	} while (!validInput);

        	if (!newName.empty()) {
            	newName[0] = toupper(newName[0]); //capitalize first character
        	}

        	itemArray[itemIndex - 1].setName(newName); //set new name for the item
        	reset();
        	cout << "\t\t\t !!!ITEM NAME CHANGED SUCCESSFULLY!!!\n\n";
 
    	} while (!validInput);
	} 
	
	else if (choice == 3) {
		reset();
		adminMenu();
	}
	
	else {
		cout << "\nINVALID CHOICE\n";
		cout << "PLEASE ENTER A VALID CHOICE\n\n";
		changeName();
	}
	
	Sleep(2500);
    reset();
    adminMenu();
}

void VendingMachine::addFunds() {
	int choice; //variable to store user's choice
	double amount; //variable to store additional funds input
	
	cout << "1. Add Funds\n";
	cout << "2. Return to Admin Menu\n\n";
	cout << setw(22) << "Enter Choice" << ": ";
	cin >> choice;
	
	if (choice == 1) {
		cout << "Enter Additional Funds: RM ";
		cin >> amount; 
		
		reset();
		
		if (amount > 0) {
			totalMoney += amount; //add funds to total money
			
			//display success message with new total money
			cout << "\t\t\t  " << setw(6) << setfill('*') << "*" << " FUNDS ADDED SUCCESSFULLY " << setw(6) << setfill('*') << "*" << setfill(' ') << endl;
			cout << "\t\t\t  " << setw(6) << " " << "NEW TOTAL MONEY: RM " << fixed << setprecision(2) << totalMoney << endl;
			cout << "\t\t\t  " << setw(38) << setfill('*') << "*" << setfill(' ') << endl << endl;
		}
		
		else if (amount == 0) {
			//display message if no changes made
			cout << "\t\t\t\t" << setw(8) << setfill('*') << "*" << " NO CHANGES " << setw(8) << setfill('*') << "*" << setfill(' ') << endl;
			cout << "\t\t\t\t" << setw(3) << " " << "TOTAL MONEY: RM " << fixed << setprecision(2) << totalMoney << endl;
			cout << "\t\t\t\t" << setw(28) << setfill('*') << "*" << setfill(' ') << endl << endl; 
		}
		
		else {
			//display error message for invalid amount
			cout << "INVALID PRICE\n";
			cout << "PLEASE ENTER A VALID PRICE\n\n";
			addFunds(); //prompt user again for valid input
		}
	}
	
	else if (choice == 2) {
		reset();
		adminMenu(); //return to admin menu
	}
	
	else {
		reset();
		cout << "INVALID CHOICE\n";
		cout << "PLEASE ENTER A VALID CHOICE\n\n";
		addFunds(); //prompt user again for valid choice
	}
	
	Sleep(2500);
    reset();
    adminMenu();
}

//use to clear screen 
void VendingMachine::reset() {
	system("cls");
}

//-----------------------------------------------------------------methods to interact with item-----------------------------------------------------------------
string VendingMachine::getItemName(const Item& item) const {
	return item.getName(); 
}

char VendingMachine::getItemChar(const Item& item) const { 
	return item.getChar();
}

int VendingMachine::getItemStock(const Item& item) const {
	return item.getNumStockQ();
}


bool VendingMachine::isEmpty() const {
	return (numQueue==0);
}

bool VendingMachine::isFull() const {
	return (numQueue==maxSize);
}

#endif

