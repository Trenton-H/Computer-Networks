#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct routingTable
{
    int dest,
        cost,
        nextHop;
};
struct neighbor
{
    int neighbor;
    int cost;
    double propDelay;
};

struct pairs
{
    int dest;
    int cost;
};

struct dvPacket
{
    int src,
        dest;
    pairs packet[100];
};

/*struct Event
{
    double time;
    String type;       //type of event:periodic update, triggered update, link failure, data packet, etc
    int firstNode,     //first node involved
        secondNode;    //second node involved
    dvPacket * packet; //pointer to the dv packet that is the event
    next event         //pointer to the next event
}; */

class Router
{
    private:
        int id;
        routingTable RT[50];
        neighbor neighbors[50];
    public:
        Router();
        void setID(int);
        int getID();
        void addRT(int dest, int cost, int nextHop);
        void displayRT();
        void addNeighbors(int n, int c, double p);
        void displayNeighbors();
        void initializeRT();
        void periodicUpdate();
        void receivePacket(dvPacket p);
};

void displayRoutingTable(Router nodes[]);

int main()
{
    string line;
    int firstNode,
        cost,
        secondNode;
    double propDelay;
    ifstream myFile;

    Router nodes[100];

    try
    {
        myFile.open("topology1.txt");
        if(!myFile.good())
            throw 66;
    }catch(int e)
    {
        cout << "Unable to open the file" << endl;
    }

    if(myFile.is_open())
    {
        while(myFile >> firstNode >> secondNode >> cost >> propDelay)
        {
            //cout << firstNode << "\t" << secondNode << "\t" << cost << "\t" << propDelay << endl;
            nodes[firstNode].addNeighbors(secondNode, cost, propDelay);
            nodes[secondNode].addNeighbors(firstNode, cost, propDelay);
            if(nodes[firstNode].getID() == -1)
                nodes[firstNode].setID(firstNode);
            if(nodes[secondNode].getID() == -1)
                nodes[secondNode].setID(secondNode);
        }
        myFile.close();
    }
    else
        cout << "Couldn't open the file" << endl;

    for(int i = 0; i < 100; i++)    //this will display the neighbors from the neighbor list
    {
        if(nodes[i].getID()>= 0)
           nodes[i].displayNeighbors();
    }

    for(int i = 0; i < 100; i++) //initialize RT for each node
    {
        if(nodes[i].getID() >= 0)
            nodes[i].initializeRT();
    }

    displayRoutingTable(nodes);

    cout << "\nsending a periodic update \n\n\n";
    for(int i = 0; i < 100; i++) //initialize RT for each node
    {
        if(nodes[i].getID() >= 0)
            nodes[i].periodicUpdate();
    }
    displayRoutingTable(nodes);

    return 0;
}

Router :: Router()
{
    id = -1;
}

void Router :: setID(int i)
{
    if(id == -1)
        id = i;
}

int Router :: getID()
{
    return id;
}
void Router :: addRT(int dest, int cost, int nextHop)
{
    if(!(RT[dest].dest))
    {
        RT[dest].dest = dest;
        RT[dest].cost = cost;
        RT[dest].nextHop = nextHop;
    }else
    {
        if(RT[dest].cost > cost)
        {
            RT[dest].dest = dest;
            RT[dest].cost = cost;
            RT[dest].nextHop = nextHop;
        }
    }
}
void Router :: displayRT()
{
    cout << "Routing Table for " << getID() << ":" << endl << endl;
    for(int i = 0; i<51; i++)
    {
        if(RT[i].cost > 0)
            cout << RT[i].dest << ", " << RT[i].cost << ", " << RT[i].nextHop << endl;
    }

}
void Router :: addNeighbors(int n, int c, double p)
{
    if(!(neighbors[n].neighbor))
    {
        neighbors[n].neighbor = n;
        neighbors[n].cost = c;
        neighbors[n].propDelay = p;
    }

}
void Router :: displayNeighbors()
{
    cout << "Router " << id << " neighbors:" << endl << endl;
    for(int i = 0; i < 51; i++)
    {
        if(neighbors[i].cost > 0)
            cout << neighbors[i].neighbor << ", " << neighbors[i].cost << ", "
                 << neighbors[i].propDelay << endl;
    }
    cout << endl;
}
void Router :: initializeRT()
{
    for(int j = 0; j < 51; j++)
    {
        if(neighbors[j].cost > 0)
        {
            addRT(neighbors[j].neighbor, neighbors[j].cost, neighbors[j].neighbor);
        }

    }
}

void Router :: periodicUpdate()
{
    dvPacket pack;
    pack.src = id;

    for(int j = 0; j < 100; j++)
    {
        if(RT[j].dest >= 0)
        {
            pack.packet[j].cost = RT[j].cost;
            pack.packet[j].dest = RT[j].dest;
        }
    }
    for(int i = 0; i < 100; i++)
    {
        if(neighbors[i].neighbor >= 0)
            pack.dest = neighbors[i].neighbor;
        receivePacket(pack);
    }
}

void Router :: receivePacket(dvPacket p)
{
    for(int i = 0; i < 100; i++)
    {
        if(!(RT[i].cost > 0))
           addRT(i, p.packet[i].cost, p.src);
        else if(RT[i].cost > p.packet[i].cost)
        {
            RT[i].cost = p.packet[i].cost;
            RT[i].nextHop = p.src;
        }
    }
}


void displayRoutingTable(Router nodes[])
{
    for(int i = 0; i < 100; i++) //display RT
    {
        if(nodes[i].getID() >= 0)
        {
            nodes[i].displayRT();
        }
    }
}


















