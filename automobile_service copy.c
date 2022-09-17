#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    char service1[30], service2[20];
    char service3[30], service4[20];
    char s1[20], s2[20];
    int oc = 35, tr = 19, cwh = 7, cwx = 12, i, t;
    
    //print the menu
    printf("Davy's auto shop services\n");
    printf("Oil change -- $35\n");
    printf("Tire rotation -- $19\n");
    printf("Car wash -- $7\n");
    printf("Car wax -- $12\n\n");
    
    //first service
    printf("Select first service:\n");
    scanf("%s", service1);
    //If the user doesn't press -, we will get the second word.
    if (strcmp(service1, "-") != 0) {
        scanf("%s", service2);
        //Concatenate the two strings
        strcat(service1, " ");
        strcat(service1, service2);
    }
    
    //Second service
    printf("Select second service:\n");
    scanf("%s", service3);
    //If the user doesn't press -, we will get the second word.
    if (strcmp(service3, "-") != 0) {
        scanf("%s", service4);
        //Concatenate the two strings
        strcat(service3, " ");
        strcat(service3, service4);
    }
    
    
    //Identifying service 1's fee
    if (strcmp(service1, "Oil change") == 0) {
        strcpy(s1, "35");
        i = 1;
    }
    else if (strcmp(service1, "Tire rotation") == 0) {
        strcpy(s1, "19");
        i = 2;
    }
    else if (strcmp(service1, "Car wash") == 0) {
        strcpy(s1, "7");
        i = 3;
    }
    else if (strcmp(service1, "Car wax") == 0) {
        strcpy(s1, "12");
        i = 4;
    }
    else {
        strcpy(s1, "No service");
        i = 5;
    }
    
    //Identifying service 2's fee
    if (strcmp(service3, "Oil change") == 0) {
        strcpy(s2, "35");
        switch (i) {
            case 1:
                t = oc + oc;
                break;
            case 2:
                t = oc + tr;
                break;
            case 3:
                t = oc + cwh;
                break;
            case 4:
                t = oc + cwx;
                break;
            case 5:
                t = oc;
                break;
        }
    }
    
    else if (strcmp(service3, "Tire rotation") == 0) {
        strcpy(s2, "19");
        switch (i) {
            case 1:
                t = tr + oc;
                break;
            case 2:
                t = tr + tr;
                break;
            case 3:
                t = tr + cwh;
                break;
            case 4:
                t = tr + cwx;
                break;
            case 5:
                t = tr;
                break;
        }
    }
    
    else if (strcmp(service3, "Car wash") == 0) {
        strcpy(s2, "7");
        switch (i) {
            case 1:
                t = cwh + oc;
                break;
            case 2:
                t = cwh + tr;
                break;
            case 3:
                t = cwh + cwh;
                break;
            case 4:
                t = cwh + cwx;
                break;
            case 5:
                t = cwh;
                break;
        }
    }
    
    else if (strcmp(service3, "Car wax") == 0) {
        strcpy(s2, "12");
        switch (i) {
            case 1:
                t = cwx + oc;
                break;
            case 2:
                t = cwx + tr;
                break;
            case 3:
                t = cwx + cwh;
                break;
            case 4:
                t = cwx + cwx;
                break;
            case 5:
                t = cwx;
                break;
        }
    }
    else {
        strcpy(s2, "No service");
        switch (i) {
            case 1:
                t = oc;
                break;
            case 2:
                t = tr;
                break;
            case 3:
                t = cwh;
                break;
            case 4:
                t = cwx;
                break;
            case 5:
                t = 0;
                break;
        }
    }
    
    //Print the invoice and account for 4 kinds of scenarios
    printf("\nDavy's auto shop invoice\n\n");
    if (isdigit(s1[0])) {
        printf("Service 1: %s, $%s\n", service1, s1);
    }
    else {
        printf("Service 1: No service\n");
    }
    
    if (isdigit(s2[0])) {
        printf("Service 2: %s, $%s\n\n", service3, s2);
    }
    else {
        printf("Service 2: No service\n\n");
    }
    
    printf("Total: $%d\n", t);
   return 0;
}
