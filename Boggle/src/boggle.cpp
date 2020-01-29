// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "boggle.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "set.h"
#include "shuffle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "gui.h"
#include "queue.h"
#include "map.h"
using namespace std;

// function prototype declarations
void intro();
int getScore(string& word);
void checkHumanWord(Grid<char>& board, string word, Set<string>& wordList, int& scoreNumber, Lexicon& dictionary);
bool humanWordSearch(Grid<char>& board, string word);
Grid<char> randomBoard();
bool checkValid(string inputChar);
Grid<char> askForGrid();
void getDirection(Vector<Vector<int>>& direction);
bool helperHumanWordSearch(Grid<char> board, string word, Vector<Vector<int>>& direction, int row, int col, int count);
void inputBoard(Grid<char>& board);
bool yesOrNo ();
bool helperComputerWordSearch(Grid<char> board, string& word, Vector<Vector<int>>& direction, int row, int col, Lexicon& dictionary, Set<string>& words, Set<string>& humanWords);

int main() {
    Vector<int> v;
    Grid<int> g(3, 4, 1);
    Stack<string> s;
    Queue<string> q;
    q.enqueue("first");
    q.enqueue("second");
    g.resize(4, 3);

    v.add(1);
    v +=2;
    v.remove(0);
    cout << "type a name" << endl;
    string name;
    cin >> name;

    cout << "name is " << name << endl;

    gui::initialize(BOARD_SIZE, BOARD_SIZE);
    intro();
    bool play = true;
    while(play){
        gui::initialize(BOARD_SIZE, BOARD_SIZE);
        cout << endl;
        Grid<char> board;
        //cout <<"pla"<< endl;
        board = askForGrid();
        for (int i = 0; i < 4; i++ ){
            cout << board[i][0] << board[i][1] << board[i][2] << board[i][3] << endl;
        }

        Lexicon dictionary = Lexicon(DICTIONARY_FILE); // store the dic into a lexicon
        cout << endl;
        Set<string> wordlistHuman = {};
        Set<string> wordListComputer = {};
        int scoreNumberHuman = 0;
        int scoreNumberComputer = 0;

        cout<< "It's your turn!" << endl;
        cout << "Your words: " << wordlistHuman << endl;
        cout << "Your score: "<< scoreNumberHuman << endl;

        string word = getLine("Type a word (or Enter to stop): ");
        while(!word.empty()){
            checkHumanWord(board, word, wordlistHuman, scoreNumberHuman, dictionary);
            word = getLine("Type a word (or Enter to stop): ");
            gui::clearHighlighting();
        }

        cout << "\nIt's my turn!" << endl;
        wordListComputer = computerWordSearch(board, dictionary,wordlistHuman);
        cout << "My words: " << wordListComputer << endl;

        while(!wordListComputer.isEmpty()){
            string word = wordListComputer.first();
            scoreNumberComputer += getScore(word);
            wordListComputer.remove(word);
        }
        cout << "My score: "<< scoreNumberComputer << endl;
        gui::setScore("computer", scoreNumberComputer);
        if(scoreNumberComputer > scoreNumberHuman){
            cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!\n" << endl;
        }else{
            cout << "WOW, you defeated me! Congratulations!\n" << endl;
        }

        play = yesOrNo();
    }
    cout << "Have a nice day." << endl;









    return 0;
}

// Prints a welcome message that introduces the program to the user.
void intro() {
    cout << "Welcome to CS 106B Boggle!" << endl;
    cout << "This game is a search for words on a 2-D board of letter cubes." << endl;
    cout << "The good news is that you might improve your vocabulary a bit." << endl;
    cout << "The bad news is that you're probably going to lose miserably to" << endl;
    cout << "this little dictionary-toting hunk of silicon." << endl;
    cout << "If only YOU had 16 gigs of RAM!" << endl;
    cout << endl;
    getLine("Press Enter to begin the game ...");
}

void checkHumanWord(Grid<char>& board, string word, Set<string>& wordList, int& scoreNumber, Lexicon& dictionary){
    if(word.length() < 4){
        cout << "The word must have at least 4 letters." << endl;
    }else if(!dictionary.contains(word)){
        cout << "That word is not found in the dictionary." << endl;
    }else if(wordList.contains(word)){
        cout << "You have already found that word." << endl;
    }else if(humanWordSearch(board, toUpperCase(word))){
        word = toUpperCase(word);
        cout << "You found a new word! "<< "\"" << word << "\"" << "\n" << endl;
        wordList.add(word);
        scoreNumber += getScore(word);
        gui::recordWord("human", word);
        gui::setScore("human", scoreNumber);
        cout << "Your words: " << wordList << endl;
        cout << "Your score: "<< scoreNumber << endl;
    }else{
        cout << "That word can't be formed on this board." << endl;
        cout << "Your words: " << wordList << endl;
        cout << "Your score: "<< scoreNumber << endl;
    }
}

bool humanWordSearch(Grid<char>& board, string word) {
    // start with finding the first letter of the word in the Grid
    Vector<Vector<int>> direction;
    getDirection(direction);
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(helperHumanWordSearch(board, word, direction, i, j, 0)){
                return true;
            }
        }
    }
    return false;
}

