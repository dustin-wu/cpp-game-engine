#pragma once

#include <vector>

class ShopHandler
{
public:
    ShopHandler( int numItems ) {
        for (int i = 0; i < numItems; i++) {
            itemsBought.push_back(false);
        }
    }

    bool itemBought(int itemIndex) {
        return itemsBought[itemIndex];
    }

    void setItemBought(int itemIndex) {
        itemsBought[itemIndex] = true;
    }

    void reset() {
        for (int i = 0; i < itemsBought.size(); i++) {
            itemsBought[i] = false;
        }
        numSteel = 0;
    }

    int numSteel = 0;

private:
    std::vector<bool> itemsBought;

};


namespace ShopHandling{
    inline ShopHandler handler(6);
}
