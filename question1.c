//Muhammad Anas
//25k-0899
#include <stdio.h>
// A bank wants to calculate the total repayment amount for a loan using a recursive
// approach. The loan decreases each year after paying a fixed installment, and interest is applied
// annually to the remaining loan. Design a recursive function calculateRepayment(loan, interestRate, years)
// that prints the repayment schedule year by year in the format "Year X: Remaining loan = Y" and
// calculates the total repayment for n years. Identify the base case (loan fully repaid or no years
// remaining) and the recursive case (repayment for remaining years). Using this function, trace the
// repayment for a loan of 100,000 at 5% yearly interest over 3 years, and explain how recursion
// helps track the loan state across years. Extend the function logically to handle extra yearly
// payments and discuss why recursion is suitable for such incremental calculations.
void calculateRepayment(float loan,float interestRate,float extraPayment,float instalement,int currentYear,int years){
    if(years == 0){
        return;
    }else if(loan <= 0){
        printf("Loan already fully repaid.\n");
        return;
    }
    else{
        float interest = loan * (interestRate / 100.0);
        float remainingLoan = (loan + interest) - extraPayment- instalement; //deduct from remaining 
        if (remainingLoan <= 0) {
            remainingLoan = 0;
        }
        printf("Year:%dReamining Loan:%.2f\n",currentYear,remainingLoan);
        calculateRepayment(remainingLoan,interestRate,extraPayment,instalement,currentYear+1,years-1);
    }
}
int main(){
float principal = 100000;
    float rate = 5.0;
    int duration = 3;
    float extraPayment = 1000; //adding extra payment 
    float installment = 36721.0; 

    printf("Starting Loan: %.2f\n", principal);
    printf("--- Repayment Schedule ---\n");
    
    calculateRepayment(principal,rate,extraPayment,installment,1,duration);
    return 0;
    //recursion is perfectly apply on this problem as they are calculating same for year 1 and year 2 
    //and there is also a base case for this to stop 
}