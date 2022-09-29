#include <cstdio>
#include <vector>
#include <list>
#include <tuple>
#include <set>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <stack>

bool cyclus(const int &start, const int &end, const std::vector<std::list<std::pair<int, int>>> &graph, const std::vector<std::pair<int, int>> &mstRoads) {
    std::stack<int> toVisit;
    std::set<int> visited;

    toVisit.push(start);
    while(!toVisit.empty()) {
        int v = toVisit.top();
        toVisit.pop();

        if(v == end)
            return true;
        if (!std::binary_search(visited.begin(), visited.end(), v)) {
            visited.insert(v);
            for(const auto &x: graph[v - 1]) {
                for(const auto &road: mstRoads) {
                    if(v == road.first && x.first == road.second)
                        toVisit.push(x.first);
                    else if(v == road.second && x.first == road.first)
                        toVisit.push(x.first);
                }

            }
        }
    }
    return false;

}


void print_neighbours_list(int towns, std::vector<std::list<std::pair<int, int>>> graph) {
    printf("Ispis liste susjeda \n\n");
    for(int i = 0; i < towns; i++) {
        printf("%d: ", i+1);
        for(auto a: graph[i]){
            printf(" '%d' ", a.first);
        }
        printf("\n");

    }
}

void print_districts_sort(const std::vector<std::vector<int>> &districts) {
    printf("Ispis sortiranja po distriktima \n\n");
    int i = 1;
    for(auto d: districts) {
        printf("%d.  ", i);

        for(int a: d){
            printf(" '%d' ", a);
        }
        printf("\n");
        i++;
    }
}

