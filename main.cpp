#include "hex.h"
//#include "minmax.cpp"
#include "montecarlo.cpp"
#include <iostream>
#include <string>

/*inline std::string winner(player p)
{
    switch(p)
    {
        case player::ONE:
            return "Player One";
        case player::TWO:
            return "Player Two";
        default:
            return "No players";
    }
}*/

int main()////
{
    HexGraph test = HexGraph();
    for(;;)
    {
        std::cout << test << std::endl;
        std::cout << "Enter x" << std::endl;
        int x;
        std::cin >> x;
        std::cout << "Enter y" << std::endl;
        int y;
        std::cin >> y;
        
        test.move(x,y,player::ONE);
        std::cout << test << std::endl;
        test.play_move(player::TWO);
        player t_win = test.getWinner();
        
        //std::cout << std::endl << test << std::endl;
        if(t_win != player::NONE)
        {
            std::string s;
            switch(t_win)
            {
                case player::ONE:
                    s = "Player One";
                    break;
                case player::TWO:
                    s = "Player Two";
                    break;
                default:
                    s = "No players";
            }
            std::cout << s << " won!" << std::endl;
            break;
        }
    }
}
/**int main()
{
    HexGraph test = HexGraph();
    std::vector<int> xs = {0,1,2,3,4,5,6,7,8,9,10};
    std::vector<int> ys = {5,5,5,5,5,5,5,5,5,5,5};
    for(int i = 0;;i++)
    {
        std::cout << test << std::endl;
        std::cout << "Enter x" << std::endl;
        int x = i > 11 ? 0 : xs.at(i);
        std::cout << x << std::endl;
        std::cout << "Enter y" << std::endl;
        int y = i > 11 ? 0 : ys.at(i);
        std::cout << y << std::endl;
        
        test.move(x,y,player::ONE);
        std::cout << test << std::endl;
        test.play_move(player::TWO);
        player t_win = test.getWinner();
        
        //std::cout << std::endl << test << std::endl;
        if(t_win != player::NONE)
        {
            std::string s;
            switch(t_win)
            {
                case player::ONE:
                    s = "Player One";
                    break;
                case player::TWO:
                    s = "Player Two";
                    break;
                default:
                    s = "No players";
            }
            std::cout << s << " won!" << std::endl;
            break;
        }
    }
}**/
/*int main()
{
    HexGraph test = HexGraph();
    player p = test.play_random();
    std::string s = winner(p);
    std::cout << s << " won!" << std::endl;
    std::cout << test << std::endl;
    return 0;
}*/
