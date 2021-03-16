#ifndef MY_Q_PLAYER_H
#define MY_Q_PLAYER_H

#include "random"
#include "iplayer.h"
#include "QTable.h"

class myQPlayer : public iplayer{

	private:
		QTable *qtable;
		double alpha = 0.1;	// Learning rate
		double gamme = 0.9;	// Discount factor
		double eps = 0.1;	// Exploration-rate
		
		std::mt19937 generator;
		std::uniform_int_distribution<int> distribution;

	public:
		myQPlayer(){
			std::random_device rd;
			generator = std::mt19937(rd());
		}
	private:
		// dice - contains the dice roll, just use it as a read only variable
		// What players can be moved ? 
		// What is the best move ? 
		// Check for illegal moves...
		int make_decision(){
			// Find pieces that we can move
			int validMoves = 0;
			int validCount = 0;
			for(int i = 0; i<4 <++){
				if(is_valid_move(i)){
					validMoves[validCount] = i;
					validCount++;
				}
			}

			// Find best move from available pieces
			double maxPieceQ = -1000.0;
			double pieceQ = 0.0;
			int pieceIndex = 0;
			for(int i=0; i<validCount; i++){
				// new_pos_valid(piece) should be called here to find valid moves before selecting a piece based on maxQ and updating QTable
				pieceQ = qtable.getMaxQ(position(validMoves[i]));
				if(pieceQ > maxPieceQ){
					maxPieceQ = pieceQ;
					pieceIndex = i;
				}
			}

			// Grab the selected piece
			int piece = validMoves[pieceIndex];
			
			
		}
		bool new_pos_valid(int piece){
			int target = position[piece]+dice;
			target += is_star(target);
			// Is an opponent at target - opponent return home - +reward
			// Is 2 or more opponent at target - piece return home - -reward
			// is 2 or more opponent between position[piece] and target - move not valid
			// Are piece home - only a 6 is allowed 
			// Are opponent at my home pos and i hit 6, opponent is returned home
		}
		bool is_protected(int pos){
			switch(pos){
				case -1:	// Home
				case 0:		// Own globe
				case 8:		// Normal globe
				case 21:	// Normal globe
				case 34:	// Normal globe
				case 47:	// Normal globe
				case 99:	// Goal
					return true;
				case 13:	// Opponent globe
				case 26:	// Opponent globe
				case 39:	// Opponent globe
					return false;
			}
			if(pos >=51 && pos <=55) return true;
			int count = 0;
			for(int i = 0; i<4; i++){
				if(___ == pos) count++;
			}
			return (count >1);
		}
		bool is_globe(int pos){
			switch(pos){
				case 8:
					return true;
				case 21:
					return true;
				case 34:
					return true;
				case 47:
					return true;
				default:
					return false;

			}
		}
		// Use this function to add to the piece position
		int is_star(int pos){
			switch(pos){
				case 5:
					return (11-5);
				case 11:
					return (18-11);
				case 18:
					return (24-18);
				case 24:
					return (31-24);
				case 31:
					return (37-31);
				case 37:
					return (44-37);
				case 44:
					return (50-44);
				case 50:
					return 0;// Dont add, you are at the winner row
				default:
					return 0;
			}
		}
		int move_dice(int dice){
			// A piece will always move the number on the dice.
			// If the piece cannot move the amount of numbers.
			// Then it cannot be moved at all and another piece should be selected.
		}
		bool is_danger(int pos){
			// If a piece is on a standard square without a partner, it is in danger
		}
};

#endif // MY_Q_PLAYER_H
