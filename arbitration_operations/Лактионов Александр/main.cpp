#include "Graph.h"

int main() {
    
    size_t n;
    cin >> n;

    Table currency;
    currency.getTable(n);
    cout << "\n";
    Ford_Floyd graph(currency);
    graph.solve();
    graph.printAnswers();

    return 0;
}