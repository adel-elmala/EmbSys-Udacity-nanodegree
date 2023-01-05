#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    char *line = NULL;
    size_t len = 0;
    size_t nread;
    fprintf(stdout, "Please enter your name: ");
    nread = getline(&line, &len, stdin);
    if ((nread <= 0) ||
        (nread <= 21) ||
        (nread >= 25))
    {
        free(line);
        return WRONG_NAME;
    }
    else
    {
        memcpy(cardData->cardHolderName, line, nread - 1);
        (cardData->cardHolderName)[nread - 1] = 0;
    }
    free(line);
    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    char *line = NULL;
    size_t len = 0;
    size_t nread;
    char month[3] = {0};
    char year[3] = {0};
    fprintf(stdout, "Please enter your card expiry date in MM/YY format: ");
    nread = getline(&line, &len, stdin);
    if ((nread != 6) ||
        (line[2] != '/'))
    {
        free(line);
        return WRONG_EXP_DATE;
    }
    else
    {
        memcpy(month, line, 2);
        // memcpy(year, line+3, 2);
        int mm = atoi(month);
        // int yy = atoi(year);
        if (mm < 1 || mm > 12)
        {
            free(line);
            return WRONG_EXP_DATE;
        }
        memcpy(cardData->cardExpirationDate, line, nread - 1);
        (cardData->cardExpirationDate)[nread - 1] = 0;
    }
    free(line);
    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    char *line = NULL;
    size_t len = 0;
    size_t nread;
    fprintf(stdout, "Please enter your card Primary Account Number: ");
    nread = getline(&line, &len, stdin);
    if ((nread > 20 || nread < 17))
    {
        free(line);
        return WRONG_PAN;
    }
    else
    {
        memcpy(cardData->primaryAccountNumber, line, nread - 1);
        (cardData->primaryAccountNumber)[nread - 1] = 0;
    }
    free(line);
    return CARD_OK;
}

void getCardExpiryDateTest(void)
{
    EN_cardError_t result;
    ST_cardData_t card = {0};
    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: getCardExpiryDate\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data:02/23\n");
    result = getCardExpiryDate(&card);
    fprintf(stderr, "Expected Result:%d\n", CARD_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 2: \n");
    fprintf(stderr, "Input Data:99/99 \n");
    result = getCardExpiryDate(&card);
    fprintf(stderr, "Expected Result:%d\n", WRONG_EXP_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 3: \n");
    fprintf(stderr, "Input Data:04-99 \n");
    result = getCardExpiryDate(&card);
    fprintf(stderr, "Expected Result:%d\n", WRONG_EXP_DATE);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}

void getCardHolderNameTest(void)
{
    EN_cardError_t result;
    ST_cardData_t card = {0};

    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: getCardHolderName\n\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data:Adel Refat Ali Mohammed\n");
    result = getCardHolderName(&card);
    fprintf(stderr, "Expected Result:%d\n", CARD_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 2: \n");
    fprintf(stderr, "Input Data:Adel Refat \n");
    result = getCardHolderName(&card);
    fprintf(stderr, "Expected Result:%d\n", WRONG_NAME);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 3: \n");
    fprintf(stderr, "Input Data:Adel Refat Ali Mohammed Adel Refat Ali Mohammed \n");
    result = getCardHolderName(&card);
    fprintf(stderr, "Expected Result:%d\n", WRONG_NAME);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}

void getCardPANTest(void)
{
    EN_cardError_t result;
    ST_cardData_t card = {0};
    fprintf(stderr, "Tester Name: Adel Refaat\n");
    fprintf(stderr, "Function Name: getCardPAN\n");

    fprintf(stderr, "Test Case 1: \n");
    fprintf(stderr, "Input Data:1234567890123456789\n"); // 19 numbers
    result = getCardPAN(&card);
    fprintf(stderr, "Expected Result:%d\n", CARD_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 2: \n"); // 16 numbers
    fprintf(stderr, "Input Data:1234567890123456 \n");
    result = getCardPAN(&card);
    fprintf(stderr, "Expected Result:%d\n", CARD_OK);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 3: \n");
    fprintf(stderr, "Input Data:12345678901234567890123 \n"); // 24 numbers
    result = getCardPAN(&card);
    fprintf(stderr, "Expected Result:%d\n", WRONG_PAN);
    fprintf(stderr, "Actual Result:%d\n\n", result);

    memset(&card, 0, sizeof(card));
    fprintf(stderr, "Test Case 4: \n");
    fprintf(stderr, "Input Data:1234567890 \n"); // 10 numbers
    result = getCardPAN(&card);
    fprintf(stderr, "Expected Result:%d\n", WRONG_PAN);
    fprintf(stderr, "Actual Result:%d\n\n", result);
}
