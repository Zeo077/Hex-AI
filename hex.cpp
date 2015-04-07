#ifndef __HEX_CPP_INCLUDE__//include guard
#define __HEX_CPP_INCLUDE__

#include "hex.h"
//#include "minmax.cpp"
#include "montecarlo.cpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <set>

//hex
//accessors
const std::pair<int,int> hex::getID(){return this->id;}

const std::vector<hex*> hex::getNeighbors(){return this->neighbors;}
const player hex::getPlayer(){return this->p;};
//mutators        
void hex::setPlayer(player pn){this->p = pn;}
void hex::addNeighbor(hex* n){this->neighbors.push_back(n);}      
void hex::clearNeighbors(){this->neighbors.clear();}
//constructors
hex::hex(std::pair<int,int> idn):id(idn),p(player::NONE){}
hex::hex(int x, int y):p(player::NONE)
{
    this->id = std::make_pair(x, y);
}
//destructor
hex::~hex(){}
//operator overload        
std::ostream& operator<<(std::ostream& out, const hex& h)
{
    int o;
    switch(h.p)
    {
        case player::ONE:
            o = 1;
            break;
        case player::TWO:
            o = 2;
            break;
        case player::OTHER:
            o = -1;
            break;
        default:
            o = 0;
    }
    out << o;
    return out;
}

