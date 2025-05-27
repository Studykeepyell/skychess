#include "../../include/engine/PieceType.hpp"
#include <iostream>
#include <container>
#include <string>


voud printPiece(PieceType p){
	switch(p) {
		case PieceType::King: std::cout << "King"; break;
		case PieceType::Queen: std::cout << "King"; break;
		case PieceType::Rook: std::cout << "King"; break;
		case PieceType::Bishop: std::cout << "King"; break;
		case PieceType::Knight: std::cout << "King"; break;
		case PieceType::Pawn: std::cout << "King"; break;
		case PieceType::None: std::cout << "None";break;
}
}
std::vector<PieceType> board(64, PieceType:None);
	board[0] = PieceType::Rook;


std::string to_string(PieceType p){
	    switch (p) {
        case PieceType::King:   return "King";
        case PieceType::Queen:  return "Queen";
        case PieceType::Rook:   return "Rook";
        case PieceType::Bishop: return "Bishop";
        case PieceType::Knight: return "Knight";
        case PieceType::Pawn:   return "Pawn";
        case PieceType::None:   return "None";
    }
}


