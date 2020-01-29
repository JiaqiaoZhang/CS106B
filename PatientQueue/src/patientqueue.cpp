// @ authors:  Jiaqiao Zhang;  Yipeng Guo
// @ date: November 3,  2019
// Description:  This program constructs a patientqueue which stores the patient in the order of
// priority (two types: min-heap or max-heap). It has the function enqueue and dequeue similar
// to queue.

#include "patientqueue.h"

// initialize a new empty queue with 10 capacity.
// input @ isMinHeap:  determine whether it is a min-heap or max-heap
PatientQueue::PatientQueue(bool isMinHeap) {

    heapType = isMinHeap;
    capacity = 10;
    patQueue = new Patient[capacity];
    patCount = 0;
    patQueue[0] = Patient();

}

// free up the memory used by the queue's internal array
PatientQueue::~PatientQueue() {
    delete []patQueue;
}

// modify the priority of a given existing patient in the patientqueue;
// input: @ value: patient name
//        @ newPriority: the new priority of this patient
void PatientQueue::changePriority(string value, int newPriority) {

    for(int i = 1; i <= patCount; i++){
        if(patQueue[i].name == value){
            patQueue[i].priority = newPriority;
            if(heapType){
                bubbleUpMin(i);      // min-heap
                trickleDownMin(i);
                return;
            }else{
                bubbleUpMax(i);
                trickleDownMax(i);   // max-heap
                return;
            }
        }
    }
    throw "patient not found!";
}

// remove all elements from the patient queue
void PatientQueue::clear() {
    for(int i = 1; i <= patCount; i++){
        patQueue[i] = Patient();
    }
    patCount = 0;
}

void PatientQueue::debug() {
    // empty
}

//  remove the frontmost patient in the queue, and return this patient's name
string PatientQueue::dequeue() {

    if (patCount == 0){
        throw "PatientQueue is empty!";
    }
    Patient frontPat = patQueue[1];
    int currIdx = patCount;
    if(currIdx == 1){
        patQueue[currIdx] = Patient();
        patCount --;
        return frontPat.name;
    }else{
        patQueue[1] = patQueue[currIdx];
        patQueue[currIdx] = Patient();
        patCount --;
        if(heapType){
            trickleDownMin(1);  // minHeap type
        }else{
            trickleDownMax(1);  // maxHeap type
        }
    }
    return frontPat.name;
}

// add the given person into the patient queue with the given priority.
// and sort it with the desired order
void PatientQueue::enqueue(string value, int priority) {
    Patient currPat = Patient(value, priority);
    patCount ++;
    if(patCount >= capacity){
        doubleCapacity();
    }
    patQueue[patCount] = currPat;
    if(heapType){
        bubbleUpMin(patCount);       // minHeap type
    }else{
        bubbleUpMax(patCount);       // maxHeap type;
    }
}

// return true if the queue does not have any elements
// return false if it has at least one element
bool PatientQueue::isEmpty() const {
    // TODO: write this function
    if(patCount == 0){
        return true;
    }
    return false;
}

// return the name of the frontmost patient in the queue without removing it
string PatientQueue::peek() const {
    if (patCount == 0){
        throw "PatientQueue is empty!";
    }
    return patQueue[1].name;   // remove this
}

// return the priority of the frontmost patient in the queue without removing it
int PatientQueue::peekPriority() const {
    if (patCount == 0){
        throw "PatientQueue is empty!";
    }
    return patQueue[1].priority;   // remove this
}

// return the number of patiens in the patient queue
int PatientQueue::size() const {
    return patCount;   // remove this
}

// print the patient queue to an output stream
ostream& operator <<(ostream& out, const PatientQueue& queue) {

    out << "{";
    for(int i = 1; i <= queue.size(); i++){
        out << queue.patQueue[i];
        if(i < queue.size()){
            out << ", ";
        }
    }
    out << "}";
    return out;
}

// sort the given element as the min heap structure by continuously comparing to the parent of that element
void PatientQueue::bubbleUpMin(int currIdx){

    while( currIdx > 1){
        int parrentIdx = currIdx/2;
        if(patQueue[currIdx] < patQueue[parrentIdx]){
            Patient temp = patQueue[parrentIdx];
            patQueue[parrentIdx] = patQueue[currIdx];
            patQueue[currIdx] = temp;
            currIdx = parrentIdx;
        }else{
            break;
        }
    }
}

// sort the given element as the max heap structure using bubble up sort
void PatientQueue::bubbleUpMax(int currIdx){

    while( currIdx > 1){
        int parentIdx = currIdx/2;
        if(patQueue[currIdx] > patQueue[parentIdx]){
            Patient temp = patQueue[parentIdx];
            patQueue[parentIdx] = patQueue[currIdx];
            patQueue[currIdx] = temp;
            currIdx = parentIdx;
        }else{
            break;
        }
    }
}

// sort the given element as min-heap structure by continuously comparing to the child with low priority number of that element
void PatientQueue::trickleDownMin(int parentIdx){

    int childIdx;
    if(2 * parentIdx > patCount){
        return;
    }
    if(2 * parentIdx + 1 <= patCount){
        if(patQueue[2 * parentIdx] < patQueue[2 * parentIdx + 1]){
            childIdx = 2 * parentIdx;
        }else{
            childIdx = 2 * parentIdx + 1;
        }
    }else{
        childIdx = 2 * parentIdx;
    }

    if(patQueue[childIdx] > patQueue[parentIdx]){
        return;
    }
    else{
        Patient temp = patQueue[parentIdx];
        patQueue[parentIdx] = patQueue[childIdx];
        patQueue[childIdx] = temp;
        trickleDownMin(childIdx);
    }
}

// sort the given element as max-heap structure by continuously comparing to the child with high priority number of that element
void PatientQueue::trickleDownMax(int parentIdx){

    int childIdx;
    if(2 * parentIdx > patCount){
        return;
    }
    if(2 * parentIdx + 1 <= patCount){
        if(patQueue[2 * parentIdx] > patQueue[2 * parentIdx + 1]){
            childIdx = 2 * parentIdx;
        }else{
            childIdx = 2 * parentIdx + 1;
        }
    }else{
        childIdx = 2 * parentIdx;
    }

    if(patQueue[childIdx] < patQueue[parentIdx]){
        return;
    }
    else{
        Patient temp = patQueue[parentIdx];
        patQueue[parentIdx] = patQueue[childIdx];
        patQueue[childIdx] = temp;
        trickleDownMax(childIdx);
    }
}

// double the capacity of the patient queue when it is full
void PatientQueue::doubleCapacity(){
   Patient* temp = patQueue;
   patQueue = new Patient[capacity * 2];

   for(int i = 0; i <= capacity; i++){
       patQueue[i] = temp[i];
   }
   capacity = capacity * 2;
   delete [] temp;

}
