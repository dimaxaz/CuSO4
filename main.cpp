#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// ��������� ��� ������������� �����
struct Graph {
    int numVertices;
    vector<vector<int>> adjList;

    Graph(int numVertices) : numVertices(numVertices), adjList(numVertices) {}

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
};

// ������� ��� ������� ������� ���������
vector<vector<pair<double, double>>> solveEquations(const vector<pair<int, int>>& edges, int numVertices) {
    vector<vector<pair<double, double>>> solutions;

    // ���������� ��� ��������� ���������� ������ �� �������
    vector<pair<double, double>> roots = { {1, 0}, {-0.5, sqrt(3) / 2}, {-0.5, -sqrt(3) / 2} };
    vector<vector<pair<double, double>>> combinations(numVertices, vector<pair<double, double>>(3));
    for (int i = 0; i < numVertices; ++i) {
        combinations[i] = roots;
    }

    // ��������� ��� ����������
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

        // ��������� � ��������� ����������
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
    // ������� ���������� ������
    int NumberOfVertices = 8;
    // ���������� ��� ����� ������ �����
    vector<pair<int, int>> EdgesList = { {0,1}, {0,4}, {0,5}, {1,7}, {1,2}, {2,3}, {2,7}, {1,3}, {3,4}, {3,6}, {4,5}, {4,6}, {5,6}, {6,7} };

    // �������������� ����
    Graph TestGraph(NumberOfVertices);
    for (const auto& edge : EdgesList) {
        TestGraph.addEdge(edge.first, edge.second);
    }

    // ������ ������� ���������
    vector<vector<pair<double, double>>> Colorings = solveEquations(EdgesList, NumberOfVertices);
    cout << "The number of colorings is " << Colorings.size() << "." << endl;

    // ���� ������� ���������, �� ������� k-� ���������.
    // ���� ���, �� ������ ����� � ���, ��� ���� ������ ���������� � ��� �����.
    if (!Colorings.empty()) {
        // ������������ ������� �����
        int k = 0;
        vector<pair<double, double>> RawColors = Colorings[k];
        map<pair<double, double>, Color> RootsToColors = {
            { {1, 0}, Color::Red },
            { {-0.5, sqrt(3) / 2}, Color::Green },
            { {-0.5, -sqrt(3) / 2}, Color::Blue }
        };

        vector<Color> ColorDictionary(NumberOfVertices);
        for (int i = 0; i < NumberOfVertices; ++i) {
            ColorDictionary[i] = RootsToColors[RawColors[i]];
        }

        // ������� ����
        RenderWindow window(VideoMode(800, 600), "Graph Coloring");

        // ���������� ���������� ������
        vector<Vector2f> vertexPositions;
        float radius = 200.0f;
        float angle = 2.0f * 3.14159f / NumberOfVertices;
        for (int i = 0; i < NumberOfVertices; ++i) {
            float x = 400.0f + radius * cos(i * angle);
            float y = 300.0f + radius * sin(i * angle);
            vertexPositions.push_back(Vector2f(x, y));
        }

        // �������� ����
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            window.clear();

            // ������ �������
            for (int i = 0; i < NumberOfVertices; ++i) {
                CircleShape vertex(10.0f);
                vertex.setFillColor(ColorDictionary[i]);
                vertex.setPosition(vertexPositions[i] - Vector2f(10.0f, 10.0f));
                window.draw(vertex);
            }

            // ������ �����
            for (const auto& edge : EdgesList) {
                VertexArray line(Lines, 2);
                line[0].position = vertexPositions[edge.first];
                line[1].position = vertexPositions[edge.second];
                line[0].color = Color::Black;
                line[1].color = Color::Black;
                window.draw(line);
            }

            window.display();
        }
    }
    else {
        cout << "The graph is non-colorable." << endl;
    }

    return 0;
}
