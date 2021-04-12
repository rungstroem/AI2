#ifndef MY_Q_PLAYER_H
#define MY_Q_PLAYER_H

#include "random"
#include "iplayer.h"
#include "QTable.h"
#include <iostream>
#include <fstream>

enum{
	home,
	goal,
	star,
	winRoad,
	safety,
	freeSpace,
	danger
};

enum{
	moveOut,
	moveSafe,
	moveFree,
	moveWinRoad,
	moveGoal,
	moveStar,
	moveGoalStar
};

class myQPlayer : public iplayer{

	private:
		QTable qtable;
		double alpha = 0.5;	// Learning rate
		double gamma = 0.9;	// Discount factor
		double eps = 0.3;	// Exploration-rate
		int expInt = 30;

		int numOfStates = 7;

		int diceVal = 0;
		int piecePos[16] = {0};
		int pieceState[4] = {0};
		int pieceAction[4] = {0};
		int pieceQVal[4] = {0};
		int indexQMax = -1;
		int indexRandom = -1;
		int deadIndexes[12] = {0};

		int newPos = 0;
		int newState = 0;
		int maxQIndex = 0;
		double reward = 0.0;
		double QVal = 0.0;
		
		double rewardAccumulated = 0.0;
		
		int oldIndex = -1;
		int oldState = -1;
		int oldAction = -1;
		double oldQVal = 0.0;
		std::random_device rd;
		std::mt19937 genRandInt;

