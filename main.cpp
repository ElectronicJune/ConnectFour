#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
using namespace std;
//player is O 
//computer is X
//code is based on computer
//create map for speed
map<string, int> o_moves_result;
map<string, int> x_moves_result;
//hueristic score
int hueristic_score(string board,char player){
  char opponent = player=='X' ? 'O':'X';
  int score = 0;
  //check for horizontal match
  for (int i=0; i<24; i++){
    if (board[i]==opponent) continue;
    score += pow(5,((int)board[i]==player)+((int)board[i+6]==player)+((int)board[i+12]==player)+((int)board[i+18]==player));
  }
  //0 6 12 18 24 30 36 | 42
  //check for vertical match
  for (int i=0; i<42; i+=6){
    for (int j=0;j<3;j++){
      if (board[i+j]==opponent) continue;
      score += pow(5,((int)board[i+j]==player)+((int)board[i+j+1]==player)+((int)board[i+j+2]==player)+((int)board[i+j+3]==player));
    }
  }
  //check for diagonal match (/)
  for (int i=3; i<27; i+=6){
    for (int j=0; j<3; j++){
      if (board[i+j]==opponent) continue;
      score += pow(5,((int)board[i+j]==player)+((int)board[i+j+5]==player)+((int)board[i+j+10]==player)+((int)board[i+j+15]==player));
    }
  }
  //check for diagonal match (\)
  for (int i=21; i<45; i+=6){
    for (int j=0; j<3; j++){
      if (board[i+j]==opponent) continue;
      score += pow(5,((int)board[i+j]==player)+((int)board[i+j-7]==player)+((int)board[i+j-14]==player)+((int)board[i+j-21]==player));
    }
  }
  return score;
}

//put pieces
string put(int place, string board, char player){
  for (int i=6*place-1; i>=6*place-6; i--){
    if (board[i]=='.') {
      board[i] = player;
      return board;
    }
  }
  return board;
}

//display board
void display(string board){
  cout << "+---------------+\n";
  for (int i=0; i<6; i++){
    cout << "| ";
    for (int j=i; j<42; j+=6){
      cout <<board[j] << ' ';
    }
    cout << "|\n";
  }
  cout << "| 1 2 3 4 5 6 7 |\n+---------------+\n";
}

//return a vector of boards after the next move
vector<string> moveCombinations(string board, char player){
  vector<string> combinations;
  for (int i=0; i<7; i++){
    if (board[i*6]=='.') {
      combinations.push_back(put(i+1,board,player));
    }
  }
  return combinations;
}

//returns the board result if ended, else return 2
int result(string board){
  //result is based on computer (1 for win)
  //check for horizontal match
  for (int i=0; i<24; i++){
    if (board[i]=='.') continue;
    if (board[i]==board[i+6] && board[i]==board[i+12] && board[i]==board[i+18]){
      return (board[i]=='X') ? 1: -1;
    }
  }
  //0 6 12 18 24 30 36 42
  //check for vertical match
  for (int i=0; i<42; i+=6){
    for (int j=0;j<3;j++){
      if (board[i+j]=='.') continue;
      if (board[i+j]==board[i+j+1] && board[i+j]==board[i+j+2] && board[i+j]==board[i+j+3]){
        return (board[i+j]=='X') ? 1: -1;
      }
    }
  }
  //check for diagonal match (/)
  for (int i=3; i<27; i+=6){
    for (int j=0; j<3; j++){
      if (board[i+j]=='.') continue;
      if (board[i+j]==board[i+j+5] && board[i+j]==board[i+j+10] && board[i+j]==board[i+j+15]){
        return (board[i+j]=='X') ? 1: -1;
      }
    }
  }
  //check for diagonal match (\)
  for (int i=21; i<45; i+=6){
    for (int j=0; j<3; j++){
      if (board[i+j]=='.') continue;
      if (board[i+j]==board[i+j-7] && board[i+j]==board[i+j-14] && board[i+j]==board[i+j-21]){
        return (board[i+j]=='X') ? 1: -1;
      }
    }
  }
  //check if game not end
  for (char i : board){
    if (i=='.') return 2;
  }
  //draw
  return 0;
}

//minimax function, used to find the best move
//X is the maximizing player
int minimax(string board, char player,int depth){
  if (player=='X' && x_moves_result.count(board)==1){
    return x_moves_result[board];
  }else if (o_moves_result.count(board)==1){
    return o_moves_result[board];
  }
  if (depth==0){
    return hueristic_score(board,player);
  }
  //if the board has ended, return the result
  if (result(board)!=2) return result(board)*(numeric_limits<int>::max()-1);
  //if not, find the max/min result of the next move combination
  int score = player =='X' ? numeric_limits<int>::min():numeric_limits<int>::max();
  for (string moves : moveCombinations(board,player)){
    // if maximizing player
    if (player == 'X'){
      int next_result =  minimax(moves,'O',depth-1);
      if (next_result>score) score = next_result;
    }else{
      int next_result =  minimax(moves,'X',depth-1);
      if (next_result<score) score = next_result;
    }
  }
  if (player=='X'){
    x_moves_result[board] = score;
  }else{
    o_moves_result[board] = score;
  }
  return score;
}

//return next best move with minimax
string nextBestMove(string board){
  int highest_score = numeric_limits<int>::min();
  string best_move;
  //expected is X
  for (string move : moveCombinations(board,'X')){
    int score = minimax(move,'O',8);
    if (score>highest_score){
      best_move = move;
      highest_score = score;
    }
  }
  cout << "\nWIN RATE: " << highest_score << endl;
  return best_move;
}

int main(){
  char first_player = 'O';
  while (true){
    char player = first_player;
    //empty board of 6x7 places
    string board = "..........................................";
    while (result(board)==2){
      if (player=='X'){
        board = nextBestMove(board);
        player = 'O';
        continue;
      }
      display(board);
      int player_move;
      do{
        cin >> player_move;
      }while (player_move<1 || player_move>7 || board[6*(player_move-1)]!='.');
      //change the board whith put() function
      board = put(player_move,board,player);
      player = 'X';
    }
    display(board);
    if (result(board)==0){
      cout << "DRAW";
    }else if (result(board)==1){
      cout << "YOU LOSE";
    }else {
      cout << "YOU WON";
    }
    cout << endl;
    first_player = first_player=='X' ? 'O' : 'X';
  }
  return 0;
}