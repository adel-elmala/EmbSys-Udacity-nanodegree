#include "card.h"
#include "terminal.h"
#include "server.h"

int main(int argc, char const *argv[])
{

#ifdef cardHolderNameTest
    getCardHolderNameTest();
#elif defined(cardExpirayDateTest)
    getCardExpiryDateTest();

#elif defined(cardPANTest)
    getCardPANTest();

#elif defined(TransactionDateTest)
    getTransactionDateTest();

#elif defined(CardExpiredTest)
    isCardExpriedTest();

#elif defined(TransactionAmountTest)
    getTransactionAmountTest();

#elif defined(BelowMaxAmountTest)
    isBelowMaxAmountTest();

#elif defined(MaxAmountTest)
    setMaxAmountTest();

#elif defined(recieveTransDataTest)
    recieveTransactionDataTest();

#elif defined(ValidAccountTest)
    isValidAccountTest();

#elif defined(BlockedAccountTest)
    isBlockedAccountTest();

#elif defined(AmountAvailableTest)
    isAmountAvailableTest();

#elif defined(saveTransTest)
    saveTransactionTest();

#endif
    return 0;
}
