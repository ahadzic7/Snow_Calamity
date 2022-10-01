#include <cstdio>
#include <vector>
#include <list>
#include <tuple>
#include <set>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <stack>

void districtsCheck(const std::vector<std::vector<int>>& districts);

void print_mst(std::set<int> mst, int districtMstLength) {
    printf("MST: ");
    for (auto el: mst) { printf("%d ", el); }
    printf("\n Length: %d", districtMstLength);
    printf("\n\n\n");

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

void readData(int &towns, int &dtowns, int &roads, std::vector<std::list<std::pair<int, int>>> &graph) {
    scanf("%d %d %d", &towns, &dtowns, &roads);

    std::vector<std::list<std::pair<int, int>>> g(towns);
    graph = std::move(g);

    for(int i = 0; i < roads; i++) {
        int Town1, Town2, Cost;
        scanf("%d %d %d", &Town1, &Town2, &Cost);

        graph[Town1 - 1].push_back(std::make_pair(Town2, Cost));
        graph[Town2 - 1].push_back(std::make_pair(Town1, Cost));
    }

//    print_neighbours_list(towns, graph);
}

void districtSort(int towns, int dtowns, const std::vector<std::list<std::pair<int, int>>> &graph, std::vector<std::vector<int>> &districts) {
    for(int i = 0; i < dtowns; i++)
        districts[i][0] = i + 1;

    int counter = towns - dtowns;

    do {
        for (int i = 0; i < towns; i++) {
            for (const auto &tp: graph[i]) {
                int t = tp.first;

                //skip district towns
                if (t <= dtowns)
                    continue;

                if (districts[t - 1][0] == 0 && districts[i][0] == 0)
                    continue;

                    //if town is not assigned a district assign it
                else if (districts[t - 1][0] == 0) {
                    districts[t - 1][0] = districts[i][0];
                    districts[t - 1][1] = districts[i][1] + 1;
                    counter--;
                } else if (districts[i][0] == 0) {
                    districts[i][0] = districts[t - 1][0];
                    districts[i][1] = districts[t - 1][1] + 1;
                    counter--;
                }
                    //if a shorter distance could be assigned
                else if (districts[i][1] + 1 < districts[t - 1][1]) {
                    districts[t - 1][0] = districts[i][0];
                    districts[t - 1][1] = districts[i][1] + 1;
                }
                else if (districts[t - 1][1] + 1 < districts[i][1]) {
                    districts[i][0] = districts[t - 1][0];
                    districts[i][1] = districts[t - 1][1] + 1;
                }
                    //if the distance is the same but id could be smaller
                else if (districts[i][1] + 1 == districts[t - 1][1] && districts[i][0] < districts[t - 1][0]) {
                    districts[t - 1][0] = districts[i][0];
                    districts[t - 1][1] = districts[i][1] + 1;
                }
                else if (districts[i][1] == districts[t - 1][1] + 1 && districts[i][0] > districts[t - 1][0]) {
                    districts[i][0] = districts[t - 1][0];
                    districts[i][1] = districts[t - 1][1] + 1;
                }
            }
        }
    } while (counter > 0);


//    print_districts_sort(districts);
}

void ds(int towns, int dtowns, const std::vector<std::list<std::pair<int, int>>> &graph, std::vector<std::vector<int>> &districts) {
    for(int i = 0; i < dtowns; i++)
        districts[i][0] = i + 1;

    std::list<int> izbjeglice;
    std::set<int> set;

    for (int i = 0; i < towns; i++) {
        for (const auto &tp: graph[i]) {
            int t = tp.first;

            //skip district towns
            if (t <= dtowns)
                continue;

            if (districts[t - 1][0] == 0 && districts[i][0] == 0) {
                izbjeglice.emplace_back(i + 1);
                izbjeglice.emplace_back(t);
                set.emplace(i + 1);
                set.emplace(t);
                continue;
            }

                //if town is not assigned a district assign it
            else if (districts[t - 1][0] == 0) {
                districts[t - 1][0] = districts[i][0];
                districts[t - 1][1] = districts[i][1] + 1;
            } else if (districts[i][0] == 0) {
                districts[i][0] = districts[t - 1][0];
                districts[i][1] = districts[t - 1][1] + 1;
            }
                //if a shorter distance could be assigned
            else if (districts[i][1] + 1 < districts[t - 1][1]) {
                districts[t - 1][0] = districts[i][0];
                districts[t - 1][1] = districts[i][1] + 1;
            }
            else if (districts[t - 1][1] + 1 < districts[i][1]) {
                districts[i][0] = districts[t - 1][0];
                districts[i][1] = districts[t - 1][1] + 1;
            }
                //if the distance is the same but id could be smaller
            else if (districts[i][1] + 1 == districts[t - 1][1] && districts[i][0] < districts[t - 1][0]) {
                districts[t - 1][0] = districts[i][0];
                districts[t - 1][1] = districts[i][1] + 1;
            }
            else if (districts[i][1] == districts[t - 1][1] + 1 && districts[i][0] > districts[t - 1][0]) {
                districts[i][0] = districts[t - 1][0];
                districts[i][1] = districts[t - 1][1] + 1;
            }
        }
    }
// rezultat 16657

//    while (!izbjeglice.empty()) {
//        for (auto it = izbjeglice.begin(); it != izbjeglice.end(); it++) {
//
//            if (districts[*it - 1][0] != 0) {
//                for (const auto &tp: graph[*it - 1]) {
//                    int t = tp.first;
//                    if (districts[t - 1][0] == 0) {
//                        districts[t - 1][0] = districts[*it - 1][0];
//                        districts[t - 1][1] = districts[*it - 1][1] + 1;
//                    }
//                    else if(districts[*it - 1][1] + 1 < districts[t - 1][1]) {
//                        districts[t - 1][0] = districts[*it - 1][0];
//                        districts[t - 1][1] = districts[*it - 1][1] + 1;
//                    }
//                    else if(districts[*it - 1][1] + 1 == districts[t - 1][1] && districts[*it - 1][0] < districts[t - 1][0]) {
//                        districts[t - 1][0] = districts[*it - 1][0];
//                        districts[t - 1][1] = districts[*it - 1][1] + 1;
//                    }
//                }
//                izbjeglice.erase(it++);
//                continue;
//            }
//
//            for (const auto &tp: graph[*it - 1]) {
//                int t = tp.first;
//
//                if (districts[t - 1][0] == 0) {
//                    //trebao bi vec biti u listi
//                    continue;
//                }
//                else {
//                    districts[*it - 1][0] = districts[t - 1][0];
//                    districts[*it - 1][1] = districts[t - 1][1] + 1;
//                    izbjeglice.erase(it++);
//                }
//            }
//        }
//    }

    while (!izbjeglice.empty()) {
        for (auto it = izbjeglice.begin(); it != izbjeglice.end(); it++) {

            if (districts[*it - 1][0] != 0) {
                for (const auto &tp: graph[*it - 1]) {
                    int t = tp.first;
                    if (districts[t - 1][0] == 0) {
                        districts[t - 1][0] = districts[*it - 1][0];
                        districts[t - 1][1] = districts[*it - 1][1] + 1;
                    }
                    else if(districts[*it - 1][1] + 1 < districts[t - 1][1]) {
                        districts[t - 1][0] = districts[*it - 1][0];
                        districts[t - 1][1] = districts[*it - 1][1] + 1;
                    }
                    else if(districts[*it - 1][1] + 1 == districts[t - 1][1] && districts[*it - 1][0] < districts[t - 1][0]) {
                        districts[t - 1][0] = districts[*it - 1][0];
                        districts[t - 1][1] = districts[*it - 1][1] + 1;
                    }
                }
                izbjeglice.erase(it++);
                continue;
            }

            bool erase = true;

            for (const auto &tp: graph[*it - 1]) {
                int t = tp.first;

                if (districts[t - 1][0] == 0) {
                    //trebao bi vec biti u listi
                    erase = false;
                }
                else if (districts[*it - 1][0] == 0) {
                    districts[*it - 1][0] = districts[t - 1][0];
                    districts[*it - 1][1] = districts[t - 1][1] + 1;
                    erase = true;
                }
                else if(districts[t - 1][1] + 1 < districts[*it - 1][1]) {
                    districts[*it - 1][0] = districts[t - 1][0];
                    districts[*it - 1][1] = districts[t - 1][1] + 1;
                }
                else if(districts[t - 1][1] + 1 == districts[*it - 1][1] && districts[t - 1][0] < districts[*it - 1][0]) {
                    districts[*it - 1][0] = districts[t - 1][0];
                    districts[*it - 1][1] = districts[t - 1][1] + 1;
                }
            }
            if(erase)
                izbjeglice.erase(it++);
        }
    }

//    while (!set.empty()) {
//        for (auto it = set.begin(); it != set.end();) {
//
//            if (districts[*it - 1][0] != 0) {
//                for (const auto &tp: graph[*it - 1]) {
//                    int t = tp.first;
//                    if (districts[t - 1][0] == 0) {
//                        districts[t - 1][0] = districts[*it - 1][0];
//                        districts[t - 1][1] = districts[*it - 1][1] + 1;
//                    }
//                    else if(districts[*it - 1][1] + 1 < districts[t - 1][1]) {
//                        districts[t - 1][0] = districts[*it - 1][0];
//                        districts[t - 1][1] = districts[*it - 1][1] + 1;
//                    }
//                    else if(districts[*it - 1][1] + 1 == districts[t - 1][1] && districts[*it - 1][0] + 1 < districts[t - 1][0]) {
//                        districts[t - 1][0] = districts[*it - 1][0];
//                        districts[t - 1][1] = districts[*it - 1][1] + 1;
//                    }
//                }
//                set.erase(it++);
//                continue;
//            }
//
//            bool erase = false;
//            for (const auto &tp: graph[*it - 1]) {
//                int t = tp.first;
//
//                if (districts[t - 1][0] == 0) {
//                    //trebao bi vec biti u listi
//
//                    continue;
//                }
//
//                else if(districts[*it - 1][0] == 0){
//                    districts[*it - 1][0] = districts[t - 1][0];
//                    districts[*it - 1][1] = districts[t - 1][1] + 1;
//                    erase = true;
//                }
//                else if(districts[t - 1][1] + 1 < districts[*it - 1][1]) {
//                    districts[*it - 1][0] = districts[t - 1][0];
//                    districts[*it - 1][1] = districts[t - 1][1] + 1;
//                }
//                else if(districts[t - 1][1] + 1 == districts[*it - 1][1] && districts[t - 1][0] < districts[*it - 1][0]) {
//                    districts[*it - 1][0] = districts[t - 1][0];
//                    districts[*it - 1][1] = districts[t - 1][1] + 1;
//                }
//            }
//            if(erase) {
//                set.erase(it++);
//            }
//
//            it++;
//
//        }
//    }

//    print_districts_sort(districts);
}



void x() {
    constexpr int MAX_COST = 250;
    int towns, dtowns, roads;

    std::vector<std::list<std::pair<int, int>>> graph(0);

    readData(towns, dtowns, roads, graph);

    //SORTING TOWNS ACCORDING TO DISTRICTS
    std::vector<std::vector<int>> districts (towns, std::vector<int>(2, 0));
    std::vector<std::vector<int>> districts2 (towns, std::vector<int>(2, 0));

 //   districtSort( towns,  dtowns, graph, districts);
 //   ds( towns,  dtowns, graph, districts);

    districtsCheck(districts);



    for(int i = 0; i < districts.size(); i++) {
        if(districts[i][0] != districts2[i][0]){
         //   printf("BELAJ!1 - %d d1 %d %d vs d2 %d %d \n\n", i, districts[i][0], districts[i][1], districts2[i][0], districts2[i][1]);
        }
//        if(districts[i][1] != districts2[i][1]) {
//            printf("BELAJ!2 - %d d1 %d %d vs d2 %d %d \n\n", i, districts[i][0], districts[i][1], districts2[i][0], districts2[i][1]);
//
//        }
    }

//    KRUSKAL'S ALGORTIHM
    std::vector<int> districtMstLengths(dtowns);

    std::unordered_map<std::string, std::vector<std::tuple<int, int, int>>> districtConnections;

    for(int k = 0; k < dtowns; k++) {
        //       printf("Dtown = %d \n", k + 1);

        std::vector<std::tuple<int, int, int>> districtRoads(0);
        std::set<int> repetition;

        for (int i = 0; i < towns; i++) {
            if (districts[i][0] != k + 1)
                continue;
            for (const std::pair<int, int> &tp: graph[i]) {
                int t = tp.first;

                if (districts[t - 1][0] != k + 1) {
                    districtConnections[std::to_string(k + 1) + std::to_string(districts[t - 1][0])].emplace_back(i + 1, t, tp.second);
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

//        for (auto el: districtConnection) { printf("%d %d %d \n", std::get<0>(el), std::get<1>(el), std::get<2>(el)); }
//        printf("\n\n\n");

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


//        print_mst(mst, districtMstLength);

    }
    int connectionsLength = 0;
    int noConnections = 0;
    for(int i = 0; i < dtowns; i++) {
        for(int j = i + 1; j < dtowns; j++) {
            if(noConnections >= dtowns - 1)
                break;
            std::vector<std::tuple<int, int, int>> connections = districtConnections[std::to_string(i + 1) + std::to_string(j + 1)];
            if(!connections.empty()) {
                int minConnection = std::get<2>(connections[0]);
                for(const auto & connection : connections) {
                    if( std::get<2>(connection) < minConnection) {
                        minConnection = std::get<2>(connection);
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
//    printf("Conn length: %d", connectionsLength);
    printf("%d", connectionsLength);
}

void districtsCheck(const std::vector<std::vector<int>>& districts) {
    for(auto red: districts) {
        if(red[0] == 0){
            printf("BELAJ!");
            break;
        }
    }
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
