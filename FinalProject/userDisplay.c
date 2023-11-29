/*
 * userDisplay.c
 *
 *  Created on: Nov 13, 2023
 *      Author: Trevor Buck
 */
#include "userDisplay.h"

#include <string.h>

/*
 * this is ugly as hell, if anyone has any better idea to
 * implement this, let me know
 */

void userDisplay_init() {
  lcd_init();
  button_init();

  strcpy(creamOptions[0].creamName, "Vanilla");
  creamOptions[0].creamCount = 5;
  creamOptions[0].creamCost = 2.0;
  creamOptions[0].creamChar = 'v';

  strcpy(creamOptions[1].creamName, "Chocolate");
  creamOptions[1].creamCount = 10;
  creamOptions[1].creamCost = 2.50;
  creamOptions[1].creamChar = 'c';

  strcpy(creamOptions[2].creamName, "Mint");
  creamOptions[2].creamCount = 3;
  creamOptions[2].creamCost = 2.0;
  creamOptions[2].creamChar = 'm';

  strcpy(creamOptions[3].creamName, "Green Tea");
  creamOptions[3].creamCount = 15;
  creamOptions[3].creamCost = 3.50;
  creamOptions[3].creamChar = 't';
}
/*
 * Once we figure out which Ice creams we want to keep
 * we can clean this function up
 */
void printMenu() {

    lcd_clear();

    int i = 0;
    char message[21];
    for (i = 0; i < 4; i++) {
        lcd_gotoLine(i+1);
        sprintf(message, "%s %d $%.2lf", creamOptions[i].creamName, creamOptions[i].creamCount, creamOptions[i].creamCost);
        lcd_puts(message);
    }

//  lcd_printf(
//      "%s %d $%.2lf\n%s %d $%.2lf\n%s %d $%.2lf\n%s %d $%.2lf\n%s %d $%.2lf",
//      creamOptions[0].creamName, creamOptions[0].creamCount,
//      creamOptions[0].creamCost, creamOptions[1].creamName,
//      creamOptions[1].creamCount, creamOptions[1].creamCost,
//      creamOptions[2].creamName, creamOptions[2].creamCount,
//      creamOptions[2].creamCost, creamOptions[3].creamName,
//      creamOptions[3].creamCount, creamOptions[3].creamCost);
}

void sellMenu(char choice, cream_t *creamOptions) {
    while (button_getButton() != 0) {} // Wait till user is no longer pressing buttons

    char qty = 0;
    while (1) {
      char qtyChoice = 0;
      lcd_printf("Buy %s?\nStock: %d at $%.2lf\n-(1) %d +(2)\n3-Confirm 4-Deny\n",
                 creamOptions[choice].creamName, creamOptions[choice].creamCount, creamOptions[choice].creamCost, qty);
      timer_waitMillis(1);
      while (qtyChoice == 0) { // while nothing is being pressed, wait for input
          qtyChoice = button_getButton();
      }

      if (qtyChoice == 1) { // decrement qty
          if (qty > 0) { qty--; }

      } else if (qtyChoice == 2) { // increment qty
          if (qty < creamOptions[choice].creamCount) { qty++; }

      } else if (qtyChoice == 3) { // confirm, send data subtract from creamCount
          char message[4];
          sprintf(message, "%d", qty);

          uart_sendChar(creamOptions[choice].creamChar);
          uart_sendStr(message);
          uart_sendChar('\n');

          creamOptions[choice].creamCount -= qty;
          lcd_printf("Thank you for\nyour purchase!");
          timer_waitMillis(2000);
          return;

      } else if (qtyChoice == 4) {lcd_clear(); return;} // decline

      timer_waitMillis(1);
      while (qtyChoice != 0) { // reset userChoice back to zero
          qtyChoice = button_getButton();
      }
  }

}


void userDisplay_sell() {
  char userChoice = 0;
  int timeout;

  while (1) {
    printMenu();

    timeout = 0;
    timer_waitMillis(1);

    while (userChoice == 0) { // while nothing is being pressed, wait for input
      userChoice = button_getButton();
      timer_waitMillis(1);
      timeout++;
      if (timeout > 15000) {lcd_clear();return;}
    }

    sellMenu(userChoice - 1, creamOptions);

    timer_waitMillis(1);
    while (userChoice != 0) { // reset userChoice back to zero essentially ensure the button is only pressed once
        userChoice = button_getButton();
    }
  }
}

