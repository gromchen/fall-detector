#ifndef FRUIT_H
#define FRUIT_H

class Fruit
{
private:
    int _quantity;

public:
    Fruit(int quantity);

    int GetQuantity();
    void Buy(int quantity);
    void Sell(int quantity);
};

#endif