#include "app.h"
#include "card.h"
#include "terminal.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_AMOUNT 5000.0f
void appStart(void)
{
    ST_cardData_t userCard = {0};
    ST_terminalData_t term = {0};

    // card
    getCardHolderName(&userCard);
    getCardExpiryDate(&userCard);
    getCardPAN(&userCard);
    // terminal
    getTransactionDate(&term);
    if (isCardExpired(&userCard, &term) == EXPIRED_CARD)
        exit(EXPIRED_CARD);

    setMaxAmount(&term, MAX_AMOUNT);
    getTransactionAmount(&term);
    if (isBelowMaxAmount(&term) == EXCEED_MAX_AMOUNT)
        exit(EXCEED_MAX_AMOUNT);

    // server
    ST_transaction_t trans = {0};
    trans.cardHolderData = userCard;
    trans.terminalData = term;
    recieveTransactionData(&trans);
}

int main()
{
    appStart();
    return 0;
}