/*							Actions
 *				Piece1	Piece2	Piece3	Piece4
 * */

/*
 * Maybe create functions that can count how many times a state is visited and how many times
 * an action is taken - for statistics afterward
 */

#include <time.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>

const int statesV2 = 256;
const int actionsV2 = 4;

class QTableV2{

	public:
		int getMaxQAction(int state){
			stateVisited[state] +=1;	//Log how many times a state is selected
			int maxVal = -1000;
			int actionIndex = -1;
			std::vector<int> indexes;
			for(int i = 0;i<actionsV2;i++){
				if(Qtable[state][i] == maxVal){	//This is an error
					indexes.push_back(i);
				}
				if(Qtable[state][i] > maxVal){
					maxVal = Qtable[state][i];
					actionIndex = i;
				}
			}
			if(indexes.size() > 1){
				int i = 0;
				std::srand(time(0));
				i = std::rand()%indexes.size();
				actionIndex = indexes[i];
			}
			return(actionIndex);
		}
		void setQVal(int state, int action, double val){
			Qtable[state][action] = val;
		}
		double getQVal(int state, int action){
			return Qtable[state][action];
		}
		void saveQ(){
			std::ofstream table;
			table.open("qtableV2.csv");
			if(table.is_open()){
				for(int i=0; i<statesV2; i++){
					for(int j=0; j<actionsV2; j++){
						table << Qtable[i][j] << ",";
					}
					table << "\n";
				}
			}
			table.close();
			std::ofstream stv;
			stv.open("StatesVisitedV2.csv");
			if(stv.is_open()){
				for(int i=0; i<statesV2; i++){
					stv << stateVisited[i] << ',';
				}
				stv.close();
			}
		}
		void restoreQ(){
			std::string line;
			std::string num;
			double number = 0.0;
			int k = 0;
			int l = 0;
			std::ifstream table("qtableV2.csv");
			if(table.is_open()){
				while(getline(table, line)){
					l = 0;
					for(int i=0; i<line.length(); i++){
						if(line[i] == ','){
							number = strtod(num.c_str(), NULL);
							Qtable[k][l] = number;
							l++;
							num.clear();
							continue;
						}
						num.push_back(line[i]);
					}
					k++;
				}
				table.close();
			}
		}
	private:
		double Qtable[statesV2][actionsV2] = {0};
		int stateVisited[statesV2] = {0};
};
