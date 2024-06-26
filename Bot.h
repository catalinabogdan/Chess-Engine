#ifndef BOT_H
#define BOT_H
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <optional>
#include <cstring>

#include "Move.h"
#include "PlaySide.h"

struct ChessPiece {
    Piece piesa;
    PlaySide culoare;
    bool isPromotion;
    int moves_count;
};


class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
    ChessPiece board[8][8];
    int isCapturedWhite[6]; // vector de frecventa
    int isCapturedBlack[6];
    std::string last_src;
    std::string last_dest;
  /* Declare custom fields above */
  Bot();

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move* move, PlaySide sideToMove);
  void recordMove2(ChessPiece board[8][8], Move* move, PlaySide sideToMove);

  /**
   * Calculates and return the bot's next move
   * @return your move
   */
  Move* calculateNextMove();

  static std::string getBotName();
  void generateChessBoard(ChessPiece board[8][8]);
  std::queue<Move*> generateLegalMoves(PlaySide engineSide);
  bool isLegalMove(ChessPiece board[8][8], Move m);
  bool isKinginCheck(ChessPiece cpyBoard[8][8], PlaySide engineSide);
  bool isSquareSafe(int row, int col, PlaySide engineSide);
  int isCastling(PlaySide engineSide);
};
#endif
