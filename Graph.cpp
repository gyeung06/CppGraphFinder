// Name: Gabriel Yeung

#include <iostream>
#include <limits> // for declaration of 'numeric_limits' for in
#include <ios>    // for declaration of 'streamsize' for in
#include <queue>
#include <stack>
#include "Graph.h"

// build an empty graph with no vertices and no edges
// the only way to add edges or vertices to the graph is with
// is with the >> operator which deletes the current graph
// and reads and builds a new one
Graph::Graph() {
    list = nullptr;
    vertices = 0;
}

// builds a graph from the data read from input stream in
// read the number of vertices n
// read pairs of integers corresponding to an edge in the graph
// read until the input stream in fails (i.e. no more data or
// the data is not an integer)
// resets the istream in and clears the buffer if the stream fails
// Note: the previous graph is destroyed first, before building a new one
// if any of the vertices of the edges (i, j) are not
// 0 <= i < n and that 0 <= j < n, these 2 endpoints are ignored
// also, every undirected edge may only be read ONCE
// if an undirected edge is entered twice, it is ignored
// precondition: istream is an open stream
Graph::Graph(std::istream &in) {
    for (int i = 0; i < vertices; i++) {
        Node *temp = list[i];
        destroyList(temp);
    }
    if (vertices > 0)
        delete[] list;
    int length, v1, v2;
    in >> length;
    list = new Node *[length];
    vertices = length;
    for (int i = 0; i < length; i++) {
        list[i] = new Node{i, nullptr};
    }

    while (in >> v1) {
        if (in >> v2) {
            if (!exists(v2, list[v1]) && v2 < vertices && v1 < vertices) {
                list[v1]->next = new Node{v2, list[v1]->next};
                if (!exists(v1, list[v2])) {
                    list[v2]->next = new Node{v1, list[v2]->next};
                }
            }
        } else {
            std::cerr << "ERROR Graph Constructor: odd numbers of input";
        }
    }
}


// read the number of vertices n
// read pairs of integers corresponding to an edge in the graph
// read until the input stream in fails
// which subsequently resets the istream in and clears the buffer
// (integers are the end vertices) of the edges
// Note: the previous graph is destroyed first
// if any of the vertices of the edges (i, j) are not
// 0 <= i < n and that 0 <= j < n, these 2 end points are ignored
// also, every undirected edge may only be read ONCE
// if an undirected edge is entered twice, it is ignored
// precondition: istream is an open streamstd::istream &operator>>(std::istream &in, Graph &graph) {
    for (int i = 0; i < graph.vertices; i++) {
        graph.destroyList(graph.list[i]);
    }
    if (graph.vertices > 0)
        delete[] graph.list;
    int length, v1, v2;
    in >> length;
    graph.list = new Graph::Node *[length];
    graph.vertices = length;
    for (int i = 0; i < length; i++) {
        graph.list[i] = new Graph::Node{i, nullptr};
    }

    while (in >> v1) {
        if (in >> v2) {
            if (!graph.exists(v2, graph.list[v1]) && v2 < graph.vertices && v1 < graph.vertices) {
                graph.list[v1]->next = new Graph::Node{v2, graph.list[v1]->next};
                if (!graph.exists(v1, graph.list[v2])) {
                    graph.list[v2]->next = new Graph::Node{v1, graph.list[v2]->next};
                }
            }
        } else {
            std::cerr << "ERROR Graph Constructor: odd numbers of input";
        }
    }
    return in;
}

// output the graph
std::ostream &operator<<(std::ostream &out, const Graph &graph) {
    for (int i = 0; i < graph.vertices; i++){
        Graph::Node* temp = graph.list[i]->next;
        out << i << " ";
        while (temp != nullptr){
            out << temp->val << " ";
            temp = temp->next;
        }
        out << std::endl;
    }
    return out;
}

// postcondition:
//    deep copy of the rtSide was made
Graph &Graph::operator=(const Graph &rtSide) {
    for (int i = 0; i < vertices; i++) {
        destroyList(list[i]);
    }
    if (vertices > 0)
        delete[] list;

    vertices = rtSide.vertices;
    list = new Node *[vertices];
    for (int i = 0; i < vertices; i++) {
        list[i] = copyLL(rtSide.list[i]);
    }
    return *this;
}


Graph::Graph(const Graph &other) {
    vertices = other.vertices;
    list = new Node *[vertices];
    for (int i = 0; i < vertices; i++) {
        list[i] = copyLL(other.list[i]);
    }
}

// destructor frees up the memory
Graph::~Graph() {
    for (int i = 0; i < vertices; i++) {
        destroyList(list[i]);
    }
    delete[] list;

}

// return true if the graph is connected, false otherwise
// SOLUTION must be programmed with the STL std::stack
bool Graph::isConnected() const {
    if (vertices < 3) {
        return true;
    }
    bool *visited = new bool[vertices];
    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
    }
    std::stack<int> vet;
    vet.push(0);
    while (!vet.empty()) {
        Node *temp = list[vet.top()]->next;
        visited[vet.top()] = true;
        vet.pop();
        while (temp != nullptr) {
            if (!visited[temp->val]) {
                vet.push(temp->val);
            }
            temp = temp->next;
        }
    }
    for (int i = 0; i < vertices; i++) {
        if (!visited[i]) {
            return false;
        }
    }
    return true;
}

// return true if the graph has at least one cycle, false otherwise
bool Graph::hasCycle() const {
    bool *visited = new bool[vertices];
    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
    }
    for (int i = 0; i < vertices; i++) {
        if (!visited[i]) {
            if (cycleCheck(i, visited, -1)) {
                return true;
            }
        }
    }
    return false;
}

// returns the number of components in the graph
void Graph::printComponents(std::ostream &out) const {
    int *visited = new int[vertices];
    for (int i = 0; i < vertices; i++) {
        visited[i] = -1;
    }
    int count = 0;
    std::stack<int> vet;
    for (int i = 0; i < vertices; i++, count++) {
        if (visited[i] == -1) {
            vet.push(i);
        }
        while (!vet.empty()) {
            Node *temp = list[vet.top()]->next;
            visited[vet.top()] = count;
            vet.pop();
            while (temp != nullptr) {
                if (visited[temp->val] == -1) {
                    vet.push(temp->val);
                }
                temp = temp->next;
            }
        }
    }
    bool endl;
    for (int it = -1; it < vertices; it++) {
        endl = false;
        for (int j = 0; j < vertices; j++) {
            if (visited[j] == it) {
                out << j << " ";
                endl = true;
            }
        }
        if (endl) {
            out << std::endl;
        }
    }
}

//returns if x exists in LL p
bool Graph::exists(int x, Graph::Node *p) const {
    if (p == nullptr) {
        return false;
    }
    if (p->val == x) {
        return true;
    }
    return exists(x, p->next);
}

//destroys every node after p
void Graph::destroyList(Graph::Node *p) {
    if (p == nullptr) {
        return;
    }
    Node *temp = p->next;
    delete p;
    destroyList(temp);
}

//returns deepcopied nodes from the given node
Graph::Node *Graph::copyLL(Graph::Node *p) {
    if (p == nullptr) {
        return p;
    }
    return new Node{p->val, copyLL(p->next)};
}

//hasCycle helper function
bool Graph::cycleCheck(int x, bool visited[], int prev) const {
    visited[x] = true;
    Node *temp = list[x]->next;

    while (temp != nullptr) {
        if (!visited[temp->val]) {
            if (cycleCheck(temp->val, visited, x)) {
                return true;
            }
        } else if (temp->val != prev) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
