#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

// Структура для представления графа
struct Graph {
    int numVertices;
    vector<vector<int>> adjList;

    Graph(int numVertices) : numVertices(numVertices), adjList(numVertices) {}

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
};

// Функция для решения системы уравнений
vector<vector<pair<double, double>>> solveEquations(const vector<pair<int, int>>& edges, int numVertices) {
    vector<vector<pair<double, double>>> solutions;

    // Генерируем все возможные комбинации корней из единицы
    vector<pair<double, double>> roots = { {1, 0}, {-0.5, sqrt(3) / 2}, {-0.5, -sqrt(3) / 2} };
    vector<vector<pair<double, double>>> combinations(numVertices, vector<pair<double, double>>(3));
    for (int i = 0; i < numVertices; ++i) {
        combinations[i] = roots;
    }

    // Проверяем все комбинации
    vector<int> indices(numVertices, 0);
    while (true) {
        bool valid = true;
        for (const auto& edge : edges) {
            if (combinations[edge.first][indices[edge.first]] == combinations[edge.second][indices[edge.second]]) {
                valid = false;
                break;
            }
        }
        if (valid) {
            vector<pair<double, double>> solution(numVertices);
            for (int i = 0; i < numVertices; ++i) {
                solution[i] = combinations[i][indices[i]];
            }
            solutions.push_back(solution);
        }

        // Переходим к следующей комбинации
        int i = numVertices - 1;
        while (i >= 0 && ++indices[i] == 3) {
            indices[i] = 0;
            --i;
        }
        if (i < 0) {
            break;
        }
    }

    return solutions;
}

int main() {
    // Зададим количество вершин
    int NumberOfVertices = 8;
    // Перечислим все ребра нашего графа
    vector<pair<int, int>> EdgesList = { {0,1}, {0,4}, {0,5}, {1,7}, {1,2}, {2,3}, {2,7}, {1,3}, {3,4}, {3,6}, {4,5}, {4,6}, {5,6}, {6,7} };

    // Инициализируем граф
    Graph TestGraph(NumberOfVertices);
    for (const auto& edge : EdgesList) {
        TestGraph.addEdge(edge.first, edge.second);
    }

    // Решаем систему уравнений
    vector<vector<pair<double, double>>> Colorings = solveEquations(EdgesList, NumberOfVertices);
    cout << "The number of colorings is " << Colorings.size() << "." << endl;

    // Если система совместна, то выводим k-ю раскраску.
    // Если нет, то делаем вывод о том, что граф нельзя раскрасить в три цвета.
    if (!Colorings.empty()) {
        // Раскрашиваем вершины графа
        int k = 0;
        vector<pair<double, double>> RawColors = Colorings[k];
        map<pair<double, double>, string> RootsToColors = {
            { {1, 0}, "red" },
            { {-0.5, sqrt(3) / 2}, "green" },
            { {-0.5, -sqrt(3) / 2}, "blue" }
        };

        vector<string> ColorDictionary(NumberOfVertices);
        for (int i = 0; i < NumberOfVertices; ++i) {
            ColorDictionary[i] = RootsToColors[RawColors[i]];
        }

        // Выводим раскраску
        for (int i = 0; i < NumberOfVertices; ++i) {
            cout << "Vertex " << i << " is colored " << ColorDictionary[i] << endl;
        }
    }
    else {
        cout << "The graph is non-colorable." << endl;
    }

    return 0;
}
