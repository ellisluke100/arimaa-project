#pragma once


#include <cctype>

class Piece
{
public:

	//Enums for type of piece and who owns the piece - only the Piece has these properties
	//"Prefer enum class over plain enum" -> no implicit conversion to int or other types
	enum class Type {
		RABBIT, CAT, DOG, HORSE, CAMEL, ELEPHANT, EMPTY
	};

	enum class Owner {
		SILVER, GOLD, NONE
	};

	//Default constructor
	Piece();

	//Creates piece with these parameters as members
	Piece(char);

	//Parse the char into a type so the program is more readable and manageable overall
	void setupPiece(char);

	void makeEmpty();

	//accessor methods
	Owner getOwner();
	Type getType();
	int getStrength();
	char getChar();

private:
	Owner piece_owner; //the player that owns the piece
	Type piece_type; //what the piece is
	int piece_strength; //relative strength - determines freezing and unfreezing
	char piece_char; //just for displaying in the console representation
};