void x() {
    constexpr int MAX_COST = 250;
    unsigned int towns, dtowns, roads;
    scanf("%d %d %d", &towns, &dtowns, &roads);

    std::string str;

    std::vector<std::list<std::pair<int, int>>> graph(towns);

//    printf("Pocelo\n");

    for(int i = 0; i < roads; i++) {
        int Town1, Town2, Cost;
        scanf("%d %d %d", &Town1, &Town2, &Cost);
        // printf("Pravi problem %d - %d %d %d!!!!", i, Town1, Town2, Cost);

        graph[Town1 - 1].push_back(std::make_pair(Town2, Cost));
        graph[Town2 - 1].push_back(std::make_pair(Town1, Cost));

    }

//    print_neighbours_list(towns, graph);

//SORTING TOWNS ACCORDING TO DISTRICTS
    std::vector<std::vector<int>> districts (towns, std::vector<int>(2, 0));

    for(int i = 0; i < dtowns; i++) {
        districts[i][0] = i + 1;
    }

    for(int i = 0; i < towns; i++) {
//        if(districts[i][0] == 0)
//            continue;
        for(const auto &tp: graph[i]){
            int t = tp.first;

            //skip district towns
            if(t <= dtowns)
                continue;
            //if town is not assigned a district assign it
            if(districts[t - 1][0] == 0) {
                districts[t - 1][0] = districts[i][0];
                districts[t - 1][1] = districts[i][1] + 1;
                continue;
            }

                //if a shorter distance could be assigned
            else if (districts[i - 1][1] + 1 < districts[t - 1][1]) {
                districts[t - 1][0] = districts[i][0];
                districts[t - 1][1] = districts[i][1] + 1;
            }

                //if the distance is the same but id could be smaller
            else if (districts[i - 1][1] + 1 == districts[t - 1][1] && districts[i - 1][0] < districts[t - 1][0]) {
                districts[t - 1][0] = districts[i][0];
                districts[t - 1][1] = districts[i][1] + 1;
            }

            else if (districts[t - 1][1] + 1 < districts[i][1]) {
                districts[i][0] = districts[t - 1][0];
                districts[i][1] = districts[t - 1][1] + 1;
            }
        }
    }

//    print_districts_sort(districts);

//KRUSKAL'S ALGORTIHM
    std::vector<int> districtMstLengths(dtowns);

    std::unordered_map<std::string, std::vector<std::tuple<int, int, int>>> districtConnections;

    for(int k = 0; k < dtowns; k++) {
        //       printf("Dtown = %d \n", k + 1);
        //     std::vector<std::tuple<int, int, int>> districtConnection(noConnections);

        std::vector<std::tuple<int, int, int>> districtRoads(0);
        std::set<int> repetition;

        for (int i = 0; i < towns; i++) {
            if (districts[i][0] != k + 1)
                continue;
            for (const std::pair<int, int> &tp: graph[i]) {
                int t = tp.first;

                if (districts[t - 1][0] != k + 1) {
                    districtConnections[std::to_string(k + 1) + '-' + std::to_string(districts[t - 1][0])].emplace_back(i + 1, t, tp.second);
                    continue;
                }

                if (t <= dtowns)
                    continue;

                if(std::binary_search(repetition.begin(), repetition.end(), t))
                    continue;

                repetition.insert( i + 1);
                districtRoads.emplace_back(i + 1, t, tp.second);

            }
        }

//        for (auto el: districtRoads) { printf("%d %d %d \n", std::get<0>(el), std::get<1>(el), std::get<2>(el)); }
//        printf("\n\n\n");

        std::sort(districtRoads.begin(), districtRoads.end(), [] (const std::tuple<int, int, int> &x, const std::tuple<int, int, int> &y) {
            return std::get<2>(x) < std::get<2>(y);
        });

//        for (auto el: districtRoads) { printf("%d %d %d \n", std::get<0>(el), std::get<1>(el), std::get<2>(el)); }
//        printf("\n\n\n");

        //     for (auto el: districtConnection) { printf("%d %d %d \n", std::get<0>(el), std::get<1>(el), std::get<2>(el)); }
        //   printf("\n\n\n");

        std::set<int> mst;
        std::vector<std::pair<int, int>> mstRoads;
        int districtMstLength = 0;
        for(const auto & road : districtRoads) {
            if(std::binary_search(mst.begin(), mst.end(), std::get<0>(road))
               && std::binary_search(mst.begin(), mst.end(), std::get<1>(road))) {
                if(cyclus(std::get<0>(road), std::get<1>(road), graph, mstRoads))
                    continue;
            }

            mst.insert(std::get<0>(road));
            mst.insert(std::get<1>(road));
            mstRoads.emplace_back(std::get<0>(road), std::get<1>(road));
            districtMstLength += std::get<2>(road);
        }

        districtMstLengths[k] = districtMstLength;

//        printf("MST: ");
//        for (auto el: mst) { printf("%d ", el); }
//        printf("\n Length: %d", districtMstLength);
//        printf("\n\n\n");

    }
    int connectionsLength = 0;
    int noConnections = 0;
    for(int i = 0; i < dtowns; i++) {
        for(int j = i + 1; j < dtowns; j++) {
            if(noConnections >= dtowns - 1)
                break;
            std::vector<std::tuple<int, int, int>> connections = districtConnections[std::to_string(i + 1) + '-' + std::to_string(j + 1)];
            if(!connections.empty()) {
                int minConnection = std::get<2>(connections[0]);
                for(int k = 0; k < connections.size(); k++) {
                    if( std::get<2>(connections[k]) < minConnection) {
                        minConnection = std::get<2>(connections[k]);
                    }
                }

                connectionsLength += minConnection;
                noConnections++;
            }
        }
        if(noConnections >= dtowns - 1)
            break;
    }

    for(auto el: districtMstLengths)
        connectionsLength += el;
    printf("Conn length: %d", connectionsLength);
}

void test() {
    //    std::vector<std::list<std::pair<int, int>>> graph;
//    graph.push_back({{2,0}, {3, 0}});
//    graph.push_back({{1,0}, {4, 0}});
//    graph.push_back({{1,0}, {4, 0}, {5, 0}});
//    graph.push_back({{2,0}, {3, 0}, {5,0}});
//    graph.push_back({{3,0}, {4, 0}});
//
//    std::vector<std::pair<int, int>> mst;
//    mst.push_back({1,3});
//    mst.push_back({4, 5});
//    mst.push_back({1, 2});
//    mst.push_back({4, 2});

//    graph.push_back({{2,0}, {5, 0}, {6, 0}});
//    graph.push_back({{1,0}, {3, 0}, {5, 0}});
//    graph.push_back({{2,0}, {4, 0}});
//    graph.push_back({{3,0}, {5, 0}});
//    graph.push_back({{1,0}, {2, 0}, {4, 0}});
//    graph.push_back({{1,0}});
//
//    std::vector<std::pair<int, int>> mst;
//    mst.push_back({1,2});
//    mst.push_back({2, 5});

//    printf("%d", cyclus(4, 3, graph,mst));

}

int main() {

    clock_t start, end;
    start = clock();

    x();
    end = clock();

    printf("\nVrijeme izvrsavanja: %f", double ((end - start) / 1000.));
    return 0;
}
