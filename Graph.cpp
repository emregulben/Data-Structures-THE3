#include "Graph.h"

/*Do not add new libraries or files*/


Graph::Graph() {
    // TODO: IMPLEMENT THIS FUNCTION.
    HashTable<int, list<Edge>> h;
    adjList = h;
}

Graph::Graph(const Graph& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    adjList = rhs.adjList;
}

Graph& Graph::operator=(const Graph& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    if (this != &rhs) {
        adjList = rhs.adjList;
    }
    return *this;
}

Graph::~Graph() {
    // TODO: IMPLEMENT THIS FUNCTION.
    
}


void Graph::addNode(const Node &node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    list<Edge> emptylist;
    adjList.Insert(node.getVid(), emptylist);
}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {
    // TODO: IMPLEMENT THIS FUNCTION.
    Edge newEdge;
    newEdge.setTailNode(tailNode);
    newEdge.setImport((long) import);
    list<Edge> &lst = adjList.Get(headNode.getVid());
    lst.push_back(newEdge);
}

list<Node> Graph::getAdjacentNodes(const Node& node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    list<Edge> lst = adjList.Get(node.getVid());
    list<Node> res;
    res.resize(lst.size());
    if (lst.empty()) {
        return res;
    }
    list<Edge>::iterator it1;
    list<Node>::iterator it2;
    for(it1 = lst.begin(), it2 = res.begin(); it1 != lst.end(); ++it1, ++it2) {
        *it2 = it1->getTailNode();
    }
    return res;
}

long Graph::getTotalImports(const Node& node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    list<Edge> lst = adjList.Get(node.getVid());
    list<Edge>::iterator it;
    long total = 0;
    for (it = lst.begin(); it != lst.end(); ++it) {
        total += it->getImport();
    }
    return total;
}

void Graph::deleteNode(const Node& node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    //int arr[adjList.Size()];
    int *arr = new int[adjList.Size()];
    adjList.getKeys(arr);
    for (int i = 0; i<adjList.Size(); i++) {
        list<Edge> &lst = adjList.Get(arr[i]);
        for (list<Edge>::iterator it = lst.begin(); it != lst.end(); ++it) {
            if (it->getTailNode().getVid() == node.getVid()) {
                lst.erase(it);
                break;
            }
        }
    }
    list<Edge> &lst2 = adjList.Get(node.getVid());
    lst2.clear();
    adjList.Delete(node.getVid());
    delete[] arr;
}

list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
    // TODO: IMPLEMENT THIS FUNCTION.
    priority_queue<Vertex, vector<Vertex>, CustomCompare> pq;
   // int arr[adjList.Size()];
    int *arr = new int[adjList.Size()];
    adjList.getKeys(arr);
    HashTable<int, Vertex> note;

    for (int i = 0; i<adjList.Size(); i++) {
        list<string> path;
        Vertex k(arr[i], path, numeric_limits<long>::max());
        note.Insert(arr[i], k);
    }
    
    list<string> path;
    path.push_back(srcNode.getCountry());
    Vertex src(srcNode.getVid(), path, 0);
    pq.push(src);
    
    while (!pq.empty()) {
        Vertex u = pq.top();
        pq.pop();
        list<Edge> lst = adjList.Get(u.id);
        for (list<Edge>::iterator it = lst.begin(); it != lst.end(); ++it) {
            int key = it->getTailNode().getVid();
            Vertex &v = note.Get(key);
            if (u.distance + it->getImport() < v.distance) {
                v.distance = u.distance + it->getImport();
                list<string> newpath = u.path;
                newpath.push_back(it->getTailNode().getCountry());
                v.path = newpath;
                pq.push(v);
            }
        }
    }
    
    Vertex res = note.Get(destNode.getVid());
    delete[] arr;
    return res.path;
}


bool Graph::isCyclic() {
    // TODO: IMPLEMENT THIS FUNCTION.
    //int arr[adjList.Size()];
    int *arr = new int[adjList.Size()];
    adjList.getKeys(arr);
    HashTable<int, Pair> record;
    for (int i = 0; i<adjList.Size(); ++i) {
        Pair p;
        record.Insert(arr[i], p);
    }
    
    
    for (int i = 0; i<adjList.Size(); ++i) {
        if (isCyclicHelp(arr[i], record)) {
            delete[] arr;
            return true;
        }
    }
    delete[] arr;
    return false;
}


list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {
    // TODO: IMPLEMENT THIS FUNCTION.
    list<string> res;
    queue<Node> q;
    q.push(srcNode);
    list<string> visited;
    visited.push_back(srcNode.getCountry());
    while (!q.empty()) {
        Node v = q.front();
        res.push_back(v.getCountry());
        q.pop();
        if (destNode.getVid() == v.getVid()) {
            return res;
        }
        
        list<Edge> lst = adjList.Get(v.getVid());
        for (list<Edge>::iterator it = lst.begin(); it != lst.end(); ++it) {
            if (!marked(visited, it->getTailNode().getCountry())) {
                q.push(it->getTailNode());
                visited.push_back(it->getTailNode().getCountry());
            }
        }
    }
    res.clear();
    return res;
}

bool Graph::marked(list<string> lst, string country) {
    for (list<string>::iterator it = lst.begin(); it != lst.end(); ++it) {
        if (country == *it) {
            return true;
        }
    }
    return false;
}

bool Graph::isCyclicHelp(int key, HashTable<int, Pair> record) {
    Pair& mypair = record.Get(key);
    if (mypair.visited == false) {
        mypair.visited = true;
        mypair.recurred = true;
        
        list<Edge> start = adjList.Get(key);
        for (list<Edge>::iterator it = start.begin(); it != start.end(); ++it) {
            int currentKey = it->getTailNode().getVid();
            Pair currentPair = record.Get(currentKey);
            
            if (currentPair.visited == false && isCyclicHelp(currentKey, record)) {
                return true;
            }
            
            else if (currentPair.recurred == true) {
                return true;
            }
        }
    }
    mypair.recurred = false;
    return false;
}

