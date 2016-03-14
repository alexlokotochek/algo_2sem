//
//  Graph.h
//  $$$money$$$
//
//  Created by Lokotochek on 25.05.15.
//  Copyright (c) 2015 Alexander Laktionov. All rights reserved.
//

#ifndef dollar_Graph_h
#define dollar_Graph_h

#include <vector>
#include <iostream>
using std::vector;
using std::cin;
using std::cout;

class Table{
    
    friend class Ford_Floyd;
    
public:
    
    ~Table(){
        data.clear();
    }
    
    void getTable(size_t size_input);
    
    
private:
    
    size_t size;
    vector<vector<double>> data;
    
};

void Table::getTable(size_t size_input){
    size = size_input;
    data.assign(size, vector<double> (size));
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j)
            cin >> data[i][j];
    }
}


class Ford_Floyd{
    
public:
    
    explicit Ford_Floyd(Table currency_input){
        currency = currency_input;
        size = currency.size;
    }
    
    ~Ford_Floyd(){
        dist_floyd.clear();
        dist_ford.clear();
        previous_floyd.clear();
        previous_ford.clear();
        answer_ford.clear();
        answer_floyd.clear();
    }
    
    void solve(){
        Ford_Bellman();
        Floyd_Warshall();
    }
    
    
    void printAnswers();
    
private:
    
    size_t size, restore_Ford_node;
    vector<vector<double>> dist_ford, dist_floyd;
    Table currency;
    vector<size_t> previous_ford, answer_ford, answer_floyd;
    vector<vector<size_t>> previous_floyd;
    
    void Ford_Bellman();
    
    void restore_Ford();
    
    void Floyd_Warshall();
    
    void restore_Floyd();
    
    void cutAnswers();
};



void Ford_Floyd::printAnswers(){
    cutAnswers();
    cout << "FORD-BELLMAN:\n";
    if (answer_ford.empty())
        cout << "No sequence\n";
    else{
        for (int i = 0; i < answer_ford.size(); ++i){
            if (answer_ford[i] == answer_ford[0] && i != 0)
                break;
            cout << " -> " << answer_ford[i] + 1;
        }
        cout << " -> " << answer_ford[0] + 1 << " -> \n";
    }
    cout << "\nFLOYD-WARSHALL:\n";
    if (answer_floyd.empty())
        cout << "No sequence\n";
    else{
        for (int i = 0; i < answer_floyd.size(); ++i)
            cout << " -> " << answer_floyd[i] + 1;
        cout << " -> " << answer_floyd[0] + 1 << " -> \n";
    }
}

void Ford_Floyd::cutAnswers(){
    for (int i = 1; i < answer_ford.size(); ++i){
        if (answer_ford[i] == answer_ford[0]){
            answer_ford.end() = answer_ford.begin() + i + 1;
        }
    }
    for (int i = 1; i < answer_floyd.size(); ++i){
        if (answer_floyd[i] == answer_floyd[0]){
            answer_floyd.end() = answer_floyd.begin() + i + 1;
        }
    }
}

void Ford_Floyd::Ford_Bellman(){
    previous_ford.assign(size, -1);
    dist_ford.assign(size + 1, vector<double> (size, 0) );
    
    dist_ford[0][0]= 1;
    for (int iter = 1; iter < size + 1; ++iter){
        for (int i = 0; i < size; ++i){
            
            dist_ford[iter][i] = dist_ford[iter - 1][i];
            for (int j = 0; j < size; ++j){
                
                if (dist_ford[iter][i] < currency.data[i][j] * dist_ford[iter - 1][j] && i != j){
                    previous_ford[i] = j;
                    if (iter == size){
                        restore_Ford_node = i;
                        restore_Ford();
                        return;
                    }
                    dist_ford[iter][i] = currency.data[i][j] * dist_ford[iter - 1][j];
                }
                
            }
        }
    }
}

void Ford_Floyd::restore_Ford(){
    vector<char> marked (size, 0);
    marked[restore_Ford_node] = 1;
    answer_ford.push_back(restore_Ford_node);
    
    auto currentPrevious = restore_Ford_node;
    while(true){
        currentPrevious = previous_ford[currentPrevious];
        answer_ford.push_back(currentPrevious);
        if (marked[currentPrevious] == 1)
            break;
        else
            marked[currentPrevious] = 1;
    }
    
    auto first = answer_ford.begin();
    auto last = answer_ford.end();
    while ((first != last) && (first != --last)) {
        std::swap(*first, *last);
        ++first;
    }
    answer_ford.push_back(answer_ford[0]);
}

void Ford_Floyd::Floyd_Warshall(){
    previous_floyd.assign(size, vector<size_t>(size, -1));
    dist_floyd = currency.data;
    
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            previous_floyd[i][j] = i;
    
    for (int iter = 0; iter < size; ++iter)
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                if (dist_floyd[i][j] < dist_floyd[i][iter] * dist_floyd[iter][j]){
                    dist_floyd[i][j] = dist_floyd[i][iter] * dist_floyd[iter][j];
                    previous_floyd[i][j] = previous_floyd[iter][j];
                }
    restore_Floyd();
}

void Ford_Floyd::restore_Floyd(){
    bool exitAnswer = false;
    for (int i = 0; i < size; ++i){
        if (dist_floyd[i][i] > 1){
            size_t current = i, currentPrevious = previous_floyd[i][i], currentNext;
            while (true){
                if (i == current && exitAnswer)
                    break;
                currentNext = current;
                current = currentPrevious;
                currentPrevious = previous_floyd[currentNext][currentPrevious];
                answer_floyd.push_back(currentNext);
                exitAnswer = true;
            }
        }
    }
}


#endif
