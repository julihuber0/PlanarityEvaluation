#include <bits/stdc++.h>

using namespace std;

void removeTrailingWhitespace(string &str) {
    size_t firstNotWhitespace = str.find_first_not_of(" \t\n\r");
    if(firstNotWhitespace != string::npos) {
        str = str.substr(firstNotWhitespace);
    } else {
        str.clear();
    }
}

int main(int argc, char *argv[]) {
    ifstream file(argv[1]);

    if(!file.is_open()) {
        printf("Unable to open file!\n");
        return 1;
    }

    vector<string> lines;
    string line;
    while(getline(file, line)) {
        removeTrailingWhitespace(line);
        lines.push_back(line);
    }

    file.close();

    int nodeCount = 0;
    int edgeStart = 0;

    int i = 0;

    while (i < lines.size()) {
        if(lines[i] == "node") {
            ++nodeCount;
        }
        else if (lines[i] == "edge") {
            edgeStart = i;
            break;
        }
        ++i;
    }

    vector<vector<int>> list(nodeCount);

    int j = edgeStart;
    while (lines[j] == "edge") {
        int src, trgt;
        j += 2;
        int sourceLength = lines[j].size() - 7;
        string source = lines[j].substr(7, sourceLength);
        istringstream iss(source);
        iss >> src;
        ++j;
        int targetLength = lines[j].size() - 7;
        string target = lines[j].substr(7, targetLength);
        istringstream iss2(target);
        iss2 >> trgt;
        list[src].push_back(trgt);
        j += 2;
    }

    string inFile(argv[1]);
    ofstream outFile(inFile.substr(0, inFile.length() - 4) + ".txt");

    if(!outFile.is_open()) {
        printf("Unable to open output file!\n");
        return 1;
    }

    outFile << "N=" << nodeCount << endl;

    for (int k = 0; k < list.size(); ++k) {
        outFile << k << ": ";
        for(int m = 0; m < list[k].size(); ++m) {
            outFile << list[k][m] << " ";
        }
        outFile << "-1" << endl;
    }

    outFile.close();
}