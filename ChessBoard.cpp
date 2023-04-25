#include "ChessBoard.h"
#include <cstdlib>

using namespace std;
 
MyPiece::MyPiece(Piece m_piesa, PlaySide m_culoare) : piesa(m_piesa), culoare(m_culoare) {}
MyPiece::~MyPiece() {}
Piece MyPiece::getType () {return piesa; }
PlaySide MyPiece::getColor() {return culoare; }

Pawn::Pawn(PlaySide culoare) : MyPiece(Piece::PAWN, culoare) {}
bool Pawn::isLegalMove(const Board& board, Move m) {
        string src = m.getSource().value();
        string dest = m.getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';

        MyPiece* piesa = board.getPiece(src_num, src_lit);
        int direction = (piesa->getColor() == PlaySide::WHITE) ? 1 : -1;

        if (src_lit != dest_lit) {
            // verificam daca putem lua piesa
            MyPiece* dest_piece = board.getPiece(dest_num, dest_lit);
            if (dest_piece->getType() == Piece::EMPTY)
                return false;

            // verificam sa nu fie de aceeasi culoare
            if (dest_piece->getColor() == piesa->getColor())
                return false;

            // verificam sa fie diagonala
            if ((src_lit + 1 == dest_lit || src_lit - 1 == dest_lit) && src_num + direction == dest_num)
                return true;

            return false;
        }
       // std::cerr << "buna! sunt mutarea " << src << " " << dest << " \n";
        if(board.getPiece(dest_num, dest_lit)->getType() != Piece::EMPTY)
            return false;
       // std::cerr << "offf tot aici sunt \n";

        // daca nu e pozitie libera, nu putem muta.
        // pionul e pe alb
        if(direction == 1){
            if(dest_num == src_num + 1){
                return true;
            }
            if(src_num == 1 && dest_num == 3  && board.getPiece(2, src_lit)->getType() == Piece::EMPTY){
                return true;
            }
        }
        else {
            if(src_num == dest_num + 1){
                return true;
            }
            if(src_num == 6 && dest_num == 4 && board.getPiece(5, src_lit)->getType() == Piece::EMPTY){
                return true;
            }
        }
        return false;
    }

Rook::Rook(PlaySide culoare) : MyPiece(Piece::ROOK, culoare) {};
bool Rook::isLegalMove(const Board& board, Move m) {
    string src = m.getSource().value();
    string dest = m.getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';

    int curr_color = board.getPiece(src_num, src_lit)->getColor();
    // verificam randul
    if (src_num == dest_num) {
        if(src_lit == dest_lit)
            return false;
        int start = min(src_lit, dest_lit);
        int end = max(src_lit, dest_lit);
        for (int i = start; i <= end; i++) {
            if (board.getPiece(src_num, i)->getType() != Piece::EMPTY && i != src_lit  && i != dest_lit) {
                return false;  // e o piesa in drum
            }
        }
        return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color|| board.getPiece(dest_num, dest_lit)->getType() == Piece::EMPTY;
    }
    // verificam coloana
    else if (src_lit == dest_lit) {
        int start = min(src_num, dest_num);
        int end = max(src_num, dest_num);
        for (int i = start; i <= end; i++) {
            if (board.getPiece(i, src_lit)->getType() != Piece::EMPTY && i != src_num && i != dest_num) {
                return false;  // e o piesa in drum
            }
        }
        return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color  || board.getPiece(dest_num, dest_lit)->getType() == Piece::EMPTY;
    }
    else return false;
}
Bishop::Bishop(PlaySide culoare) : MyPiece(Piece::BISHOP, culoare) {};
bool Bishop::isLegalMove(const Board& board, Move m) {
    if (!m.isPromotion()) {
        return false;
    }
    string src = m.getSource().value();
    string dest = m.getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';

    
    int d_num = abs(src_num - dest_num);
    int d_lit = abs(src_lit - dest_lit);
    int curr_color = board.getPiece(src_num, src_lit)->getColor();

    // Check if source and destination squares are the same
    if (src_lit == dest_lit && src_num == dest_num) {
        return false;
    }

    // Check if the move is diagonal
    if (d_num != d_lit) {
        return false;
    }

    // Check for pieces in the path of the move
    int x_dir = (dest_lit > src_lit) ? 1 : -1;
    int y_dir = (dest_num > src_num) ? 1 : -1;
    int x = src_lit + x_dir;
    int y = src_num + y_dir;
    while (x != dest_lit && y != dest_num) {
        if (board.getPiece(y, x)->getType() != Piece::EMPTY) {
            return false;
        }
        x += x_dir;
        y += y_dir;
    }

    // Check if the destination square is occupied by a piece of the same color
    if (board.getPiece(dest_num, dest_lit)->getColor() == curr_color) {
        return false;
    }

    return true;
}


Knight::Knight(PlaySide culoare) : MyPiece(Piece::KNIGHT, culoare) {};
bool Knight::isLegalMove(const Board& board, Move m) {
        if(!m.isPromotion())
            return false;
        string src = m.getSource().value();
        string dest = m.getDestination().value();
        int src_lit = src[0] - 'a'; // 1
        int src_num = src[1] - '1'; // 7
        int dest_lit = dest[0] - 'a'; //2
        int dest_num = dest[1] - '1'; // 5

        int curr_color = board.getPiece(src_num, src_lit)->getColor();

        if(board.getPiece(dest_num, dest_lit)->getColor() == curr_color ){
            return false;
        }

        int d_num = abs(src_num - dest_num);
        int d_lit = abs(src_lit - dest_lit);
        return (d_num == 1 && d_lit == 2) || (d_num == 2 && d_lit == 1);
    }