bool helperHumanWordSearch(Grid<char> board, string word, Vector<Vector<int>>& direction, int row, int col, int count){

    if(count == word.length()){
        return true;
    }else{
        if(col == board.width()|| row == board.height() || col == -1 || row == -1){
            return false;
        }
        if(board[row][col] == word[count]){
            char findLetter = board[row][col];
            //gui::clearHighlighting();
            board[row][col] = '1'; // choose
            gui::setHighlighted(row, col, true);
            // explore 8 directions
            for(int i = 0; i < direction.size(); i++){
                Vector<int> dir = direction[i];
                if(helperHumanWordSearch(board, word, direction, row+dir[0], col+dir[1], count+1)){
                    return true;
                }
            }
            board[row][col] = findLetter;  // unchoose
            gui::setHighlighted(row, col, false);
        }
     }
     return false;
}

Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    // TODO: write this function

    Set<string> words;
    Vector<Vector<int>> direction;
    getDirection(direction);
    for(int i = 0; i < BOARD_SIZE; i++){
        int j = 0;
        while (j<BOARD_SIZE){
        //for(int j = 0; j < BOARD_SIZE; j++){
            string word;
            if(helperComputerWordSearch(board, word, direction, i, j, dictionary, words, humanWords)){
                //if (!humanWords.contains(word)){
                    words.add(word);
                    gui::recordWord("computer", word);
                    j--;
                //}

            }
            j++;
        }
    }
    return words;
}
//&& !words.contains(word)
bool helperComputerWordSearch(Grid<char> board, string& word, Vector<Vector<int>>& direction, int row, int col, Lexicon& dictionary, Set<string>& words, Set<string>& humanWords){
    if(!dictionary.containsPrefix(word)){
        return false;
    }else{
        if(col == board.width()|| row == board.height() || col == -1 || row == -1){
            return false;
        }else if(dictionary.contains(word) && word.length() >= 4 && !words.contains(word) &&!humanWords.contains(word)){
            return true;
        }else{
            char findLetter = board[row][col];
            board[row][col] = '1'; // choose
            word += charToString(findLetter);
            // explore 8 directions
            for(int i = 0; i < direction.size(); i++){
                Vector<int> dir = direction[i];
                if(helperComputerWordSearch(board, word, direction, row+dir[0], col+dir[1], dictionary, words, humanWords)){
                    return true;
                }
            }
            board[row][col] = findLetter;  // unchoose
            word = word.substr(0, word.length()-1);
     }

     return false;
    }
}

int getScore(string& word){
    int len = word.length();
    if (len == 4) {return 1;}
    else if (len == 5) {return 2;}
    else if (len == 6) {return 3;}
    else if (len == 7) {return 5;}
    else {return 11;}

}


void getDirection(Vector<Vector<int>>& direction){
    direction.add({-1, 0});    //up
    direction.add({-1, -1});   //upleft
    direction.add({0, -1});    //left
    direction.add({1, -1});    //leftdown
    direction.add({1, 0});     //down
    direction.add({1, 1});     //downright
    direction.add({0, 1});     //right
    direction.add({-1, 1});    //rightup
}

Grid<char> randomBoard(){
    Vector<string> cubes = LETTER_CUBES;
    Grid<char> board(BOARD_SIZE,BOARD_SIZE);
    for (int i = 0 ; i < BOARD_SIZE ; i++ ){
        for (int j = 0 ; j < BOARD_SIZE ; j ++){
            shuffle(cubes);
            string diceStr = cubes[0];
            diceStr = shuffle(diceStr);
            board[i][j] = diceStr[0];
            cubes.remove(0);
        }
    }
    return board;
}

Grid<char> inputBoard(){
    string inputChar = getLine("Type the 16 letters on the board: ");
    while(!checkValid(inputChar)){
        cout<< "Invalid board string. Try again." << endl;
        inputChar = getLine("Type the 16 letters on the board: ");
    }
    Grid<char> board(BOARD_SIZE,BOARD_SIZE);
    inputChar = toUpperCase(inputChar);
    string::size_type idx = 0;

    for (int i = 0 ; i < BOARD_SIZE ; i++ ){
        for (int j = 0 ; j < BOARD_SIZE ; j ++){
            board[i][j] = inputChar[idx];
            idx++;
        }
    }
    gui::labelCubes(board);
    return board;
}



bool checkValid(string inputChar){
    if(inputChar.length()!=BOARD_SIZE*BOARD_SIZE){
        return false;
    }
    for (string::size_type i = 0; i < inputChar.length(); i++){
        if (inputChar[i] < 'A' || (inputChar[i] > 'Z' && inputChar[i] < 'a') || inputChar[i] > 'z'){
            return false;
        }
    }
    return true;
}

Grid<char> askForGrid(){
    string choice = getLine("Generate a random board? ");
    Grid<char> board;
    if(choice == "Y"){
        board = randomBoard();
        gui::labelCubes(board);
        return board;
    }else if(choice == "N"){
        board = inputBoard();
    }else{
        cout << "Please type a word that starts with 'Y' or 'N'." << endl;
        return askForGrid();
    }
    return board;
}

bool yesOrNo (){
    string choice = getLine("Play again? ");
    if(choice == "Y"){
        return true;
    }else if(choice == "N"){
        return false;

    }else{
        cout << "Please type a word that starts with 'Y' or 'N'." << endl;
        return yesOrNo();
    }
}






