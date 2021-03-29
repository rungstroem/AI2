#ifndef MY_Q_PLAYER_H
#define MY_Q_PLAYER_H

#include "random"
#include "iplayer.h"
#include "QTable.h"

enum{
	home,
	goal,
	globe,
	star,
	winRoad,
	satefy,
	freeSpace,
	danger
}

enum{
	moveOut,
	getInGoal,
	moveGlobe,
	moveStar,
	goalViaStar,
	winRoad,
	safety,
	kill,
	move,
	noMoves
}

// Q(s,a) = Q(s,a) + alpha*( reward(s) + gamma*maxQ(s+1,a+1,) - Q(s,a) )

class myQPlayer : public iplayer{

	private:
		QTable *qtable;
		double alpha = 0.1;	// Learning rate
		double gamme = 0.9;	// Discount factor
		double eps = 0.1;	// Exploration-rate
		int newPos[16] = {0};
		int numOfStates = 8;
		double exploration  = 0;
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
			exploration = 0;
			// Find pieces that we can move
			int validMoves[4] = 0;
			int validCount = 0;
			for(int i = 0; i<4; i++){
				if(is_valid_move(i)){
					validMoves[validCount] = i;
					validCount++;
				}
			}

			// Find best move from available pieces
			double maxPieceQ = -1000.0;
			double pieceQ = 0.0;
			int pieceIndex = 0;
			// extend with max reward for action
			for(int i=0; i<validCount; i++){
				int pos = -1;
				int state = -1;
				int action = -1;
				pos = get_new_pos(validMoves[i]);
				state = state_translate(pos, validMoves[i]);
				// Get best actions for state
				action = qtable.getMaxQAction(state);
				// Get Q-val for from action
				pieceQ = qtable.getQVal(state,action);
				// max reward
				if(pieceQ > maxPieceQ){
					maxPieceQ = pieceQ;
					pieceIndex = i;
				}
			}
			// I have the piece, the pieces' state and the pieces' action
			// Grab the selected piece
			int piece = validMoves[pieceIndex];
			// Move the selected piece and find state
			int pos = position[piece];
			int state = state_translator(pos, piece);
			int action = qtable.getMaxQAction(state);
			
			int newPos = get_new_pos(piece);
			int newState = state_translator(newPos, piece);
			srand(time(NULL));
			exploration = (double)(rand()%1000)/1000;
			if(exploration > eps){
				newState = rand()%10;
				newState = newState+piece*numOfStates;
			}
			int newAction = getMaxQAction(newState);
			int reward = get_reward(newState, piece);
			
			double Q = getQVal(state,action);
			double Qn = getQVal(newState, newAction);
			Q = Q +alpha*(reward + gamma*Qn-Q);
			setQVal(state, action, Q);
			
			return position(piece);
			
			
		}
		int get_new_pos(int piece){
			int target = position[piece]+dice;
			target += get_star(target);
			return target;
		}
		double get_reward(int state, int piece){
			switch(state){
				case home:
					return -1.0;
				case goal +numOfStates*piece:
					return 10.0;
				case globe +numOfStates*piece: 
					return 2.0;
				case winRoad +numOfStates*piece:
					return 5.0;
				case safety +numOfStates*piece:
					return 2.0;
				case freeSpace +numOfStates*piece:
					return 1.0;
				case danger +numOfStates*piece:
					return -1.0;
			}
		}
		int state_translate(int pos, int piece){
			// Translates piece position to state type. 
			int state;
			if(pos == -1){
				state = home +numOfStates*piece;
			}else if(pos == 99){
				state = goal +numOfStates*piece;
			}else if(is_globe(pos)){
				state = globe +numOfStates*piece;
			}else if(is_star(pos)){
				state = star +numOfStates*piece;
			}else if(pos >= 51 && pos <= 56){
			       	state = winRoad +numOfStates*piece;
			}else if(is_safe(piece)){
				state = safety +numOfStates*piece;
			}else if(is_danger(pos)){
				state = danger +numOfStates*piece;
			}else{
				state = freeSpace +numOfStates*piece;
			}
			return state;
		}
			// Is an opponent at target - opponent return home - +reward
			// Is 2 or more opponent at target - piece return home - -reward
			// is 2 or more opponent between position[piece] and target - move not valid
			// Are piece home - only a 6 is allowed 
			// Are opponent at my home pos and i hit 6, opponent is returned home
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
				case 0:
					return true;
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
		int get_star(int pos){
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
		int is_star(int pos){
			switch(pos){
				case 5:
					return true;
				case 11:
					return true;
				case 18:
					return true;
				case 24:
					return true;
				case 31:
					return true;
				case 37:
					return true;
				case 44:
					return true;
				case 50:
					return true;// Dont add, you are at the winner row
				default:
					return false;
			}
		}
		bool is_safe(int piece){
			int otherPiece = -1;
			for(int i = 0;i<4; i++){
				otherPiece = is_valid_move(i);
				if(piece != otherPiece){
					if(position(piece) == position(otherPiece)){
						return true;
					}
				}
			}
			return false;
		}
		bool is_danger(int pos){
			for(int i = 0;i<7;i++){
				// If one or more opponents are less than squares behind, then my piece is in danger
				if(!is_protected(pos) && count_opponents(pos-i)>0){
					return true;
				}
			}
			return false;
		}
};

#endif // MY_Q_PLAYER_H
