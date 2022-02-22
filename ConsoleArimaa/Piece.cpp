#include "Piece.h"

//Constructors
Piece::Piece() {
	
}

Piece::Piece(char piece_char) {
	this->piece_char = piece_char;
	this->setupPiece(piece_char);
}

//Parse piece_char into piece type and owner
void Piece::setupPiece(char piece_char) {
	//Set owner of piece - upper for GOLD, lower for SILVER, empty for NONE
	if (piece_char == ' ') {
		this->piece_owner = Owner::NONE;
	} 
	else if (isupper(piece_char)) {
		this->piece_owner = Owner::GOLD;
	}
	else {
		this->piece_owner = Owner::SILVER;
	}

	//Determine type of piece - setting to lower so we can check only one letter
	switch (tolower(piece_char)) {
		case ' ': this->piece_type = Type::EMPTY; this->piece_strength = 0; break;
		case 'r': this->piece_type = Type::RABBIT; this->piece_strength = 1; break;
		case 'c': this->piece_type = Type::CAT; this->piece_strength = 2; break;
		case 'd': this->piece_type = Type::DOG; this->piece_strength = 3; break;
		case 'h': this->piece_type = Type::HORSE; this->piece_strength = 4; break;
		case 'm': this->piece_type = Type::CAMEL; this->piece_strength = 5; break;
		case 'e': this->piece_type = Type::ELEPHANT; this->piece_strength = 6; break;
	}
}

void Piece::makeEmpty() {
	this->piece_char = ' ';
	this->piece_owner = Owner::NONE;
	this->piece_type = Type::EMPTY;
	this->piece_strength = 0;
}

//Accessors
Piece::Owner Piece::getOwner() {
	return this->piece_owner;
}

Piece::Type Piece::getType() {
	return this->piece_type;
}

int Piece::getStrength() {
	return this->piece_strength;
}

char Piece::getChar() {
	return this->piece_char;
}