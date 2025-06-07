#ifndef _QUEUE_
#define _QUEUE_

#include <iostream>
#include <string>

using namespace std;

template <class T>
class Queue {
	private:
		T* queueArray;
		int queueSize;
		int numItem;
		int front;
		int rear;
		
	public:
		Queue(int size); 
		~Queue();
		void enqueue(T newItem); 			//add new item(s) to the queue (admin function)
		void dequeue(T &item);             	//remove one instance of an item from the queue
		bool isFull() const;                //check if queue is full
		bool isEmpty() const;               //check if queue is empty
		void clear();                       //reset the queue to 0 if there is no stock left (admin function)
		int getNumItem() const;
};

template <class T>
Queue<T>::Queue(int size) {
	queueArray = new T[size]; //create the dynamic array
	queueSize = size;
	numItem = 0; 			  //no items
	front = 0;
	rear = queueSize - 1;	
}

template <class T>
Queue<T>::~Queue() {
	delete[] queueArray;
}

template <class T>
void Queue<T>::enqueue(T newItem) {
	
    if (isFull()) {
        cout << "The queue is full!" << endl;
        return;
    }
	else {
        //move the rear to the next location
        rear = (rear + 1) % queueSize;

        //add the new data to the array
        queueArray[rear] = newItem;

        //increase numItem by 1
        numItem++;
    }
}

template <class T>
void Queue<T>::dequeue(T &item) {
	
	if (isEmpty()) {  
		cout << "The queue is empty!" << endl;
	}
	else {
		//take out the front item and store it to item
		item = queueArray[front];
		
		//move the front to the next position
		front = (front + 1) % queueSize;
		
		//reduce numItem by 1
		numItem--;
	}	
}

template <class T>
bool Queue<T>::isFull() const {
	return (numItem == queueSize);
}

template <class T>
bool Queue<T>::isEmpty() const {
	return (numItem == 0);
}

template <class T>
void Queue<T>::clear() {
	front = 0;
	rear = queueSize - 1;
	numItem = 0;
}

template <class T>
int Queue<T>::getNumItem() const {
	return numItem;
}

#endif
