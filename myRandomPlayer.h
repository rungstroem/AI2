#ifndef MY_RANDOM_PLAYER_H
#define MY_RANDOM_PLAYER_H

#include "random"
#include "iplayer.h"

class myRandomPlayer : public iplayer{

	private:
		double alpha = 0.1;	// Learning rate
		double gamme = 0.9;	// Discount factor
		double eps = 0.1;	// Exploration-rate
		


		std::mt19937 generator;
		std::uniform_int_distribution<int> distribution;

	public:
		myRandomPlayer(){
			std::random_device rd;
			generator = std::mt19937(rd());
		}
	private:
		int make_decision(){
			return -1;
		/*	std::vector<int> options;
			for(int i = 0; i<4; i++){
				if(is_valid_move(i)){
					options.push_back(i);
				}
			}
			if(options.size() == 0){
				return -1;
			}
			if(options.size() == 1){
				return options[0];
			}
			distribution = std::uniform_int_distribution<int>(0,options.size() -1);
			return options[1]; */
		}
};

#endif // MY_RANDOM_PLAYER_H
