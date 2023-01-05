#include "terminal.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    char cardMM[3] = {0};
    char cardYY[3] = {0};
    char MM[3] = {0};
    char YY[3] = {0};
    memcpy(cardMM, cardData->cardExpirationDate, 2);
    memcpy(cardYY, (cardData->cardExpirationDate) + 3, 2);
    memcpy(MM, (termData->transactionDate) + 3, 2);
    memcpy(YY, (termData->transactionDate) + 8, 2); // dd/mm/yyyy
    int cmm, cyy, mm, yy;
    cmm = atoi(cardMM);
    cyy = atoi(cardYY);
    mm = atoi(MM);
    yy = atoi(YY);
    if ((yy >= cyy) && (mm > cmm))
    {
        // printf("EXPIRED_CARD\n");
        return EXPIRED_CARD;
    }
    else
    {
        // printf("TERMINAL_OK\n");
        return TERMINAL_OK;
    }
}

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{

    char *line = NULL;
    size_t len = 0;
    size_t nread;
    fprintf(stdout, "Please enter the transaction date (dd/mm/yyyy): ");
    nread = getline(&line, &len, stdin);
    if ((nread != 11) ||
        (line[2] != '/') ||
        (line[5] != '/'))
    {
        free(line);
        return WRONG_DATE;
    }

    else
    {
        char day_s[3] = {0};
        char month_s[3] = {0};
        char year_s[5] = {0};
        memcpy(day_s, line, 2);
        memcpy(month_s, line + 3, 2);
        memcpy(year_s, line + 6, 4);
        int dd = atoi(day_s);
        int mm = atoi(month_s);
        int yyyy = atoi(year_s);
        if ((dd < 1) ||
            (dd > 31) ||
            (mm < 1) ||
            (mm > 12) ||
            (yyyy < 2000))
        {
            free(line);
            return WRONG_DATE;
        }

        memcpy(termData->transactionDate, line, nread - 1);
        (termData->transactionDate)[nread - 1] = 0;
    }
    free(line);
    return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{

    char *line = NULL;
    size_t len = 0;
    size_t nread;
    fprintf(stdout, "Please enter the transaction amount: ");
    nread = getline(&line, &len, stdin);
    if ((nread < 0))
    {
        free(line);
        return WRONG_DATE;
    }

    else
    {
        float amount = atof(line);
        if (amount <= 0)
        {
            free(line);
            return INVALID_AMOUNT;
        }

        termData->transAmount = amount;
    }
    free(line);
    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    if (termData->transAmount <= termData->maxTransAmount)
        return TERMINAL_OK;
    else
        return EXCEED_MAX_AMOUNT;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    if (maxAmount <= 0)
        return INVALID_MAX_AMOUNT;
    else
    {
        termData->maxTransAmount = maxAmount;
        return TERMINAL_OK;
    }
}

void getTransactionDateTest(void)
{
    EN_cardError_t result;
    ST_terminalData_t term = {0};
    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: getTransactionDate\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data:25/06/2022\n");
    result = getTransactionDate(&term);
    fprintf(stderr, "Expected Result:%d\n", TERMINAL_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&term, 0, sizeof(term));
    fprintf(stderr, "Test Case 2: \n"); // wrong day
    fprintf(stderr, "Input Data:50/06/2022 \n");
    result = getTransactionDate(&term);
    fprintf(stderr, "Expected Result:%d\n", WRONG_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&term, 0, sizeof(term));
    fprintf(stderr, "Test Case 3: \n");
    fprintf(stderr, "Input Data:25/80/2022 \n"); // wrong month
    result = getTransactionDate(&term);
    fprintf(stderr, "Expected Result:%d\n", WRONG_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&term, 0, sizeof(term));
    fprintf(stderr, "Test Case 4: \n");
    fprintf(stderr, "Input Data:25/08/1990 \n"); // wrong year
    result = getTransactionDate(&term);
    fprintf(stderr, "Expected Result:%d\n", WRONG_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&term, 0, sizeof(term));
    fprintf(stderr, "Test Case 5: \n");
    fprintf(stderr, "Input Data:05-06-2022 \n"); // wrong format
    result = getTransactionDate(&term);
    fprintf(stderr, "Expected Result:%d\n", WRONG_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&term, 0, sizeof(term));
    fprintf(stderr, "Test Case 6: \n");
    fprintf(stderr, "Input Data:12547899554 \n"); // wrong format
    result = getTransactionDate(&term);
    fprintf(stderr, "Expected Result:%d\n", WRONG_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}

void isBelowMaxAmountTest(void)
{
    EN_cardError_t result;
    ST_terminalData_t term = {0};
    term.maxTransAmount = 5000.0;
    term.transAmount = 1000.0;

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: isBelowMaxAmount\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: 1000 transaction : 5000 max\n");
    result = isBelowMaxAmount(&term);
    fprintf(stderr, "Expected Result:%d\n", TERMINAL_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    term.transAmount = 6000.0;
    fprintf(stderr, "Test Case 2: \n"); // EXCEED_MAX_AMOUNT
    fprintf(stderr, "Input Data: 6000 transaction : 5000 max \n");
    result = isBelowMaxAmount(&term);
    fprintf(stderr, "Expected Result:%d\n", EXCEED_MAX_AMOUNT);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}
void setMaxAmountTest(void)
{
    EN_cardError_t result;
    ST_terminalData_t term = {0};
    // term.maxTransAmount = 5000.0;
    // term.transAmount = 1000.0;

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: setMaxAmount\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data:  1000 \n");
    result = setMaxAmount(&term, 1000);
    fprintf(stderr, "Expected Result:%d\n", TERMINAL_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    fprintf(stderr, "Test Case 2: \n"); // INVALID_MAX_AMOUNT
    fprintf(stderr, "Input Data: 0  \n");
    result = setMaxAmount(&term, 0);
    fprintf(stderr, "Expected Result:%d\n", INVALID_MAX_AMOUNT);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}

void getTransactionAmountTest(void)
{
    EN_cardError_t result;
    ST_terminalData_t term = {0};
    // strncpy(term.transactionDate, "25/01/2022", 10);

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: getTransactionAmount\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: 1000.0\n");
    result = getTransactionAmount(&term);
    fprintf(stderr, "Expected Result:%d\n", TERMINAL_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    fprintf(stderr, "Test Case 2: \n"); // negative
    fprintf(stderr, "Input Data: -100 \n");
    result = getTransactionAmount(&term);
    fprintf(stderr, "Expected Result:%d\n", INVALID_AMOUNT);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    fprintf(stderr, "Test Case 3: \n"); // zero
    fprintf(stderr, "Input Data: 0 \n");
    result = getTransactionAmount(&term);
    fprintf(stderr, "Expected Result:%d\n", INVALID_AMOUNT);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}
void isCardExpriedTest(void)
{

    EN_cardError_t result;
    ST_cardData_t card = {0};
    strncpy(card.cardExpirationDate, "02/23", 5);
    ST_terminalData_t term = {0};
    strncpy(term.transactionDate, "25/01/2022", 10);

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: isCardExpriedTest\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data: 02/23 , 25/01/2022\n");
    result = isCardExpired(&card, &term);
    fprintf(stderr, "Expected Result:%d\n", TERMINAL_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    strncpy(term.transactionDate, "25/03/2023", 10);

    fprintf(stderr, "Test Case 2: \n"); // passed month
    fprintf(stderr, "Input Data: 02/23 , 25/03/2023 \n");
    result = isCardExpired(&card, &term);
    fprintf(stderr, "Expected Result:%d\n", EXPIRED_CARD);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}