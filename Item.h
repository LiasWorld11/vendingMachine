#ifndef _ITEM_
#define _ITEM_

#include <string>
#include <cctype>
#include <iomanip>
#include <stdexcept>
#include "Queue.h"

using namespace std;

class Item {
	private:
		string itemName;
		double itemPrice;
		char itemChar;
		Queue<Item> *itemQueue;
		int maxSize;
		
	public:
		Item();
		Item(string name, double price, int size); 		  //overloaded constructor
		
		void setName(string n);   						  //set name for the item (admin function)
		void setPrice(double p);  						  //set price for the item (admin function)
		
		string getName() const;   						  //get name for the item
		double getPrice() const;  						  //get price for the item
		char getChar() const;     						  //get char to represent item in the item interface
		int getMaxSize() const;                           //get maximum size of queue
		
		//methods to interact with the queue (use dynamic pointer)
		int addStockToQ(int stock);                       //add item to queue
        void removeStockFromQ(Item item);                 //remove item from queue
        bool isItemQFull() const;                         //check if item queue is full
        bool isItemQEmpty() const;                        //check if item queue is empty
        void clearItemQ();                                //clear the item queue
        int getNumStockQ() const;                         //get the number of stock in queue
};

Item::Item(){
	itemName = "";
	itemPrice = 0.00;
	itemChar = ' ';
	itemQueue = nullptr;
	maxSize = 20;
}

Item::Item(string name, double price, int stock){
	maxSize = 20;
	itemName = name;
	itemPrice = price;
	itemChar = toupper(itemName[0]);
	itemQueue = new Queue<Item>(maxSize);
	
	if (stock > maxSize){
		throw invalid_argument ("Invalid size! Stock cannot exceed maximum."); //cannot run if size is over maximum 
	}
	
	for (int i=0; i<stock; i++){
        itemQueue->enqueue(*this);
    }	
}

void Item::setName(string n){
	itemName = n;
	itemChar = toupper(itemName[0]);
}

void Item::setPrice(double p){
	itemPrice = p;
}

string Item::getName() const{
	return itemName;
}

double Item::getPrice() const{
	return itemPrice;
}

char Item::getChar() const{
	return itemChar;
}

int Item::getMaxSize() const{
	return maxSize;
}

//------------------------------methods to interact with the queue------------------------------
int Item::addStockToQ(int stock){
    int addedCount = 0;  //counter for successfully added items

    for (int i=0; i<stock; ++i){
        if (!isItemQFull()) {
            itemQueue->enqueue(*this);
            addedCount++;
        }
        else{
        	break;
		}
    }
    
    if (addedCount < stock){
    	cout << setw(10) << "\n\n\t\t\t" << "   [ QUEUE IS FULL ]" << setw(10) << endl;
	    cout << setw(5) << "\t\t\t\t" << "ADDED " << addedCount << " ITEMS OUT OF " << stock << endl;
	}
	return addedCount;
}

void Item::removeStockFromQ(Item item){
	
	if (!isItemQEmpty()){
		itemQueue->dequeue(item);	
	}
}

bool Item::isItemQFull() const{
    return itemQueue->isFull();
}

bool Item::isItemQEmpty() const{
    return itemQueue->isEmpty();
}

void Item::clearItemQ(){
    itemQueue->clear();
}

int Item::getNumStockQ() const{
	return itemQueue->getNumItem();
}

#endif
