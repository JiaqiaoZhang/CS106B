// @ authors:  Jiaqiao Zhang;  Yipeng Guo
// @ date: November 3,  2019


#ifndef _patientqueue_h
#define _patientqueue_h

#include <iostream>
#include <string>
#include "patient.h"
using namespace std;

class PatientQueue {
public:
    // initialize a new empty queue with 10 capacity.
    // input @ isMinHeap:  determine whether it is a min-heap or max-heap
    PatientQueue(bool isMinHeap = true);

    // free up the memory used by the queue's internal array
    ~PatientQueue();

    // modify the priority of a given existing patient in the patientqueue;
    // input: @ value: patient name
    //        @ newPriority: the new priority of this patient
    void changePriority(string value, int newPriority);

    // remove all elements from the patient queue
    void clear();
    void debug();

    //  remove the frontmost patient in the queue, and return this patient's name
    string dequeue();

    // add the given person into the patient queue with the given priority.
    // and sort it with the desired order
    void enqueue(string value, int priority);

    // return true if the queue does not have any elements
    // return false if it has at least one element
    bool isEmpty() const;

    // return the name of the frontmost patient in the queue without removing it
    string peek() const;

    // return the priority of the frontmost patient in the queue without removing it
    int peekPriority() const;

    // return the number of patiens in the patient queue
    int size() const;

    // print the patient queue to an output stream
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

private:
    // patQueue is the patient queue
    Patient* patQueue;

    // the variable determine the heap type (min or max)
    bool heapType;

    // the number of patients in the queue
    int patCount;

    // the capacity of the patient queue
    int capacity;

    // sort the given element as the min heap structure using bubble up sort
    void bubbleUpMin(int currIdx);

    // sort the given element as the max heap structure using bubble up sort
    void bubbleUpMax(int currIdx);

    // sort the given element as min-heap structure by continuously comparing to the child with low priority number of that element
    void trickleDownMin(int parentIdx);

    // sort the given element as max-heap structure by continuously comparing to the child with high priority number of that element
    void trickleDownMax(int parentIdx);

    // double the capacity of the patient queue when it is full
    void doubleCapacity();

};

#endif
