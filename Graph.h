// Name: 

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>

using std::istream;

class Graph {


private:
    struct Node {
        int val;
        Node *next;
    };
    Node** list;
    int vertices;

    bool exists(int x, Node *p) const;

    void destroyList(Node *p);

    Node *copyLL(Node *p);

    bool cycleCheck(int x, bool visited[], int prev) const;

public:

    // build an empty graph with no vertices and no edges
    // the only way to add edges or vertices to the graph is with
    // is with the >> operator which deletes the current graph
    // and reads and builds a new one
    Graph();

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
    Graph(std::istream &in);

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
    // precondition: istream is an open stream
    friend std::istream &operator>>(std::istream &, Graph &);

    // output the graph
    friend std::ostream &operator<<(std::ostream &, const Graph &);

    // return true if the graph is connected, false otherwise
    // SOLUTION must be programmed with the STL std::stack
    bool isConnected() const;

    // return true if the graph has at least one cycle, false otherwise
    bool hasCycle() const;

    // returns the number of components in the graph
    void printComponents(std::ostream &) const;

    Graph &operator=(const Graph &);

    Graph(const Graph &);

    ~Graph();


};

#endif