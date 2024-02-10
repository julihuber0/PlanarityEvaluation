#include <bits/stdc++.h>
#include <ogdf/fileformats/GraphIO.h>

using namespace std;
using namespace ogdf;

int getGraphClass(const string &graphClass) {
    if (graphClass == "random_planar_connected") {
        return 1;
    }
    return -1;
}

int main(int argc, char *argv[]) {

    Graph G;
    GraphAttributes GA(G, GraphAttributes::nodeLabel);
    if (!GraphIO::read(GA, G, argv[1], GraphIO::readGML)) {
        cout << "Read failed" << endl;
        return 1;
    }
    string graphClass = GA.label(G.nodes.head());
    int n = G.numberOfNodes();

    string inFile(argv[1]);
    ofstream outFile(inFile.substr(0, inFile.length() - 4) + ".txt");

    if(!outFile.is_open()) {
        printf("Unable to open output file!\n");
        return 1;
    }

    outFile << "N=" << n << endl;

    for (node node: G.nodes) {
        outFile << node->index() << ": ";
        for (adjEntry a: node->adjEntries) {
            outFile << a->twinNode()->index() << " ";
        }
        outFile << "-1" << endl;
    }
    outFile << getGraphClass(graphClass) << endl;

    outFile.close();
}