//HexGraph
//HexGraph utility coordinate conversions
std::pair<int,int> HexGraph::nodeToCoor(int node)
{
    int y = node % this->size;
    int x = node - this->size * y;
    return std::make_pair(x-1,y);
    //node {0,120}, coor {(0,0),(0,10),(10,0),(10,10)}
}
int HexGraph::coorToNode(int x, int y)
{
    return x + y*this->size;
}
int HexGraph::coorToNode(std::pair<int,int> c)
{
    return coorToNode(c.first,c.second);
}
std::vector<std::pair<int,int>> HexGraph::calculate_neighbors(std::pair<int,int> c)
{
    std::vector<std::pair<int,int>> ret = calculate_neighbors(c.first,c.second);
    return ret;//calculate_neighbors(c.first,c.second);
}
std::vector<std::pair<int,int>> HexGraph::calculate_neighbors(int x, int y)
{
    std::pair<int,int> h1, h2, h3, h4, h5, h6;
    h1 = std::make_pair(x-1,y-1);
    h2 = std::make_pair(x+1,y-1);
    h3 = std::make_pair(x-1,y);
    h4 = std::make_pair(x+1,y);
    h5 = std::make_pair(x-1,y+1);
    h6 = std::make_pair(x+1,y+1);
    std::pair<int,int> hs[] = {h1,h2,h3,h4,h5,h6};
    std::vector<std::pair<int,int>> ret;
    for(auto h : hs)
    {
        if(h.first < 0 || h.first >= this->size || h.second < 0 || h.second >= this->size)
            continue;
        ret.push_back(h);
    }
    return ret;
}
//private accessor for matrix
hex& HexGraph::access(int x, int y)
{
    int index = coorToNode(x,y);
    return this->grid.at(index);
}
void HexGraph::read_fringe(hex& h)
{
    for(hex* el : h.getNeighbors())
    {
        if(el->getPlayer() == player::NONE)
        {
            if(this->node_right.getID() != el->getID() && this->node_down.getID() != el->getID())
                this->fringe_set.insert(el);
        }
    }
}
void HexGraph::add_fringe(hex& h)
{
    //if(this->fringe_set.find(&h) != this->fringe_set.end())
        this->fringe_set.erase(&h);
    this->read_fringe(h);
}
//creates edge list graph
void HexGraph::connectGraph(void)
{
    for(int i = 0; i < this->size; i++)
    {
        for(int j = 0; j < this->size; j++)
        {
            std::vector<std::pair<int,int>> hs = this->calculate_neighbors(i,j);
            this->access(i,j).clearNeighbors();
            for(auto el : hs)
            {
                int x = el.first;
                int y = el.second;
                this->access(i,j).addNeighbor(&this->access(x,y));
            }
        }
    }
    
    //initializes start and goal nodes for checkWin
    for(int i = 0; i<this->size; i++)
    {
        hex& l = this->access(0,i);
        node_left.addNeighbor(&l);
        this->fringe_set.insert(&l);
        
        hex& r = this->access(this->size-1,i);
        r.addNeighbor(&this->node_right);
        this->fringe_set.insert(&r);
    }
    for(int i = 0; i<this->size; i++)
    {
        hex& u = this->access(i,0);
        node_up.addNeighbor(&u);
        this->fringe_set.insert(&u);
        
        hex& d = this->access(i,this->size-1);
        d.addNeighbor(&this->node_down);
        this->fringe_set.insert(&d);
    }
    
    //initializes center fringe set
    int center = this->size/2;
    hex& center_hex = this->access(center,center);
    this->fringe_set.insert(&center_hex);
    this->read_fringe(center_hex);
    
    /*std::cout << "Initial fringe_set" << std::endl;
    for(auto el : this->fringe_set)
    {
        std::pair<int,int> i = el->getID();
        std::cout << "(" << i.first << "," << i.second << std::endl;
    }*/
}
//pre-order depth-first search
bool HexGraph::search(hex* current, hex* node_goal, player p, std::set<hex*> &closed)
{
    std::vector<hex*> edge_list = current->getNeighbors();

    for(hex* h : edge_list)
    {
        if(h == node_goal)//found goal node; player wins
            return true;
        else if(h->getPlayer() != p)//h does not belong to player; skip
            continue;
        else 
        {
            std::set<hex*>::iterator it = closed.find(h);
            if(it != closed.end())//if h is already in set, skip
                continue;
            closed.insert(h);//insert h 
            if(this->search(h, node_goal, p, closed))//recursively continue search
                return true;
        }
    }
    return false;
}
//checks if player p won
bool HexGraph::checkWin(player p)
{
    hex* node_start;
    hex* node_goal;
    switch(p)
    {
        case player::ONE:
            node_start = &this->node_left;
            node_goal = &this->node_right;
            break;
        case player::TWO:
            node_start = &this->node_up;
            node_goal = &this->node_down;
            break;
        default:
            std::cerr << "Invalid Player" << std::endl;
            return false;
    }
    std::set<hex*> closed;
    return this->search(node_start, node_goal, p, closed);
}
player HexGraph::getWinner()
{
    if(this->checkWin(player::ONE))
        return player::ONE;
    else if(this->checkWin(player::TWO))
        return player::TWO;
    else
        return player::NONE;
}
std::set<std::pair<int,int>> HexGraph::getSet()
{
    return this->none_set;
}
std::set<hex*> HexGraph::getFringe()
{
    return this->fringe_set;
}
std::set<std::pair<int,int>> HexGraph::getNone()
{
    return this->none_set;
}
bool HexGraph::move(int x, int y, player p)
{
    return this->move(this->access(x,y), p);
}
bool HexGraph::move(std::pair<int,int> coor, player p)
{
    hex& h = this->access(coor.first,coor.second);
    return this->move(h, p);
}
bool HexGraph::move(hex& h, player p)
{
    if(this->none_set.find(h.getID()) == this->none_set.end())
        return false;
    this->none_set.erase(h.getID());
    h.setPlayer(p);
    this->add_fringe(h);
    return this->checkWin(p);
}
void HexGraph::clear(std::set<std::pair<int,int>>& s)
{
    for(auto el : s)
    {
        this->access(el.first, el.second).setPlayer(player::NONE);
        this->none_set.insert(el);
    }
}
//returns a random empty hex
hex* HexGraph::random_empty_hex()
{
    int limit = this->none_set.size();
    int rnum = (std::uniform_int_distribution<int>(0,limit-1))(rgen);
    std::set<std::pair<int,int>>::iterator ret = this->none_set.begin();
    std::advance(ret, rnum);
    return &(this->access(ret->first,ret->second));
}
/*hex* HexGraph::random_empty_hex()
{
    //goes until empty hex found, or all hexes exhausted
    for(int i = 0; i < this->size*this->size; i++)
    {
        int x = this->dist(this->rgen);
        int y = this->dist(this->rgen);
        hex* temp = &this->access(x,y);
        //hex* temp = &this->grid.at(x).at(y);
        if(temp->getPlayer() == player::NONE)
        {
            return temp;
        }
    }
    return nullptr;
}*/
//randomly fills rest of board; player p moves next
void HexGraph::random_fill(player p, player other)
{
    int hxs = this->none_set.size();
    std::vector<std::pair<int,int>> board;
    //for(std::set<std::pair<int,int>>::iterator it : this->none_set)
    for(auto el : this->none_set)
    {
        board.push_back(el);
    }
    std::shuffle(board.begin(), board.end(), this->rgen);
    for(int i = 0; i < hxs; i++)
    {
        if(i%2 == 0)
            this->move(board.at(i), p);
        else
            this->move(board.at(i), other);
    }
}
//randomly fills board
void HexGraph::random(void)
{
    int hxs = this->size*this->size;
    std::vector<player> board(hxs);//vector with enough space to represent every hex
    for(int i = 0; i < hxs/2; i++)
    {
        board.at(i) = player::TWO;//fills first half of vector with player::TWO
    }
    for(std::vector<player>::iterator i = board.begin()+hxs/2; i != board.end(); i++)
    {
        *i = player::ONE;//fills rest of vector with player::ONE
    }
    std::shuffle(board.begin(), board.end(), this->rgen);//shuffles vector
    std::vector<player>::iterator p = board.begin();//points iterator at beginning of vector
    for(std::vector<hex>::iterator i = this->grid.begin(); i != this->grid.end(); i++)
    {
        i->setPlayer(*(p++));
    }
}
//randomly takes a move for player p; returns true if it was a winning move
bool HexGraph::random_move(player p)
{
    hex* h = this->random_empty_hex();
    if(h == nullptr)
        return false;
    h->setPlayer(p);
    return this->checkWin(p);
}
//Takes turns at random and returns the winner
player HexGraph::play_random()
{
    for(int i = 0; i < this->size*this->size; i++)//ends when board is full
    {
        if(i%2 == 0)//on even i's player one moves
        {
            if(random_move(player::ONE))
                return player::ONE;
        }
        else if(random_move(player::TWO))
            return player::TWO;
    }
    if(checkWin(player::ONE))
        return player::ONE;
    else if(checkWin(player::TWO))
        return player::TWO;
    else
        return player::NONE;
}
void HexGraph::play_move(player p)
{
    std::cout << "play_move" << std::endl;
    minmax* tr = new minmax(*this, p);
    tr->monte_carlo(256);
    std::pair<int,int> m = tr->getMove();
    std::cout << "moving to (" << m.first << "," << m.second <<")" << std::endl;
    this->move(m,p);
    delete tr;
}
//constructor
HexGraph::HexGraph(int s)
{
    this->size = s;
    this->grid.resize(this->size*this->size);
    this->dist = std::uniform_int_distribution<int>(0,s-1);
    this->rgen = std::mt19937(time(nullptr));
    for(int i = 0; i < this->size; i++)
    {
        for(int j = 0; j < this->size; j++)
        {
            this->access(i,j) = hex(i,j);  
            this->none_set.insert(std::make_pair(i,j));
        }
    }
    this->node_left = hex(-1,-1);
    this->node_right = hex(this->size, this->size);
    this->node_up = hex(this->size, -1);
    this->node_down = hex(-1, this->size);
    this->connectGraph();
}
HexGraph::HexGraph(const HexGraph& other)
{
    this->size = other.size;
    this->grid = other.grid;
    this->none_set = other.none_set;
    this->dist = other.dist;
    this->rgen = other.rgen;
    this->rgen.seed(time(nullptr));
    this->node_left = hex(-1,-1);
    this->node_right = hex(this->size, this->size);
    this->node_up = hex(this->size, -1);
    this->node_down = hex(-1, this->size);
    this->connectGraph();
}
//destructor
HexGraph::~HexGraph(){}
//operator overload
std::ostream& operator<<(std::ostream& out, const HexGraph& hg)
{
    int index = 0;
    std::string pad = "";
    for(int i = 0; i < hg.size; i++)
    {
        out << pad;
        for(int j = 0; j < hg.size; j++)
            out<<"\\"<<hg.grid.at(index++)<<"\\"<<" ";
        out << std::endl;
        pad = pad + " ";
    }
    return out;
}

#endif