// TODO: remove and replace this file header comment
// This is the CPP file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.

#include "encoding.h"
#include "priorityqueue.h"
#include "filelib.h"

using namespace std;

void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<char, string>& encodingMap,string& value);
void flattenTreeToHeaderHelper(HuffmanNode* t, string &flattenTree);
HuffmanNode* recreateTreeFromHeaderHelper(string &str);

Map<char, int> buildFrequencyTable(istream& input)
{
    Map<char, int> freqTable;
    char ch;
    while(input.get(ch)){
        freqTable[ch]++;
    }
    return freqTable;
}

HuffmanNode* buildEncodingTree(Map<char, int>& freqTable)
{

    PriorityQueue<HuffmanNode*> encodingTree;
    for(char ch:freqTable){
        HuffmanNode* node = new HuffmanNode(ch);
        encodingTree.enqueue(node, freqTable[ch]);
    }

    while(encodingTree.size() != 1){
        double priChild1 = encodingTree.peekPriority();
        HuffmanNode* child1 = encodingTree.dequeue();
        double priChild2 = encodingTree.peekPriority();
        HuffmanNode* child2 = encodingTree.dequeue();
        HuffmanNode* newNode = new HuffmanNode(child1, child2);
        encodingTree.enqueue(newNode, priChild1+priChild2);
    }
    return encodingTree.dequeue();
}

string flattenTreeToHeader(HuffmanNode* t)
{
    string flattenTree;
    if(t->isLeaf()){
        flattenTree += '.';
        flattenTree += t->ch;
    }
    flattenTreeToHeaderHelper(t, flattenTree);
    return flattenTree;
}
void flattenTreeToHeaderHelper(HuffmanNode* t, string &flattenTree){
    if(!t->isLeaf()){
        flattenTree += "(";
        flattenTreeToHeaderHelper(t->zero, flattenTree);
        flattenTreeToHeaderHelper(t->one, flattenTree);
        flattenTree += ")";

    }
    else{
        flattenTree += ".";
        flattenTree += t->ch;
    }
}
HuffmanNode* recreateTreeFromHeader(string str){
    HuffmanNode* tree = new HuffmanNode('\0');
//    char ch = str[0];
//    if(ch == '.'){
//        tree->ch = str[1];
//    }else{
//        str = str.substr(1);
    tree = recreateTreeFromHeaderHelper(str);
    return tree;
}
//void recreateTreeFromHeaderHelper(HuffmanNode* t, string &str){
//    char ch = str[0];
//    if(ch == '('){
//        HuffmanNode* node = new HuffmanNode('\0');
//        if(t->zero == nullptr){
//            t->zero = node;
//        }else{
//            t->one = node;
//        }
//        str = str.substr(1);
//        recreateTreeFromHeaderHelper(node, str);
//        recreateTreeFromHeaderHelper(t, str);
//    }
//    if(ch == '.'){
//        HuffmanNode* node = new HuffmanNode(str[1]);
//        if(t->zero == nullptr){
//            t->zero = node;
//        }else{
//            t->one = node;
//        }
//        str = str.substr(2);
//        recreateTreeFromHeaderHelper(t, str);
//    }
//    if(ch == ')'){
//        str = str.substr(1);
//        return;
//    }
//}
HuffmanNode* recreateTreeFromHeaderHelper(string &str){
    char ch = str[0];
    if(str.length() == 0){
        return nullptr;
    }
    if(ch == '.'){
        HuffmanNode* node = new HuffmanNode(str[1]);
        str = str.substr(2);
        return node;
    }
    HuffmanNode* node = new HuffmanNode('\0');
    if(ch == '('){
        str = str.substr(1);
        node->zero = recreateTreeFromHeaderHelper(str);
        node->one = recreateTreeFromHeaderHelper(str);
    }
    str = str.substr(1);
    return node;
}

Map<char, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    // TODO: implement this function
    Map<char, string> encodingMap;
    string value = "";
    buildEncodingMapHelper(encodingTree, encodingMap, value);
    return encodingMap;
}
void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<char, string>& encodingMap, string& value){
    if(encodingTree != nullptr){
        buildEncodingMapHelper(encodingTree->zero, encodingMap, value += "0");
        value = value.substr(0, value.length()-1);
        buildEncodingMapHelper(encodingTree->one, encodingMap, value +="1");
        value = value.substr(0, value.length()-1);
        if(encodingTree->isLeaf()){
            encodingMap[encodingTree->ch] = value;
        }
    }
}


void freeTree(HuffmanNode* t)
{
    if(t != nullptr){
       freeTree(t->zero);
       freeTree(t->one);
       delete t;
    }
}

void compress(istream& input, HuffmanOutputFile& output)
{
    Map<char, int>freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<char, string>encodingMap = buildEncodingMap(encodingTree);
    string flattenTree = flattenTreeToHeader(encodingTree);

    rewindStream(input);
    output.writeHeader(flattenTree);
    char ch;
    while(input.get(ch)){
        string bitSeq = encodingMap[ch];
        for(int i = 0; i <= bitSeq.length()-1; i++){
            char bit = bitSeq[i];
            output.writeBit(bit-'0');
        }
    }
    freeTree(encodingTree);
}


void decompress(HuffmanInputFile& input, ostream& output)
{
    string flattenTree = input.readHeader();
    HuffmanNode* encodingTree = recreateTreeFromHeader(flattenTree);
    HuffmanNode* currentNode = encodingTree;
    int bit = 0;
    while(bit != -1){
       if(currentNode->isLeaf()){
            output << currentNode->ch;
            currentNode = encodingTree;
       }else{
           bit = input.readBit();
           if(bit == 0){
               currentNode = currentNode->zero;
           }
           else if(bit == 1){
               currentNode = currentNode->one;
           }
       }
    }
    freeTree(encodingTree);
}