	public:
		myQPlayer(){
			genRandInt = std::mt19937(rd());
		}
		void saveQ(){
			// save Qtable to file
			qtable.saveQ();
		}
		void restoreQ(){
			qtable.restoreQ();
		}
		void saveReward(){
			std::ofstream fout;
			std::ifstream fin;
			fin.open("AccumulatedReward.csv");
			fout.open("AccumulatedReward.csv", std::ios::app);
			if(fin.is_open()){
				fout << rewardAccumulated << ",\n";	// Writing the reward to the file
			}
			fin.close();
			fout.close();
			// Reset acculumated reward
			rewardAccumulated = 0.0;
		}
	private:
		// dice - contains the dice roll, just use it as a read only variable
		// What players can be moved ? 
		// What is the best move ? 
		// Check for illegal moves...
		int make_decision(){

			//Get all pieces position
			for(int i=0; i<16; i++){
				piecePos[i] = position[i];
			}
			//Calculate the state for all of my pieces
			for(int i=0; i<4; i++){
				pieceState[i] = get_state(piecePos[i], i);
			}

			if(oldIndex == -1){
				// Skip
			}else{
				//Grab the position of the piece we just moved
				newPos = piecePos[oldIndex];
				//Calculate that pieces new state - where does the environment place us
				newState = pieceState[oldIndex];
				//Calculate the reward
				reward = get_reward(newState, oldIndex, newPos);
				rewardAccumulated += reward;
				//Send opponents home...
				send_them_home();
				//Get previous Q-value
				oldQVal = qtable.getQVal(oldState, oldAction);
				//Get max action index in new state
				maxQIndex = qtable.getMaxQAction(newState);
				//Calculate update val
				QVal = oldQVal + alpha*(reward +gamma*qtable.getQVal(newState, maxQIndex)-oldQVal);
				//Update the old state action pair in the QTable
				qtable.setQVal(oldState, oldAction, QVal);

			}
			//Start a new move
			// Initialize for every episode
			diceVal = dice;

			// Calculate action from my pieces positions and the dice value
			for(int i=0; i<4; i++){
				if(pieceState[i] == 99){	//If a piece is on the goal, then it should not do anything anymore
					pieceAction[i] == -1;
					continue;
				}
				pieceAction[i] = get_action(diceVal, piecePos[i]);
			}
			// Get the Q value for every pieces' state action pair
			for(int i=0; i<4; i++){
				if(pieceAction[i] == -1){	//Set the q val for a piece at goal low, so it won't be selected as index to move
					pieceQVal[i] == -1000;
					continue;
				}
				pieceQVal[i] = qtable.getQVal(pieceState[i], pieceAction[i]);
			}
			// Get the index of the maximum QVal between all pieces Q values
			indexQMax = get_maxQIndex();
			// Add random exploration to equation
			indexRandom = random_exploration(indexQMax);
			// Index is the piece we select to move
			
			oldIndex = indexRandom;
			oldAction = pieceAction[oldIndex];
			oldState = pieceState[oldIndex];
			
			return indexRandom;
		}
		int get_action(int diceV, int piece){
			int pos = piecePos[piece];
			int newPos = pos + diceV;
			int action = 0;
			if(newPos > 4){
				action = moveOut;
			}
			if(is_safe(piece, pos)){
				action = moveSafe;
			}
			if(is_free(newPos, piece)){
				action = moveFree;
			}
			if(is_winRoad(newPos)){
				action = moveWinRoad;
			}
			if(newPos > 56){
				action = moveGoal;
			}
			if(is_star(newPos)){
				action = moveStar;
			}
			if(is_goal_star(newPos)){
				action = moveGoalStar;
			}
			return action;

		}
		double get_reward(int state, int piece, int newPos){	//The reward function may not work properly
			double accumulatedReward = 0.0;
			if(kill(newPos) == true){
				accumulatedReward += 2.0;
			}else if(kill_on_home(newPos) == true){
				accumulatedReward += 3.0;
			}else if(die(newPos) == true){
				accumulatedReward -= 10.0;
			}else if(is_goal_star(newPos) == true){
				accumulatedReward += 10;
				return accumulatedReward;
			}
			if(state == home +numOfStates*piece) return -1.0;
			if(state == goal +numOfStates*piece) return 100.0;
			if(state == winRoad +numOfStates*piece) return 5.0;
			
			if(state == safety +numOfStates*piece) return 2.0 + accumulatedReward;
			if(state == freeSpace +numOfStates*piece) return 1.0 + accumulatedReward;
			if(state == danger +numOfStates*piece) return -1.0 + accumulatedReward;
			return 0.0;
		}
		int get_state(int pos, int piece){
			// Translates piece position to state type. 
			int state;
			if(pos == -1){
				return home +numOfStates*piece;	//If piece is home then nothing can get to it
			}
			if(pos == 99){
				state = goal;
			}
			if(is_star(pos)){
				state = star;
			}
			if(is_winRoad(pos)){
				state = winRoad;
			}
			if(is_protected(pos) || is_safe(piece, pos)){
				state = safety;
			}
			if(is_danger(pos, piece)){
				state = danger;
			}
			if(is_free(pos, piece)){
				state = freeSpace;	
			}
			return state +numOfStates*piece;
		}
		void send_them_home(){
			for(int i=4; i<16; i++){
				piecePos[i] = deadIndexes[i-4];
			}
		}
		bool kill_on_home(int pos){
			int count = 0;
			for(int i=4; i<16; i++){
				if(pos == 0 && pos == piecePos[i]){
					deadIndexes[i-4] = -1;
					count++;
				}
			}
			if(count > 0){
				return true;
			}
			return false;

		}
		bool kill(int pos){
			int count = 0;
			for(int i=4; i>16; i++){
				if(pos == piecePos[i]){
					deadIndexes[i-4] = -1;
					count++;
				}
			}
			if(count == 1){
				return true;
			}
			return false;
		}
		bool die(int pos){
			int count = 0;
			for(int i=4; i<16; i++){
				if(pos == piecePos[i]){
					count++;
				}
			}
			if(count > 1){
				return true;
			}
			return false;
		}
		int get_maxQIndex(){
			int index = -1;
			double maxVal = -100.0;
			for(int i=0; i<4; i++){
				if(pieceQVal[i] > maxVal){
					maxVal = pieceQVal[i];
					index = i;
				}
			}
			return index;
		}
		int random_exploration(int index){
			int indexRandom = index;
			std::uniform_int_distribution<int> distribution = std::uniform_int_distribution<int>(0,100);
			if(distribution(genRandInt) > expInt){
				std::uniform_int_distribution<int> distribution2 = std::uniform_int_distribution<int>(0,4-1);
				while(1){	//This loop is made so the index of a piece on goal is not selected
					indexRandom = distribution2(genRandInt);
					if(pieceState[indexRandom] == 99){
						continue;
					}else{
						break;
					}
				}
			}
			return indexRandom;
		}
		int get_new_pos(int pos, int diceV){
			int newPos = pos + diceV;
			newPos += move_star(newPos);
			if(die(newPos) == true){
				return -1;
			}
			return newPos;
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
					return true;
				case 13:	// Opponent globe
				case 26:	// Opponent globe
				case 39:	// Opponent globe
					return false;
			}
			return false;
		}
		bool is_winRoad(int pos){
			if(pos >=51 && pos <=55){
				return true;
			}
			return false;
		}
		bool is_safe(int piece, int pos){
			for(int i=0; i<4; i++){
				if(piece == i){
					continue;
				}
				if(pos == piecePos[i]){
					return true;
				}
			}
			return false;
		}
		// Use this function to add to the piece position
		int move_star(int pos){
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
					return 99;	// Last star takes you directly to home
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
					return true;	// This star takes you home
				default:
					return false;
			}
		}
		bool is_goal_star(int pos){
			if(pos == 50){
				return true;
			}
			return false;
		}
		bool is_danger(int pos, int piece){
			bool helper = false;
			for(int i = 0;i<7;i++){
				// If one or more opponents are less than squares behind, then my piece is in danger
				if(!is_protected(pos) && count_opponents(pos-i)>0){
					helper = true;
				}
			}
			if(helper && !is_protected(pos) && !is_safe(piece, pos) && !is_winRoad(pos)){
				return true;
			}
			return false;
		}
		bool is_free(int pos, int piece){
			if(!is_danger(pos, piece) && !is_star(pos) && !is_safe(piece, pos) && !is_protected(pos) && !is_winRoad(pos)){
				return true;
			}
			return false;
		}
		int count_opponents(int pos){
			int count = 0;
			for(int i = 4; i< 16; i++){
				if(position[i] == pos){
					count++;
				}
			}
			return count;
		}
};

#endif // MY_Q_PLAYER_H
