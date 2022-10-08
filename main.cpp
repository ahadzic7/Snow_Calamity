#include <cstdio>
#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <stack>
#include <queue>

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

void print_districts(const std::vector<std::vector<int>>& districts, int dtowns) {
    std::vector<std::vector<int>> d(dtowns, std::vector<int>());
    for(int i = 0; i < districts.size(); i++) {
        d[districts[i][0] - 1].push_back(i + 1);
    }
    for(int i = 0; i < d.size(); i++) {
        printf("%d: ", i + 1);
        for(int j = 1; j < d[i].size(); j++) {
            printf("%d ", d[i][j]);
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

void bfsDistrictSort(int towns, int dtowns, const std::vector<std::list<std::pair<int, int>>> &graph, std::vector<std::vector<int>> &districts) {
    std::vector<std::queue<int>> townsToVisit(dtowns);

    for(int i = 0; i < dtowns; i++) {
        districts[i][0] = i + 1;
        townsToVisit[i].push(i + 1);
    }

    int visited = 0;
    std::vector<bool> visitedTowns (towns, false);

    while (visited < towns) {

        for(int i = 0; i < dtowns; i++) {

            int limit = townsToVisit[i].size();

            while (limit != 0) {

                int visitingTown = townsToVisit[i].front();
                townsToVisit[i].pop();

                if(!visitedTowns[visitingTown - 1]) {
                    visitedTowns[visitingTown - 1] = true;
                    visited++;
                    for(const auto &tp: graph[visitingTown - 1]) {
                        int t = tp.first;
                        if(districts[t - 1][0] == 0) {
                            townsToVisit[i].push(tp.first);
                        }
                    }
                }

                if(districts[visitingTown - 1][0] == 0) {
                    for(const auto &tp: graph[visitingTown - 1]){
                        int neighbour = tp.first;
                        if(districts[neighbour - 1][0] == 0)
                            continue;
                        else if(districts[neighbour - 1][0] != 0 && districts[visitingTown - 1][0] == 0) {
                            districts[visitingTown - 1][0] = districts[neighbour - 1][0];
                            districts[visitingTown - 1][1] = districts[neighbour - 1][1] + 1;
                        }
                        else if(districts[neighbour - 1][1] + 1 < districts[visitingTown - 1][1]) {
                            districts[visitingTown - 1][0] = districts[neighbour - 1][0];
                            districts[visitingTown - 1][1] = districts[neighbour - 1][1] + 1;
                        }
                        else if(districts[neighbour - 1][1] + 1 == districts[visitingTown - 1][1] && districts[neighbour - 1][0] < districts[visitingTown - 1][0]) {
                            districts[visitingTown - 1][0] = districts[neighbour - 1][0];
                            districts[visitingTown - 1][1] = districts[neighbour - 1][1] + 1;
                        }
                    }
                }

                limit--;
            }

        }

    }

//    print_districts_sort(districts);
//    print_districts(districts, dtowns);
}

void roadSort(int towns, int dtowns, const std::vector<std::list<std::pair<int, int>>> &graph,const std::vector<std::vector<int>> &districts,
              std::vector<std::vector<std::tuple<int, int, int>>> &listOfDistrictRoads) {

    constexpr int MAX_COST = 251;
    std::vector<std::vector<int>> districtGraph (dtowns, std::vector<int>(dtowns, MAX_COST));
    std::vector<std::tuple<int, int, int>> districtConnections(0);

    for(int k = 0; k < dtowns; k++) {

        std::vector<std::tuple<int, int, int>> districtRoads(0);

        for (int i = 0; i < towns; i++) {
            if (districts[i][0] == k + 1) {
                for (const auto &tp: graph[i]) {
                    int t = tp.first;

                    if(t <= dtowns) continue;
                    else if(districts[t - 1][0] != k + 1) {
                        if(tp.second < districtGraph[districts[t - 1][0] - 1][k]) {
                            continue;
                        }
                    }
                    else if (i < t) {
                        districtRoads.emplace_back(i + 1, t, tp.second);
                    }
                }
            }
        }

        std::sort(districtRoads.begin(), districtRoads.end(),[](const std::tuple<int, int, int> &x, const std::tuple<int, int, int> &y) {
                      return std::get<2>(x) < std::get<2>(y);
                  });

        listOfDistrictRoads.emplace_back(districtRoads);
    }

    for(int i = 0; i < towns; i++) {
        for(const auto &tp: graph[i]) {
            int dt1 = districts[tp.first - 1][0] - 1;
            int dt2 = districts[i][0] - 1;

            if(dt1 != dt2) {
                if(tp.second < districtGraph[dt1][dt2])
                    districtGraph[dt1][dt2] = tp.second;
            }
        }
    }

    for(int i = 0; i < dtowns; i++) {
        for(int j = 0; j < i; j++) {
            if(districtGraph[i][j] != MAX_COST)
                districtConnections.emplace_back(i + 1, j + 1, districtGraph[i][j]);
        }
    }

    std::sort(districtConnections.begin(), districtConnections.end(),[](const std::tuple<int, int, int> &x, const std::tuple<int, int, int> &y) {
        return std::get<2>(x) < std::get<2>(y);
    });

    listOfDistrictRoads.emplace_back(districtConnections);
}

int mstFind(int town, std::vector<int> &boss) {
    return (boss[town] == town ? town : (boss[town] = mstFind(boss[town], boss)));
}

void kruskal(int towns, int dtowns, const std::vector<std::vector<std::tuple<int, int, int>>> &listOfDistrictRoads) {
    std::vector<int> boss(towns);
    std::vector<int> rank(towns, 0);

    for(int i = 0; i < towns; i++)
        boss[i] = i;

    std::vector<int> districtMst(dtowns + 1, 0);

    for(int i = 0; i <= dtowns; i++) {
        for(const auto &road: listOfDistrictRoads[i]) {

            int x = std::get<0>(road) - 1;
            int y = std::get<1>(road) - 1;

            int a = mstFind(x, boss);
            int b = mstFind(y, boss);

            if(a != b)
                districtMst[i] += std::get<2>(road);

            if(rank[b] > rank[a])
                boss[a] = b;
            else {
                boss[b] = a;
                if(rank[b] == rank[a])
                    rank[a]++;
            }

        }
    }
    int sum = 0;
    for(int i = 0; i <= dtowns; i++) {
        sum += districtMst[i];
//        printf("%d: mst = %d\n", i, districtMst[i]);
    }
//    printf("\n\n%d", sum);
    printf("%d", sum);
}

void x() {
    int towns, dtowns, roads;

//    clock_t a, b,c, d, e;
//    a = clock();

    std::vector<std::list<std::pair<int, int>>> graph(0);

    readData(towns, dtowns, roads, graph);
//    b = clock();
//    printf("Read data %f\n\n", ((b - a) / 1000.));

    //SORTING TOWNS ACCORDING TO DISTRICTS
    std::vector<std::vector<int>> districts (towns, std::vector<int>(2, 0));
    bfsDistrictSort(towns, dtowns, graph, districts);
//    c= clock();
//    printf("Sorting %f \n\n",((c - b) / 1000.));

    //PREPARATION FOR KRUSKAL
    std::vector<std::vector<std::tuple<int, int, int>>> listOfDistrictRoads;
    roadSort(towns, dtowns, graph, districts, listOfDistrictRoads);
//    d=clock();
//    printf("Kruskal preparation %f, \n\n", ((d - c) / 1000.));

    //KRUSKAL
    kruskal(towns, dtowns, listOfDistrictRoads);
//    e=clock();
//    printf("\n\nKruskal %f, \n\n", ((e - d) / 1000.));
}

int main() {
//    clock_t start, end;
//    start = clock();

    x();
//    end = clock();
//    printf("\nVrijeme izvrsavanja: %f", double ((end - start) / 1000.));
    return 0;
}
