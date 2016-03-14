#include <iostream>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

class gameMap{
public:
    vector<int> map;
    int stepsFromStart = 0; // шагов до этого состояния
    int badCellsCounter = 0; // сколько не на своем месте
    
    bool recalcBadCells(){
        int counter = 0;
        int lineSize = sqrt(map.size());

        for (int i = 0; i < map.size(); ++i){
            if (map[i] != map.size())
                counter += abs( i/lineSize - (map[i]-1)/lineSize )
                + abs( i%lineSize - (map[i]-1)%lineSize );
        }
        
        // LC
        for (int iter = 0; iter < lineSize; ++iter){
            for(int i = 0; i < lineSize; ++i){

                // rows
                if ( (map[iter*lineSize + i]-1)/lineSize == iter ){
                    for (int j = i + 1; j < lineSize; ++j){
                        if((map[iter*lineSize + j]-1)/lineSize == iter
                           && map[iter*lineSize + i] > map[iter*lineSize + j]
                           && map[iter*lineSize + i] != map.size() && map[iter*lineSize + j] != map.size())
                            counter += 2;
                        }
                }

                // columns
                if ( (map[i*lineSize + iter]-1) % lineSize == iter){
                    for(int j = i + 1; j < lineSize; ++ j){
                        if((map[j * lineSize + iter]-1) % lineSize == iter
                           && map[i*lineSize + iter] > map[j*lineSize + iter]
                           && map[i * lineSize + iter] != map.size() && map[j * lineSize + iter] != map.size())
                            counter += 2;
                    }
                }
            }
        }

        badCellsCounter = counter;
        return (counter == 0);
    }
    
    
    bool operator<(const gameMap &b) const{
        if (stepsFromStart+badCellsCounter != b.stepsFromStart+b.badCellsCounter)
            return stepsFromStart+badCellsCounter < b.stepsFromStart+b.badCellsCounter;
        else
            return map < b.map;
    }
};

class Azvezda {
public:
    explicit Azvezda(vector<int> startSituation){
        start.map = startSituation;
        int lineSize = sqrt(start.map.size());
        direction = {(-1)*lineSize, lineSize, -1, 1};
        if (isSolvable() == true)
            final = azvezdochka(start);
        else
            counter = -1;
    }
    
    // для проверки ответа раскомментировать код в getAns()
    pair<int, vector<string>> getAns(){
        vector<string> answer;
        if (counter != -1){
            answer = getAnswer();
            //if (checkCorrectness(answer, start.map) == false)
            //cout << "----\nWRONG\n----\n";
        }
        return pair<int, vector<string>>(counter, answer);
    }
    
private:
    
    struct directions{
        int UP, DOWN, LEFT, RIGHT;
    };
    
    gameMap start, final;
    int counter = 0;
    directions direction;
    
    int whereIsEmpty (vector<int> &v){
        for (int i = 0; i < start.map.size(); ++i)
            if (v[i] == v.size())
                return i;
        return -1;
    }
    
    set<gameMap> opened;
    map<vector<int>, pair<int, vector<int>>> marked; //situation, <distance, prevSituation>
    
    void checkNeighbour(gameMap &node, int prevStepsFromStart, int action, size_t emptyPos){
        int lastDist = -1; //если еще не смотрели вершину, то новое расстояние - лучшее
        vector<int> previous = node.map;
        swap(node.map[emptyPos], node.map[emptyPos + action]);
        if (marked.count(node.map)){
            lastDist = ((*marked.find(node.map)).second).first;
            node.recalcBadCells();
        }else{
            node.stepsFromStart = prevStepsFromStart + 1;
            node.recalcBadCells();
            opened.insert(node);
            marked.insert(pair<vector<int>, pair<int, vector<int>>>
                          (node.map,
                           pair<int, vector<int>>(node.stepsFromStart+node.badCellsCounter, previous)));
        }
        if (prevStepsFromStart + 1 + node.badCellsCounter < lastDist){
            gameMap old = node;
            old.stepsFromStart = lastDist - old.badCellsCounter;
            opened.erase(old);
            node.recalcBadCells();
            node.stepsFromStart = prevStepsFromStart + 1;
            opened.insert(node);
            auto s = marked.find(node.map);
            s->second = pair<int, vector<int>>(node.stepsFromStart + node.badCellsCounter, previous);
        }
    }
    
