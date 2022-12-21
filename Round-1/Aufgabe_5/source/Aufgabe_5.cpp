#include <bits/stdc++.h>

#define ve vector
#define vi ve<int>
#define vvi ve<vi>
#define vvvi ve<vvi>
#define everyN(var, start) for(int var = start; var < n; var++)
#define everyM(var, start) for(int var = start; var < m; var++)

using namespace std;
using namespace chrono;
const int maxNodes = 151;
const int maxDist = 1000;

ifstream inputFile;
ofstream outputFile, outputFileGraph, outputFileGraphEnd;
vvi G(maxNodes);
int n, m;

/*
This function prints a path to cout and the ouput file
*/
void printPath(vi path, string name){
    cout << name << ": "; // print paht name
    outputFile << name << ": ";
    for(int x : path){ // print each node in path
        cout << " -> " << setw(3) << x << " ";
        outputFile << " -> " << setw(3) << x << " ";
    }
    cout << endl;
    outputFile << endl;
}

/*
This functions prints the result paths
*/
void printResults(vi path_1, vi path_2, int i){
    printPath(path_1, "path 1"); // print path 1
    printPath(path_2, "path 2"); // print path 2
    cout << endl << "Lenght of paths: " << i << endl; // print length of path to cout
    outputFile << endl << "Lenght of paths: " << i << endl; // print length of path to output file
}

/*
This function creates the Graph End file, which represents the graph with colored paths.
*/
void writeContentToGraphEndFile(vi path_1, vi path_2, int i){
    everyN(x,0){ // go through every edge
        for(int y : G[x]){
            int a = 0, b = 0;
            for(int k = 0; k < i; k++){ // check if edge has to be colored
                if(path_1[k] == x && path_1[k+1] == y){
                    a++;
                }
                if(path_2[k] == x && path_2[k+1] == y){
                    b++;
                }
            }
            if(a == 0 && b == 0){
                outputFileGraphEnd << x << "->" << y << endl; // write edge to output
            } else {
                if(a != 0){
                    outputFileGraphEnd << x << "->" << y << " [color = red; penwidth=4; label=\" " << a << ((a == 1) ? " time\"]" : " times\"]") << endl;  // color edges on path 1 red
                }
                if(b != 0){
                    outputFileGraphEnd << x << "->" << y << " [color = blue; penwidth=4 label=\" " << b << ((b == 1) ? " time\"]" : " times\"]") << endl; // color edges on path 2 blue
                }
            }
        }
    }
    outputFileGraphEnd << "}" << endl; // print end of graph file with colored edges
}

/*
This function is used recursive and performs a Depht-First-Search to find all paths
*/
void dfs(int node, int dist, vi &path, vvvi &paths){
    if(paths[node][dist].size() == 0 && dist < maxDist){ // check if edge hasn't been visited with same distance and path isn't too long
        if(dist > 0){ // only write path if distance is greater than 0
            paths[node][dist] = path; // save path
            paths[node][dist].push_back(node); // push back end node
        }
        path.push_back(node); // push back next node to path
        for(int newNode : G[node]){
            dfs(newNode, dist+1, path, paths); // perform next dfs for all neighbors
        }
        path.pop_back(); // remove last element from path
    }
}

/*
This function tries to find two paths with the same length to the same destination, starting from node 0 and 1
*/

void getPath(vvvi &paths_1, vvvi &paths_2){
    for(int i = 1; i < maxDist; i++){  // check distances from 0 to max distance
        everyN(j,2){ // check for each node
            if(paths_1[j][i].size() != 0 && paths_2[j][i].size() != 0){ // check if there are 2 paths with same lengths 
                printResults(paths_1[j][i], paths_2[j][i], i); // print out results
                writeContentToGraphEndFile(paths_1[j][i], paths_2[j][i], i); // write to Graph file with colored edges
                return;
            }
        }
    }
    // there is no path
    everyN(x,0){ // write original graph to Graph file with colored edges
        for(int y : G[x]){
            outputFileGraphEnd << x << "->" << y << endl;
        }
    }
    outputFileGraphEnd << "}" << endl;
    cout << endl << "there is no solution!" << endl; // write to output
    outputFile << endl << "there is no solution!" << endl; // write to output file
}

/*
This function is called to solve the problem
*/
void solve(){
    vvvi paths1(n, vvi(maxDist)); // create vector for path saving
    vvvi paths2(n, vvi(maxDist));
    vi path1; // create vector for path
    vi path2;
    dfs(0, 0, path1, paths1); // start dfs from start point 1
    dfs(1, 0, path2, paths2); // start dfs from start point 2
    getPath(paths1, paths2); // calculate path from results of dfs
}

/*
This function gets the filename of the input File
*/
string getFilename(){
    string filename; 
    cout << "Please enter filename without file extension" << endl; // print message to user
    cout << "Files must be located in '/beispieleingaben/'" << endl;
    cout << "-> ";
    cin >> filename; // get input from user
    return filename;
}

/*
    This function is the main Function and is called at the execution start
    It reads the input file and starts the solve() methode
*/
int main(){
    string filename = getFilename(); // get filename for in-/output
    inputFile.open("../beispieleingaben/huepfburg"+filename+".txt"); // open input file
    if(inputFile.is_open()){
        outputFile.open("../beispielausgaben/huepfburg"+filename+".out"); // open output file
        outputFileGraph.open("../beispielausgaben/huepfburg"+filename+".dot"); // open output file for graph
        outputFileGraphEnd.open("../beispielausgaben/huepfburg"+filename+"_end.dot"); // open output file for graph with colored paths
        outputFileGraph << "digraph{" << endl << "rankdir=LR" << endl; // write head into graph file
        outputFileGraphEnd << "digraph{" << endl << "rankdir=LR" << endl; // write head into graph file with colored paths
        inputFile >> n >> m; // get number of edges and nodes
        for(int i = 0; i < n; i++){ // write nodes to graph files
            outputFileGraph << i;
            outputFileGraphEnd << i;
            if(i == 0 || i == 1){ // set color of start nodes to green
                outputFileGraph << " [fillcolor = green; style=filled]";
                outputFileGraphEnd << " [fillcolor = green; style=filled]"; 
            }
            outputFileGraph << endl;
            outputFileGraphEnd << endl;
        }
        everyM(i,0){ // read edges from input file
            int n1, n2;
            inputFile >> n1 >> n2;
            n1--; n2--;
            G[n1].push_back(n2); // write to adjacency list
            outputFileGraph << n1 << "->" << n2 << endl; // write edges to graph file
        }
        outputFileGraph << "}" << endl; // print end of graph file
        inputFile.close(); // close input file
    } else {
        cout << "Unable to open file";
    }

    auto t1 = high_resolution_clock::now();
    solve(); // call function to solve problem
    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << "Calculated Solution in " << ms_int.count() << " ms" << endl;
    outputFile << "Calculated Solution in " << ms_int.count() << " ms" << endl;
    system(("dot -Tpng -o ../beispielausgaben/huepfburg"+filename+".png ../beispielausgaben/huepfburg"+filename+".dot").c_str()); // perform dot command for graph file
    system(("dot -Tpng -o ../beispielausgaben/huepfburg"+filename+"_end.png ../beispielausgaben/huepfburg"+filename+"_end.dot").c_str()); // perform dot command for graph file with colored edges
    outputFile.close();
    outputFileGraph.close();
    outputFileGraphEnd.close();
    return 0;
}