// 2: A space research organization is developing a simulation to track the fuel levels of a
// spacecraft as it travels between multiple planets. During its journey, the spacecraft consumes a fixed
// amount of fuel at each planetary stop due to thrust and atmospheric entry. However, when it passes
// through certain gravitational zones, it regenerates some fuel because of gravitational assists that
// reduce energy usage. Additionally, after every fourth planetary stop, the spacecraft undergoes a
// “solar recharge,” where its solar panels absorb extra energy from nearby stars, increasing its fuel
// reserves. The mission control team wants to model this behavior using a recursive function
// calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets) that
// prints the remaining fuel after each planet in the format "Planet X: Fuel Remaining = Y", and
// determines whether the spacecraft can complete its journey. The base case occurs when either the
// fuel is exhausted or all planets have been visited, and the recursive case handles fuel adjustments for
// each subsequent planet.
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
