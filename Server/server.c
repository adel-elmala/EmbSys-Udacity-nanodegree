#include "server.h"
#include <string.h>
#include <stdio.h>

unsigned int nAccounts = 5;
unsigned int ntrans = 0;
ST_accountsDB_t accountsDB[255] = {{2000.0, RUNNING, "8989374615436851"},
                                   {100000.0, BLOCKED, "5807007076043875"},
                                   {9500.0, RUNNING, "8506006067047538"},
                                   {102.0, RUNNING, "2314007066043425"},
                                   {688030.0, RUNNING, "8107023056047815"}};

ST_transaction_t transDB[255] = {0};

ST_accountsDB_t *findAccount(uint8_t PAN[20]);
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
    ST_accountsDB_t *trgt;
    if ((trgt = findAccount(cardData->primaryAccountNumber)) == NULL)

        return ACCOUNT_NOT_FOUND;
    else
    {
        *accountRefrence = *trgt;
        return SERVER_OK;
    }
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    if (accountRefrence->state == RUNNING)
        return SERVER_OK;
    else
        return BLOCKED_ACCOUNT;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
    if ((termData->transAmount) > (accountRefrence->balance))
        return LOW_BALANCE;
    else
        return SERVER_OK;
}
void listSavedTransactions(void)
{
    for (int i = 0; i < ntrans; ++i)
    {
        fprintf(stdout, "#########################\n");
        fprintf(stdout, "Transaction Sequence Number: %d\n", transDB[i].transactionSequenceNumber);
        fprintf(stdout, "Transaction Date: %s\n", transDB[i].terminalData.transactionDate);
        fprintf(stdout, "Transaction Amount: %.2f\n", transDB[i].terminalData.transAmount);
        fprintf(stdout, "Transaction State: %d\n", transDB[i].transState);
        fprintf(stdout, "Terminal Max Amount: %.2f\n", transDB[i].terminalData.maxTransAmount);
        fprintf(stdout, "Cardholder Name: %s\n", transDB[i].cardHolderData.cardHolderName);
        fprintf(stdout, "PAN: %s\n", transDB[i].cardHolderData.primaryAccountNumber);
        fprintf(stdout, "Card Expiration Date: %s\n", transDB[i].cardHolderData.cardExpirationDate);
        fprintf(stdout, "#########################\n");
    }
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    // fix me once you fit thing together
    static uint32_t seqN = 0;
    if (ntrans < 255)
    {

        transData->transactionSequenceNumber = seqN;
        transDB[ntrans] = *transData;
        ntrans++;
        seqN++;
        listSavedTransactions();
        return SERVER_OK;
    }
    else
        return SAVING_FAILED;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    ST_accountsDB_t *target;
    EN_transState_t state = APPROVED;
    if ((target = findAccount((transData->cardHolderData).primaryAccountNumber)) == NULL)
        state = FRAUD_CARD;
    if (target && (target->balance < (transData->terminalData).transAmount))
        state = DECLINED_INSUFFECIENT_FUND;
    if (target && (target->state == BLOCKED))
        state = DECLINED_STOLEN_CARD;
    if (nAccounts >= 255)
        state = INTERNAL_SERVER_ERROR;
    if (state == APPROVED)
        target->balance -= (transData->terminalData).transAmount;
    transData->transState = state;
    saveTransaction(transData);
    return state;
}

ST_accountsDB_t *findAccount(uint8_t PAN[20])
{
    for (int i = 0; i < nAccounts; i++)
    {
        if (strncmp((accountsDB[i]).primaryAccountNumber, PAN, 20) == 0)
            return accountsDB + i;
    }
    return NULL;
}