    gameMap azvezdochka(gameMap &start){
        gameMap current = start;
        current.recalcBadCells();
        size_t length = sqrt(start.map.size());
        vector<int> empty(start.map.size());
        empty[0] = -1;
        marked.insert(pair<vector<int>, pair<int, vector<int>>>
                      (current.map,
                       pair<int, vector<int>>(current.stepsFromStart+current.badCellsCounter, empty)));
        opened.insert(current);
        gameMap neighbour;
        while (!opened.empty()){
            current = *(opened.begin());
            opened.erase(opened.begin());
            if (current.recalcBadCells())
                return current;
            int emptyPos = whereIsEmpty(current.map);
            neighbour = current;
            if (emptyPos >= length) // we can go up
                checkNeighbour(neighbour, current.stepsFromStart, direction.UP, emptyPos);
            neighbour = current;
            if (emptyPos < length*(length-1)) // we can go down
                checkNeighbour(neighbour, current.stepsFromStart, direction.DOWN, emptyPos);
            neighbour = current;
            if (emptyPos % length != length - 1) // we can go right
                checkNeighbour(neighbour, current.stepsFromStart, direction.RIGHT, emptyPos);
            neighbour = current;
            if (emptyPos % length != 0) // we can go left
                checkNeighbour(neighbour, current.stepsFromStart, direction.LEFT, emptyPos);
        }
        return start; // fail
    }
    
    vector<string> getAnswer(){
        if (final.map == start.map)
            return vector<string>();
        int empCur, empPrev;
        vector<string> steps;
        vector<int> prev = ((*marked.find(final.map)).second).second;
        vector<int> currentSituation = final.map;
        while(true){
            empCur = whereIsEmpty(currentSituation);
            empPrev = whereIsEmpty(prev);
            if (prev[0] == -1)
                break;
            if (empCur == empPrev + direction.LEFT)
                steps.push_back("L");
            if (empCur == empPrev + direction.RIGHT)
                steps.push_back("R");
            if (empCur == empPrev + direction.DOWN)
                steps.push_back("D");
            if (empCur == empPrev + direction.UP)
                steps.push_back("U");
            ++counter;
            currentSituation = prev;
            prev = ((*marked.find(prev)).second).second;
        }
        reverse(steps.begin(), steps.end());
        return steps;
    }
    
    bool checkCorrectness(vector<string> &seq, vector<int> &map){
        for (int i = 0; i < seq.size(); ++i){
            int emptyPos = whereIsEmpty(map);
            int action = 0;
            if (seq[i] == "U")
                action = direction.UP;
            if (seq[i] == "D")
                action = direction.DOWN;
            if (seq[i] == "L")
                action = direction.LEFT;
            if (seq[i] == "R")
                action = direction.RIGHT;
            swap(map[emptyPos], map[emptyPos + action]);
        }
        for (int i = 0; i < map.size(); ++i){
            if (map[i] != i+1)
                return false;
        }
        return true;
    }
    
    bool isSolvable(){
        int inversions = 0;
        int lineSize = sqrt(start.map.size());
        int emptyPos = whereIsEmpty(start.map) / lineSize + 1;
        for (int i = 0; i < start.map.size()-1; ++i){
            for (int j = i + 1; j < start.map.size(); ++j){
                if (start.map[i] != start.map.size()
                    && start.map[j] != start.map.size()
                    && start.map[i] > start.map[j])
                {
                    ++inversions;
                }
            }
        }
        return (emptyPos%2 == inversions%2);
    }
    
};

// для замера времени раскомментировать код в main
int main() {
    int k = 4, n, input;
    cin >> n;
    
    for (int it = 0; it < n; ++it){
        vector<int> start(k*k);
        for (int i = 0; i < start.size(); ++i){
            cin >> input;
            start[i] = ( input == 0 ? k*k : input );
        }
        //auto timeBegin = std::chrono::steady_clock::now();
        Azvezda game(start);
        //auto timeEnd = std::chrono::steady_clock::now();
        //auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeBegin).count()*0.000001;
        //cout << "time: " << totalTime << "\n\n";
        auto answer = game.getAns();
        if (answer.first != -1){
            for (auto &iter: answer.second)
                cout << iter;
            cout << "\n";
            // вывод длины последовательности
            //cout << answer.second.size() << "\n";
        }else{
            cout << "This puzzle is not solvable.\n";
        }
        
    }

    return 0;
}