#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define COMPLAINT_FILE  "complaints.dat"
#define HISTORY_FILE    "history.dat"
#define COUNTER_FILE    "counters.dat"
#define AUDIT_FILE      "audit.log"
#define APPLICATION_LOG "application.log"

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "EcomAdmin@2026"

/*
 * Replace these values with the
 * client-approved helpdesk details.
 */
#define HELPDESK_NUMBER "1800-123-0127"
#define HELPDESK_EMAIL  "helpdesk@techsolutions.com"
#define TIMESTAMP_LENGTH 40
#define NAME_LENGTH         50
#define CONTACT_LENGTH      100
#define ID_LENGTH           30
#define TITLE_LENGTH        100
#define DESCRIPTION_LENGTH  500
#define REMARK_LENGTH       300
#define RESOLUTION_LENGTH   500

/*
 * Increased to support:
 * YYYY-MM-DD HH:MM:SS +0530
 */

typedef enum
{
    STATUS_OPEN = 1,
    STATUS_IN_PROGRESS,
    STATUS_ESCALATED,
    STATUS_RESOLVED,
    STATUS_CLOSED
} ComplaintStatus;

typedef enum
{
    PRIORITY_LOW = 1,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH,
    PRIORITY_CRITICAL
} ComplaintPriority;

typedef enum
{
    CATEGORY_DAMAGED_PRODUCT = 1,
    CATEGORY_WRONG_DELIVERY,
    CATEGORY_MISSING_ITEM,
    CATEGORY_DELIVERY_DELAY,
    CATEGORY_PAYMENT_ISSUE,
    CATEGORY_REFUND_DELAY,
    CATEGORY_CANCELLATION_ISSUE,
    CATEGORY_PRODUCT_QUALITY,
    CATEGORY_OTHER
} ComplaintCategory;

typedef struct
{
    unsigned int complaintId;

    char customerName[NAME_LENGTH];
    char email[CONTACT_LENGTH];
    char phone[20];

    char orderId[ID_LENGTH];
    char productId[ID_LENGTH];

    ComplaintCategory category;
    char title[TITLE_LENGTH];
    char description[DESCRIPTION_LENGTH];

    ComplaintPriority priority;
    ComplaintStatus status;

    unsigned int escalationLevel;
    char escalationReason[REMARK_LENGTH];
    char adminRemark[REMARK_LENGTH];
    char resolution[RESOLUTION_LENGTH];

    char createdAt[TIMESTAMP_LENGTH];
    char updatedAt[TIMESTAMP_LENGTH];
    char resolvedAt[TIMESTAMP_LENGTH];
    char closedAt[TIMESTAMP_LENGTH];
} Complaint;

typedef struct ComplaintNode
{
    Complaint data;
    struct ComplaintNode *next;
} ComplaintNode;

typedef struct
{
    unsigned int historyId;
    unsigned int complaintId;

    char action[50];
    char details[REMARK_LENGTH];
    char timestamp[TIMESTAMP_LENGTH];
} ComplaintHistory;

typedef struct HistoryNode
{
    ComplaintHistory data;
    struct HistoryNode *next;
} HistoryNode;

typedef struct
{
    unsigned int lastComplaintId;
    unsigned int lastHistoryId;
} CounterRecord;

typedef struct
{
    ComplaintNode *complaintHead;
    HistoryNode *historyHead;
    CounterRecord counters;
} ApplicationContext;

void readLine(
    const char *prompt,
    char *buffer,
    size_t bufferSize
);

int readInteger(
    const char *prompt,
    int minimum,
    int maximum
);

int validateEmail(const char *email);
int validatePhone(const char *phone);

void getCurrentTimestamp(
    char *buffer,
    size_t bufferSize
);

const char *getStatusText(
    ComplaintStatus status
);

const char *getPriorityText(
    ComplaintPriority priority
);

const char *getCategoryText(
    ComplaintCategory category
);

void pauseScreen(void);

#endif