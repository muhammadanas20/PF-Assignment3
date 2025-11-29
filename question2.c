//Muhammad Anas
//25k-0899
#include <stdio.h>

void calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets){
    if(fuel <= 0){
        return;
    }
    else if(planet > totalPlanets){
        return;
    }
    else{
        int fuelRemaining = fuel-consumption;
        if(planet%4 == 0){
           fuelRemaining += solarBonus;
        } 
        if (planet % 3 == 0) {
            fuelRemaining += recharge;
        }
        if (fuelRemaining <= 0) {
             printf("Planet %d: Fuel Remaining = 0 \n", planet);
             calculateFuel(0, consumption, recharge, solarBonus, planet + 1, totalPlanets);
             return;
        }
        printf("Planet%d:Remaining fuel:%d\n",planet,fuelRemaining);
        calculateFuel(fuelRemaining,consumption,recharge,solarBonus,planet+1,totalPlanets);
    }
}
int main(){
    calculateFuel(100,5,20,4,1,9);
}
