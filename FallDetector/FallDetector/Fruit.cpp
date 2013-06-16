#include "Fruit.h"

Fruit::Fruit(int quantity)
{
    _quantity = quantity;
}

int Fruit::GetQuantity()
{
    return _quantity;
}

void Fruit::Buy(int quantity)
{
    _quantity += quantity;
}

void Fruit::Sell(int quantity)
{
    _quantity -= quantity;
}