#include <stdio.h>

int main(void)
{
    int change;
    int dollar = 100, dollar_change, num_dollar = 0;
    // amount, how much non-divisible vale left, number of each coin
    int quarter = 25, quarter_change, num_quarter = 0;
    int dime = 10, dime_change, num_dime = 0;
    int nickel = 5, nickel_change, num_nickel = 0;
    int penny = 1, penny_change, num_penny = 0;
    
    
    
    scanf("%d", &change);
    
    //Building a cascade of nested if-else branches that follows the same logic
    
    if (change == 0)
    {
        printf("No change\n");
    }
    else
    {
        if (change % dollar == 0)
        {
            num_dollar = change/dollar; //If dollar can make all the change, give that number of $
        }
        else //If some money left
        {
            num_dollar = change/dollar; //Still record the largest divisor
            dollar_change = change % dollar; //Amount left in dollar_change
            if (dollar_change % quarter == 0)
            {
                num_quarter = dollar_change / quarter;
            }
            else
            {
                num_quarter = dollar_change / quarter;
                quarter_change = dollar_change % quarter;
                if (quarter_change % dime == 0) //Repeat the process until the coin can fully fill the                              amount left.
                {
                    num_dime = quarter_change / dime;
                }
                else
                {
                    num_dime = quarter_change / dime;
                    dime_change = quarter_change % dime;
                    if (dime_change % nickel == 0)
                    {
                        num_nickel = dime_change/nickel;
                    }
                    else
                    {
                        num_nickel = dime_change / nickel;
                        nickel_change = dime_change % nickel;
                        if (nickel_change % penny == 0)
                        {
                            num_penny = nickel_change / penny;
                        }
                    }
                }
            }
            
            
        }
            //print nums if they're not zeros
            if (num_dollar != 0)
            {
                if (num_dollar > 1)
                {
                    printf("%d Dollars\n", num_dollar);
                }
                else
                {
                    printf("%d Dollar\n", num_dollar);
                }
            }
            if (num_quarter != 0)
            {
                if (num_quarter > 1)
                {
                    printf("%d Quarters\n", num_quarter);
                }
                else
                {
                    printf("%d Quarter\n", num_quarter);
                }
            }
            if (num_dime != 0)
            {
                if (num_dime > 1)
                {
                    printf("%d Dimes\n", num_dime);
                }
                else
                {
                    printf("%d Dime\n", num_dime);
                }
            }
            if (num_nickel != 0)
            {
                if (num_nickel > 1)
                {
                    printf("%d Nickels\n", num_nickel);
                }
                else
                {
                    printf("%d Nickel\n", num_nickel);
                }
            }
            if (num_penny != 0)
            {
                if (num_penny > 1)
                {
                    printf("%d Pennies\n", num_penny);
                }
                else
                {
                    printf("%d Penny\n", num_penny);
                }
            }
                
    }
    return 0;
}
