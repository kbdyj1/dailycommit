#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace { //================================================================

enum class City : int {
    Moscow,
    London,
    Seoul,
    Seattle,
    Dubai,
    Sydney
};

ostream& operator<<(ostream& os, City city)
{
    switch (city) {
    case City::Moscow: os << "Moscow";
        break;
    case City::London: os << "London";
        break;
    case City::Seoul: os << "Seoul";
        break;
    case City::Seattle: os << "Seattle";
        break;
    case City::Dubai: os << "Dubai";
        break;
    case City::Sydney: os << "Sydney";
        break;
    }
    return os;
}

auto setupGraph = [](auto& graph)
{
    graph.addEdge(City::London, City::Moscow, 2500);
    graph.addEdge(City::London, City::Seoul, 9000);
    graph.addEdge(City::London, City::Dubai, 5500);
    graph.addEdge(City::Seoul, City::Moscow, 6600);
    graph.addEdge(City::Seoul, City::Seattle, 8000);
    graph.addEdge(City::Seoul, City::Dubai, 7000);
    graph.addEdge(City::Seoul, City::Sydney, 8000);
    graph.addEdge(City::Seattle, City::Moscow, 8400);
    graph.addEdge(City::Seattle, City::Sydney, 12000);
    graph.addEdge(City::Dubai, City::Sydney, 1200);
    graph.addEdge(City::Seattle, City::London, 8000);
    graph.removeEdge(City::Seattle, City::London);
};

namespace _1 {

template <int N>
struct Graph
{
    vector<vector<int>> data;

    Graph()
    {
        data.reserve(N);
        auto row = vector<int>(N);
        fill(row.begin(), row.end(), -1);
        for (int i=0; i<N; i++) {
            data.push_back(row);
        }
    }

    void addEdge(City c0, City c1, int distance)
    {
        auto n0 = static_cast<int>(c0);
        auto n1 = static_cast<int>(c1);

        data[n0][n1] = distance;
        data[n1][n0] = distance;

        if (0 < distance) {
            cout << "Add edge: " << c0 << " - " << c1 << " : " << distance << "km\n";
        } else {
            cout << "Remove edge: " << c0 << " - " << c1 << "\n";
        }
    }

    void removeEdge(City c0, City c1)
    {
        addEdge(c0, c1, -1);
    }
};

void test()
{
    auto graph = Graph<6>();

    setupGraph(graph);
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <int N>
struct Graph {
    vector<vector<pair<int,int>>> data;

    Graph() : data(vector<vector<pair<int,int>>>(N, vector<pair<int,int>>()))
    {
    }

    void addEdge(City c0, City c1, int distance)
    {
        auto n0 = static_cast<int>(c0);
        auto n1 = static_cast<int>(c1);
        data[n0].push_back({n1, distance});
        data[n1].push_back({n0, distance});

        cout << "Add edge: " << c0 << " - " << c1 << " : " << distance << "km\n";
    }

    void removeEdge(City c0, City c1)
    {
        auto n0 = static_cast<int>(c0);
        auto n1 = static_cast<int>(c1);

        remove_if(data[n0].begin(), data[n0].end(), [n1](const auto& pair){
            return pair.first == n1;
        });
        remove_if(data[n1].begin(), data[n1].end(), [n0](const auto& pair){
            return pair.first == n0;
        });
        cout << "Remove edge: " << c0 << " - " << c1 << "\n";
    }
};

void test()
{
    auto graph = Graph<6>();

    setupGraph(graph);
}

} //_2 --------------------------------------------------------------

} //==========================================================================

void test_ch_02_graph()
{
#if (0)
    _1::test();
#endif

    _2::test();
}
