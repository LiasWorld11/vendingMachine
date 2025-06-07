#include <iostream>
#include "Item.h"
#include "Vending_Machine.h"

int main() {
    //create a vending machine with capacity for 5 items
	VendingMachine vm(5); 

    //create some items to add to the vending machine
    Item item1("Cola", 1.50, 19);
    Item item2("Sprite", 1.50, 0);
    Item item3("Milo", 1.00, 3);
    Item item4("Chocolate", 2.00, 20);
    Item item5("Tea", 2.50, 12);

    //add items to the vending machine
    vm.addItem(item1);
    vm.addItem(item2);
    vm.addItem(item3);
    vm.addItem(item4);
    vm.addItem(item5);

    //print the vending machine interface
    vm.mainMenu();
     
    return 0;
}

