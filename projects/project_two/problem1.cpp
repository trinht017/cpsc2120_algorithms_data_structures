#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Team{
    string name;
    int off;
    int def;
};

int main() {
    vector<Team> teams;

    string name;
    int off, def;
    Team temp;

    while(cin >> name >> off >> def) {
        temp.name = name;
        temp.off = off;
        temp.def = def;
        teams.push_back(temp);
    }

    sort(teams.begin(), teams.end(), [](Team &a, Team &b) 
                                     {return a.off > b.off;});

    vector<Team> notInTrouble;
    notInTrouble.push_back(teams[0]);
    int maxVal = teams[0].def;
    int firstE = teams[0].def;
    int currentE;

    for (int i = 1; i < (int)teams.size(); i++) {
        currentE = teams[i].def;
        if (currentE >= firstE && currentE >= maxVal) {
            maxVal = currentE;
            notInTrouble.push_back(teams[i]);
        }
    }

    sort(notInTrouble.begin(), notInTrouble.end(), [](Team &a, Team &b) 
                                                   {return a.name < b.name;});
    
    for (Team &e : notInTrouble) {
        cout << " " << e.name << " " << e.off << " " << e.def << endl; 
    } 

    return 0;
}