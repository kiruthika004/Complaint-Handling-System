#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"

void readLine(
    const char *prompt,
    char *buffer,
    size_t bufferSize
)
{
    size_t length;
    int character;

    if (buffer == NULL || bufferSize == 0U)
    {
        return;
    }

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, (int)bufferSize, stdin) == NULL)
        {
            buffer[0] = '\0';
            return;
        }

        length = strlen(buffer);

        if (length > 0U &&
            buffer[length - 1U] == '\n')
        {
            buffer[length - 1U] = '\0';
        }
        else
        {
            while ((character = getchar()) != '\n' &&
                   character != EOF)
            {
                /* Discard additional input. */
            }
        }

        if (buffer[0] != '\0')
        {
            return;
        }

        puts("Input cannot be empty.");
    }
}

int readInteger(
    const char *prompt,
    int minimum,
    int maximum
)
{
    char input[64];
    char *endPointer;
    long value;

    while (1)
    {
        readLine(prompt, input, sizeof(input));

        value = strtol(input, &endPointer, 10);

        if (*endPointer == '\0' &&
            value >= minimum &&
            value <= maximum)
        {
            return (int)value;
        }

        printf(
            "Enter a value between %d and %d.\n",
            minimum,
            maximum
        );
    }
}

int validateEmail(const char *email)
{
    const char *atSymbol;
    const char *dotSymbol;

    if (email == NULL ||
        strchr(email, ' ') != NULL)
    {
        return 0;
    }

    atSymbol = strchr(email, '@');

    if (atSymbol == NULL ||
        atSymbol == email)
    {
        return 0;
    }

    dotSymbol = strchr(atSymbol + 1, '.');

    return dotSymbol != NULL &&
           dotSymbol[1] != '\0';
}

int validatePhone(const char *phone)
{
    size_t index;
    size_t length;

    if (phone == NULL)
    {
        return 0;
    }

    length = strlen(phone);

    if (length < 8U || length > 15U)
    {
        return 0;
    }

    for (index = 0U; index < length; index++)
    {
        if (!isdigit((unsigned char)phone[index]))
        {
            return 0;
        }
    }

    return 1;
}

void getCurrentTimestamp(
    char *buffer,
    size_t bufferSize
)
{
    time_t currentTime;
    time_t istTime;
    struct tm *istDateTime;
    size_t result;

    if (buffer == NULL || bufferSize == 0U)
    {
        return;
    }

    currentTime = time(NULL);

    if (currentTime == (time_t)-1)
    {
        (void)snprintf(
            buffer,
            bufferSize,
            "Timestamp unavailable"
        );

        return;
    }

    /*
     * Indian Standard Time is UTC + 05:30.
     *
     * 5 hours  = 5 * 60 * 60 seconds
     * 30 mins  = 30 * 60 seconds
     * Total    = 19800 seconds
     */
    istTime = currentTime + (time_t)(5 * 60 * 60)
                          + (time_t)(30 * 60);

    /*
     * gmtime() is used because the IST offset
     * has already been added manually.
     */
    istDateTime = gmtime(&istTime);

    if (istDateTime == NULL)
    {
        (void)snprintf(
            buffer,
            bufferSize,
            "Timestamp unavailable"
        );

        return;
    }

    result = strftime(
        buffer,
        bufferSize,
        "%Y-%m-%d %H:%M:%S",
        istDateTime
    );

    if (result == 0U)
    {
        (void)snprintf(
            buffer,
            bufferSize,
            "Timestamp unavailable"
        );

        return;
    }

    /*
     * Append the fixed IST timezone indicator.
     */
    (void)snprintf(
        buffer + result,
        bufferSize - result,
        " +0530 IST"
    );
}
const char *getStatusText(
    ComplaintStatus status
)
{
    switch (status)
    {
        case STATUS_OPEN:
            return "Open";

        case STATUS_IN_PROGRESS:
            return "In Progress";

        case STATUS_ESCALATED:
            return "Escalated";

        case STATUS_RESOLVED:
            return "Resolved";

        case STATUS_CLOSED:
            return "Closed";

        default:
            return "Unknown";
    }
}

const char *getPriorityText(
    ComplaintPriority priority
)
{
    switch (priority)
    {
        case PRIORITY_LOW:
            return "Low";

        case PRIORITY_MEDIUM:
            return "Medium";

        case PRIORITY_HIGH:
            return "High";

        case PRIORITY_CRITICAL:
            return "Critical";

        default:
            return "Unknown";
    }
}

const char *getCategoryText(
    ComplaintCategory category
)
{
    switch (category)
    {
        case CATEGORY_DAMAGED_PRODUCT:
            return "Damaged Product";

        case CATEGORY_WRONG_DELIVERY:
            return "Wrong Delivery";

        case CATEGORY_MISSING_ITEM:
            return "Missing Item";

        case CATEGORY_DELIVERY_DELAY:
            return "Delivery Delay";

        case CATEGORY_PAYMENT_ISSUE:
            return "Payment Issue";

        case CATEGORY_REFUND_DELAY:
            return "Refund Delay";

        case CATEGORY_CANCELLATION_ISSUE:
            return "Cancellation Issue";

        case CATEGORY_PRODUCT_QUALITY:
            return "Product Quality";

        case CATEGORY_OTHER:
            return "Other";

        default:
            return "Unknown";
    }
}

void pauseScreen(void)
{
    char input[8];

    printf("\nPress Enter to continue...");
    (void)fgets(input, sizeof(input), stdin);
}