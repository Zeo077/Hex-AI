#include <vector>
#include <climits>

#include "hex.h"

struct tree
{
    //int value;
    std::pair<int,int> value;
    HexGraph state;
    player p;
    std::pair<int,int> move;
    tree():value(std::make_pair(0,0)),p(player::NONE){}
    tree(HexGraph g, player pl, std::pair<int,int> v = std::make_pair(0,0)):value(v),state(g),p(pl){}
};
///INT_MIN
class minmax
{
    private:
        tree* root;
        HexGraph base_state;
        std::vector<tree*> children;
        //tree* favorite_child;
        std::pair<int,int> favorite_move;
        std::pair<int,int> max_value;
        player p;
    public:
        void monte_carlo(int trials)
        {
            //int debug_int = 4;
            std::cout << "monte_carlo" << std::endl;
            std::set<std::pair<int,int>> poss = this->base_state.getNone();
            HexGraph hg = this->base_state;
            for(std::pair<int,int> el : poss)
            //for(hex* el : this->base_state.getFringe())
            {
                //tree* t = new tree(this->base_state, this->p);
                //t->move = el->getID();
                //t->state.move(t->move,this->p);
                //hg.move(el, this->p);
                
                player other;
                if(this->p == player::ONE)
                    other = player::TWO;
                else 
                    other = player::ONE;
                
                int wins = 0, losses = 0;
                hg.move(el, this->p);
                //hg.move(el->getID().first, el->getID().second, this->p);
                std::set<std::pair<int,int>> nS = hg.getNone();
                for(int i = 0; i < trials; i++)
                {
                    //hg.move(el, this->p);
                    //HexGraph hg = t->state;
                    hg.random_fill(this->p, other);
                    player win = hg.getWinner();
                    if(win == this->p)
                        wins++;
                    else
                        losses--;
                    /*if(debug_int-- > 0)
                    {
                        if(win == this->p) std::cout << "win" << std::endl;
                        std::cout << hg << std::endl;
                        hg.clear(nS);
                    }
                    else*/
                        hg.clear(nS);
                }
                hg.clear(poss);
                //this->children.push_back(t);
                if(wins > this->max_value.first)
                {
                    this->max_value = std::make_pair(wins,losses);
                    this->favorite_move = el;//el->getID();
                    //std::cout << "*" << std::endl;
                }
                /*std::cout << "move: ";
                //std::cout << "(" << el->getID().first << "," << el->getID().second << ")" << std::endl;
                std::cout << "(" << el.first << "," << el.second << ")" << std::endl;
                std::cout << "(" << wins << "," << losses << ")" << std::endl;*/
            }
            /*for(auto el : this->children)
            {
                std::cout << "move: ";
                std::cout << "(" << el->move.first << "," << el->move.second << ")" << std::endl;
                std::cout << "(" << el->value.first << "," << el->value.second << ")" << std::endl;
            }*/
            std::cout << std::endl;
        }
        std::pair<int,int> getMove()
        {
            /*if(this->favorite_child == nullptr)
                //return std::make_pair(-7,-7);
                this->monte_carlo(10);*/
            return this->favorite_move;
            //return this->favorite_child->move;
        }
        minmax(HexGraph& g, player pl):p(pl),max_value(std::make_pair(INT_MIN,INT_MIN))
        {
            this->base_state = g;
        }
        ~minmax()
        {
            this->children.clear();
        }
};