#ifndef MY_Q_PLAYERV2_H
#define MY_Q_PLAYERV2_H

#include "random"
#include "iplayer.h"
#include "QTableV2.h"
#include <iostream>
#include <fstream>


class myQPlayerV2 : public iplayer{

	private:
		QTableV2 qtable;
		double alpha = 0.1;	// Learning rate
		double gamma = 0.1;	// Discount factor
		int eps = 100;		// Exploration rate

		int diceVal = 0;
		int piecePos[16] = {0};

		double reward = 0.0;
		double QVal = 0.0;
		
		double rewardAccumulated = 0.0;
	
		int maxQIndex;
		
		int oldAction = 0;
		unsigned char oldState = 0;
		unsigned char state = 0;
		unsigned char pieceState = 0;

		int posSum = 0;

	public:
		myQPlayerV2(){
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
			fin.open("AccumulatedRewardV2.csv");
			fout.open("AccumulatedRewardV2.csv", std::ios::app);
			if(fin.is_open()){
				fout << rewardAccumulated << ",\n";	// Writing the reward to the file
			}
			fin.close();
			fout.close();
			// Reset acculumated reward
			rewardAccumulated = 0.0;
		}
		int decrease_exploration_rate(int i=0){
			eps = (int)((exp(-0.01*i))*100);
			return eps;
		}
		void set_discount(double g){
			gamma = g;
		}
		void set_learning_rate(double a){
			alpha = a;
		}
	private:
		// dice - contains the dice roll, just use it as a read only variable
		// What players can be moved ? 
		// What is the best move ? 
		// Check for illegal moves...
		int make_decision(){
			// Initialize for every episode
			diceVal = dice;
			
			// Get all pieces position
			for(int i=0; i<16; i++){
				piecePos[i] = position[i];
			}
			

			for(int i=0;i<4;i++){
				pieceState = 0;
				if(kill(piecePos[i], diceVal) == true){
					pieceState = pieceState | 2; 
				}
				if(be_killed(piecePos[i], diceVal) == true){
					pieceState = pieceState | 1;
				}
				state = state | pieceState<<i*2;
			}

			// Get the max action index in new state
			maxQIndex = qtable.getMaxQAction(state);
			
			// randomness 
			int randomIndex = random_exploration(maxQIndex);

			// Recieve the reward for the new state
			reward = get_reward(state);
			//rewardAccumulated += reward;
			
			// Get the old q value
			double oldQVal = qtable.getQVal(oldState, oldAction);

			// The equation
			QVal = oldQVal + alpha*(reward +gamma*qtable.getQVal(state, maxQIndex)-oldQVal);
			rewardAccumulated += QVal;
			// Update table
			qtable.setQVal(oldState, oldAction, QVal);

			oldAction = randomIndex;
			oldState = state;

			return randomIndex;

		}

		int random_exploration(int index){
			int indexRandom = index;
			srand(time(NULL));
			if(rand()%100 > eps){
				std::vector<int> validIndex;
				for(int i=0; i<4; i++){
					if(piecePos[i] == 99 || piecePos[i] == 56){
						continue;
					}
					validIndex.push_back(i);
				}
				if(validIndex.size() > 1){
					srand(time(NULL));
					indexRandom = validIndex[rand()%validIndex.size()];
				} else if(validIndex.size() == 1){
					indexRandom = validIndex[0];
				}else{
					indexRandom = -1;
				}
			}
			
			return indexRandom;
		}

		bool kill(int pos, int dice){
			int count = 0;
			for(int i=4; i<16; i++){
				if((pos+dice) == piecePos[i]){
					count ++;
				}
			}
			if(count == 1){
				return true;
			}else{
				return false;
			}
		}
		bool be_killed(int pos, int dice){
			int count = 0;
			for(int i=4; i<16; i++){
				if((is_globe(piecePos[i])  == (pos+dice)) && piecePos[i] != 0){
					return true;
				}
				if((pos+dice) == piecePos[i]){
					count++;
				}
			}
			if(count >1){
				return true;
			}
			return false;

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
		int get_reward(int pos){
			if(pos > posSum){
				pos = posSum;
				return +1;
			}
			if(pos < posSum){
				pos = posSum;
				return -1;
			}
			if(pos == posSum){
				pos = posSum;
				return 0;
			}
			return 0;
		}
};

#endif // MY_Q_PLAYERV2_H
