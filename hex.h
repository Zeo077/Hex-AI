#ifndef __HEX_H_INCLUDE__//include guard
#define __HEX_H_INCLUDE__

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <set>

enum class player {NONE=0,ONE=1,TWO=2,OTHER=-1};

class hex
{
//member variables
    protected:
        std::pair<int,int> id;
        std::vector<hex*> neighbors;//edge list
        player p;
    public:
//accessors
        inline const std::pair<int,int> getID();
        inline const std::vector<hex*> getNeighbors();
        inline const player getPlayer();
//mutators        
        inline void setPlayer(player pn);
        inline void addNeighbor(hex* n); 
        inline void clearNeighbors();    
//constructors
        hex(std::pair<int,int> idn = std::make_pair(-1,-1));
        hex(int x, int y);
        ~hex();
//operator overload        
        friend std::ostream& operator<<(std::ostream& out, const hex& h);
};

class HexGraph
{
//member variables
    private:
        int size;
        std::vector<hex> grid;//matrix
        hex node_left;//start node for player one
        hex node_right;//goal node for player one
        hex node_up;//start node for player two
        hex node_down;//start node for player two
        std::set<std::pair<int,int>> none_set;
        std::set<hex*> fringe_set;
        //random
        std::mt19937 rgen;
        std::uniform_int_distribution<int> dist;
        std::pair<int,int> nodeToCoor(int node);
        int coorToNode(int x, int y);
        int coorToNode(std::pair<int,int> c);
        std::vector<std::pair<int,int>> calculate_neighbors(std::pair<int,int> c);
        std::vector<std::pair<int,int>> calculate_neighbors(int x, int y);
        hex& access(int x, int y);
        void read_fringe(hex& h);
        void add_fringe(hex& h);
//creates edge list graph
        void connectGraph(void);
//pre-order breadth-first search
        bool search(hex* node_start, hex* node_end, player p, std::set<hex*> &closed);
//checks if player p won
        bool checkWin(player p);
//returns a random empty hex
        hex*
         random_empty_hex();
        hex* random_fringe_hex();
    public:
        player getWinner();
        std::set<std::pair<int,int>> getSet();
        std::set<hex*> getFringe();
        std::set<std::pair<int,int>> getNone();
        bool move(int x, int y, player p);
        bool move(std::pair<int,int> coor, player p);
        bool move(hex& h, player p);
        void clear(std::set<std::pair<int,int>>& s);
        
        
//randomly fills rest of board; player p moves next
        void random_fill(player p, player other);
//randomly fills board
//deprecated
        void random(void);
//randomly takes a move for player p; returns true if it was a winning move
        bool random_move(player p);
//takes turns at random and returns the winner
        player play_random();
        void play_move(player p);
//constructor
        HexGraph(int s = 11);
        HexGraph(const HexGraph& other);
        ~HexGraph();
//operator overload
        friend std::ostream& operator<<(std::ostream& out, const HexGraph& hg);
};
#endif