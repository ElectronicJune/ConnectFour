#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <map>
using namespace std;
map<string, int> x_scores;
map<string, int> o_scores;
//player is O 
//computer is X
//code is based on computer
//hueristic score
int hueristic_score(string board,char player){
  char opponent = player=='X' ? 'O':'X';
  int score = 0;
  for (int i=0;i<42;i++){
    if (board[i]==opponent) continue;
    if (i<24) {
      score += pow(5,((int)board[i]==player)+((int)board[i+6]==player)+((int)board[i+12]==player)+((int)board[i+18]==player));

      if (i%6 > 2){
        score += pow(5,((int)board[i]==player)+((int)board[i+5]==player)+((int)board[i+10]==player)+((int)board[i+15]==player));
      }else{
        score += pow(5,((int)board[i]==player)+((int)board[i+7]==player)+((int)board[i+14]==player)+((int)board[i+21]==player));
      }
    }
    if (i%6 <3){
      score += pow(5,((int)board[i]==player)+((int)board[i+1]==player)+((int)board[i+2]==player)+((int)board[i+3]==player));
    }
  }
  return score/count(board.begin(),board.end(),player);
}

//put pieces (start from 1)
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

//returns the board result if ended, else return 2
int result(string board){
  for (int i=0;i<42;i++){
    if (board[i]=='.') continue;
    if (i<24) {
      if (board[i]==board[i+6] && board[i]==board[i+12] && board[i]==board[i+18]){
        return (board[i]=='X') ? 1: -1;
      }
      if (i%6 > 2){
        if (board[i]==board[i+5] && board[i]==board[i+10] && board[i]==board[i+15]){
          return (board[i]=='X') ? 1: -1;
        }
      }else{
        if (board[i]==board[i+7] && board[i]==board[i+14] && board[i]==board[i+21]){
          return (board[i]=='X') ? 1: -1;
        }
      }
    }
    if (i%6 <3){
      if (board[i]==board[i+1] && board[i]==board[i+2] && board[i]==board[i+3]){
        return (board[i]=='X') ? 1: -1;
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
  if (player=='X' && x_scores.count(board)==1){
    return x_scores[board];
  }else if (o_scores.count(board)==1){
    return o_scores[board];
  }
  if (depth==0){
    return hueristic_score(board,player);
  }
  //if the board has ended, return the result
  if (result(board)!=2) return result(board)*(numeric_limits<int>::max()-1);
  for (int i=0; i<7; i++){
    if (player=='X' && result(put(i+1,board,player))==1) {
      return numeric_limits<int>::max();
    }else if (result(put(i+1,board,player))==-1){
      return numeric_limits<int>::min();
    }
  }
  //if not, find the max/min result of the next move combination
  int score = player =='X' ? numeric_limits<int>::min() : numeric_limits<int>::max();
  for (int i=0;i<7; i++){
    if (board[6*i]!='.') continue;
    // if maximizing player
    if (player == 'X'){
      int next_result =  minimax(put(i+1,board,'X'),'O',depth-1);
      if (next_result>score) score = next_result;
    }else{
      int next_result =  minimax(put(i+1,board,'O'),'X',depth-1);
      if (next_result<score) score = next_result;
    }
  }
  if (player=='X'){
    x_scores[board] = score;
  }else{
    o_scores[board] = score;
  }
  return score;
}

//return next best move with minimax
string nextBestMove(string board){
  int highest_score = numeric_limits<int>::min();
  string best_move;
  //expected is X
  for (int i=0;i<7;i++){
    if (board[i*6]!='.') continue;
    string move = put(i+1,board,'X');
    int score = minimax(move,'O',7);
    if (score>=highest_score){
      best_move = move;
      highest_score = score;
    }
  }
  cout << "\nWIN RATE: " << highest_score << endl;
  x_scores.clear();
  o_scores.clear();
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