void saveTransactionTest(void)
{
    EN_cardError_t result;
    ST_transaction_t trans = {0};

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: saveTransaction\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: Success \n");
    result = saveTransaction(&trans);
    fprintf(stderr, "Expected Result:%d\n", SERVER_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    fprintf(stderr, "Test Case 2: Failure \n");
    fprintf(stderr, "Input Data:  \n");
    int tmp = ntrans;
    ntrans = 255;
    result = saveTransaction(&trans);
    fprintf(stderr, "Expected Result:%d\n", SAVING_FAILED);
    fprintf(stderr, "Actual Result:%d\n\n", result);
    ntrans = tmp;
}
void isAmountAvailableTest(void)
{
    EN_cardError_t result;
    ST_accountsDB_t account = {0};
    account.balance = 5000.0;
    ST_terminalData_t term = {0};

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: isAmountAvailable\n");

    term.transAmount = 4000.0;
    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: transAmount - 4000 \n");
    result = isAmountAvailable(&term, &account);
    fprintf(stderr, "Expected Result:%d\n", SERVER_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    term.transAmount = 8000.0;
    fprintf(stderr, "Test Case 2: transAmount - 8000 \n");
    fprintf(stderr, "Input Data:  \n");
    result = isAmountAvailable(&term, &account);
    fprintf(stderr, "Expected Result:%d\n", LOW_BALANCE);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}
void isBlockedAccountTest(void)
{
    EN_cardError_t result;
    ST_accountsDB_t account = {0};

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: isBlockedAccount\n");

    account.state = RUNNING;
    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: state - Running \n");
    result = isBlockedAccount(&account);
    fprintf(stderr, "Expected Result:%d\n", SERVER_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    account.state = BLOCKED;
    fprintf(stderr, "Test Case 2: \n");
    fprintf(stderr, "Input Data: state - bocked \n");
    result = isBlockedAccount(&account);
    fprintf(stderr, "Expected Result:%d\n", BLOCKED_ACCOUNT);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}
void isValidAccountTest(void)
{
    EN_cardError_t result;
    ST_cardData_t card = {0};
    ST_accountsDB_t account = {0};

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: isValidAccount\n");

    strncpy(card.primaryAccountNumber, "2314007066043425", 20);

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: PAN - 2314007066043425 \n");
    result = isValidAccount(&card, &account);
    fprintf(stderr, "Expected Result:%d\n", SERVER_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    strncpy(card.primaryAccountNumber, "1414007066043425", 20);
    fprintf(stderr, "Test Case 2: \n");
    fprintf(stderr, "Input Data: PAN - 1414007066043425 \n");
    result = isValidAccount(&card, &account);
    fprintf(stderr, "Expected Result:%d\n", ACCOUNT_NOT_FOUND);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}
void recieveTransactionDataTest(void)
{
    EN_cardError_t result;
    ST_transaction_t trans = {0};

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: recieveTransactionData\n");

    strncpy(trans.cardHolderData.primaryAccountNumber, "5989374615436851", 20);

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: PAN - 5989374615436851 \n");
    result = recieveTransactionData(&trans);
    fprintf(stderr, "Expected Result:%d\n", FRAUD_CARD);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    strncpy(trans.cardHolderData.primaryAccountNumber, "8989374615436851", 20);
    trans.terminalData.transAmount = 5000;
    fprintf(stderr, "Test Case 2: \n");
    fprintf(stderr, "Input Data: (PAN - 8989374615436851) -- (transAmount - 5000)  \n");
    result = recieveTransactionData(&trans);
    fprintf(stderr, "Expected Result:%d\n", DECLINED_INSUFFECIENT_FUND);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    strncpy(trans.cardHolderData.primaryAccountNumber, "5807007076043875", 20);
    // trans.terminalData.transAmount = 1000;
    fprintf(stderr, "Test Case 3: \n");
    fprintf(stderr, "Input Data: (PAN - 5807007076043875) -- blocked  \n");
    result = recieveTransactionData(&trans);
    fprintf(stderr, "Expected Result:%d\n", DECLINED_STOLEN_CARD);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    int tmp = nAccounts;
    nAccounts = 255;
    fprintf(stderr, "Test Case 3: \n");
    fprintf(stderr, "Input Data: (PAN - 8989374615436851) -- blocked  \n");
    result = recieveTransactionData(&trans);
    fprintf(stderr, "Expected Result:%d\n", INTERNAL_SERVER_ERROR);
    fprintf(stderr, "Actual Result:%d\n\n", result);
    nAccounts = tmp;
}