Queen::Queen(PlaySide culoare) : MyPiece(Piece::QUEEN, culoare) {};
bool Queen::isLegalMove(const Board& board, Move m) {
        string src = m.getSource().value();
        string dest = m.getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';
        
        int d_num = abs(src_num - dest_num);
        int d_lit = abs(src_lit - dest_lit);
        int curr_color = board.getPiece(src_num, src_lit)->getColor();

        // verificam obstacolele din calea reginei
        if (d_num == d_lit) { // pe diagonala
            // daca pozitia destinatie e aceeasi cu pozitia sursa
           if (src_lit == dest_lit && src_num == dest_num) {
                return false;
           }
            // Check for pieces in the path of the move
            int x_dir = (dest_lit > src_lit) ? 1 : -1;
            int y_dir = (dest_num > src_num) ? 1 : -1;
            int x = src_lit + x_dir;
            int y = src_num + y_dir;
            while (x != dest_lit && y != dest_num) {
                if (board.getPiece(y, x)->getType() != Piece::EMPTY) {
                    return false;
                }
                x += x_dir;
                y += y_dir;
            }
            // Check if the destination square is occupied by a piece of the same color
            if (board.getPiece(dest_num, dest_lit)->getColor() == curr_color) {
                return false;
            }
            else return true;
        } 
        else if (src_num == dest_num) { // pe orizontala
            if(src_lit == dest_lit)
                return false;
            int start = min(src_lit, dest_lit);
            int end = max(src_lit, dest_lit);
            for (int i = start; i <= end; i++) {
                if (board.getPiece(src_num, i)->getType() != Piece::EMPTY && i != src_lit && i != dest_lit) {
                    return false;  // e o piesa in drum
                }
            }
            return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color || board.getPiece(dest_num, dest_lit)->getType() == Piece::EMPTY;
        } 
        else if (src_lit == dest_lit) { // pe verticala
            int start = min(src_num, dest_num);
                int end = max(src_num, dest_num);
                for (int i = start; i <= end; i++) {
                    if (board.getPiece(i, src_lit)->getType() != Piece::EMPTY && i != src_num && i != dest_num) {
                        return false;  // e o piesa in drum
                    }
                }
                return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color || board.getPiece(dest_num, dest_lit)->getType() == Piece::EMPTY;
            }
        else return false;
    }

King::King(PlaySide culoare) : MyPiece(Piece::KING, culoare) {};
bool King::isLegalMove(const Board& board, Move m) {
        string src = m.getSource().value();
        string dest = m.getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';

        // verificam daca sursa este aceeasi cu destinatia
        if(src_lit == dest_lit && src_num == dest_num){
            return false;
        }
        int d_num = abs(src_num - dest_num);
        int d_lit = abs(src_lit - dest_lit);
        // daca la destinatie ai o piesa de aceeasi culoare
        if(board.getPiece(dest_num, dest_lit)->getColor() == culoare)
            return false;
        return(d_num <= 1 && d_lit <= 1);
}

EmptySquare::EmptySquare(): MyPiece(Piece::EMPTY, PlaySide::NONE) {};
bool EmptySquare::isLegalMove(const Board& board, Move m) {
        return true;
    }

Board::Board() {
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(i == 0){
                    switch(j){
                        case 0: 
                        case 7: board[i][j] = new Rook(PlaySide::WHITE); break;
                        case 1:
                        case 6: board[i][j] = new Knight(PlaySide::WHITE); break;
                        case 2: 
                        case 5: board[i][j] = new Bishop(PlaySide::WHITE); break;
                        case 3: board[i][j] = new Queen(PlaySide::WHITE); break;
                        case 4: board[i][j] = new King(PlaySide::WHITE); break;
                    }
                }
                else if(i == 7){
                     switch(j){
                        case 0: 
                        case 7: board[i][j] = new Rook(PlaySide::BLACK); break;
                        case 1:
                        case 6: board[i][j] = new Knight(PlaySide::BLACK); break;
                        case 2: 
                        case 5: board[i][j] = new Bishop(PlaySide::BLACK); break;
                        case 3: board[i][j] = new Queen(PlaySide::BLACK); break;
                        case 4: board[i][j] = new King(PlaySide::BLACK); break;
                    }
                }
                else if(i == 1){
                    board[i][j] = new Pawn(PlaySide::WHITE);
                }
                else if(i == 6){
                    board[i][j] = new Pawn(PlaySide::BLACK);
                }
                else board[i][j] = new EmptySquare();
            }
        }
    }
Board::~Board() {
        // Free memory for all pieces
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                delete board[i][j];
            }
        }
}
MyPiece* Board::getPiece(int row, int col) const {
        return board[row][col];
}

void Board::movePiece(int startRow, int startCol, int endRow, int endCol) {
    MyPiece* piece = board[startRow][startCol];
    board[startRow][startCol] = new EmptySquare();
    board[endRow][endCol] = piece;
}

void Board::movePiece(Move *m){
        string src = m->getSource().value();
        string dest = m->getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';
        Move::moveTo(m->getSource(), m->getDestination()); 
        movePiece(src_num, src_lit, dest_num, dest_lit);
    }

void Board::undoPiece(Move *m){
    string src = m->getSource().value();
    string dest = m->getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';
    MyPiece* piece = board[src_num][src_lit];
    board[src_num][src_lit] = board[dest_num][dest_lit];
    board[dest_num][dest_lit] = piece;
    Move::moveTo(m->getDestination(), m->getSource());
}