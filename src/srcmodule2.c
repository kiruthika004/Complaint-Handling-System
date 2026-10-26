#include <stdio.h>
#include <time.h>
#include "module2.h"
#include <stdlib.h>
#include <string.h>

#include "module1.h"



/* Private helper function declaration */
static long getEscalationLimitSeconds(ComplaintPriority priority);
/* =========================================================
 * INTERNAL HELPER FUNCTIONS
 * ========================================================= */
static int appendComplaintNode(
    ApplicationContext *app,
    const Complaint *complaint
)
{
    ComplaintNode *newNode;
    ComplaintNode **current;

    if (app == NULL || complaint == NULL)
    {
        return 0;
    }

    newNode = malloc(sizeof(*newNode));

    if (newNode == NULL)
    {
        writeApplicationLog(
            "APPEND_COMPLAINT",
            "Memory allocation failed"
        );

        return 0;
    }

    newNode->data = *complaint;
    newNode->next = NULL;

    current = &app->complaintHead;

    while (*current != NULL)
    {
        current = &(*current)->next;
    }

    *current = newNode;

    return 1;
}

static int appendHistoryNode(
    ApplicationContext *app,
    const ComplaintHistory *history
)
{
    HistoryNode *newNode;
    HistoryNode **current;

    if (app == NULL || history == NULL)
    {
        return 0;
    }

    newNode = malloc(sizeof(*newNode));

    if (newNode == NULL)
    {
        writeApplicationLog(
            "APPEND_HISTORY",
            "Memory allocation failed"
        );

        return 0;
    }

    newNode->data = *history;
    newNode->next = NULL;

    current = &app->historyHead;

    while (*current != NULL)
    {
        current = &(*current)->next;
    }

    *current = newNode;

    return 1;
}

static int isValidComplaintRecord(
    const Complaint *complaint
)
{
    if (complaint == NULL)
    {
        return 0;
    }

    if (complaint->complaintId == 0U)
    {
        return 0;
    }

    if (complaint->category < CATEGORY_DAMAGED_PRODUCT ||
        complaint->category > CATEGORY_OTHER)
    {
        return 0;
    }

    if (complaint->priority < PRIORITY_LOW ||
        complaint->priority > PRIORITY_CRITICAL)
    {
        return 0;
    }

    if (complaint->status < STATUS_OPEN ||
        complaint->status > STATUS_CLOSED)
    {
        return 0;
    }

    return 1;
}

static int saveComplaintUpdate(
    ApplicationContext *app,
    ComplaintNode *node,
    const char *historyAction,
    const char *historyDetails,
    const char *auditAction
)
{
    if (app == NULL ||
        node == NULL ||
        historyAction == NULL ||
        historyDetails == NULL ||
        auditAction == NULL)
    {
        return 0;
    }

    getCurrentTimestamp(
        node->data.updatedAt,
        sizeof(node->data.updatedAt)
    );

    if (!saveComplaints(app))
    {
        writeApplicationLog(
            auditAction,
            "Unable to save complaint changes"
        );

        writeComplaintAuditLog(
            "ADMIN",
            auditAction,
            node->data.complaintId,
            "FAILED"
        );

        return 0;
    }

    if (!addComplaintHistory(
            app,
            node->data.complaintId,
            historyAction,
            historyDetails
        ))
    {
        writeApplicationLog(
            auditAction,
            "Complaint updated but history could not be saved"
        );
    }

    writeComplaintAuditLog(
        "ADMIN",
        auditAction,
        node->data.complaintId,
        "SUCCESS"
    );

    return 1;
}
static long getEscalationLimitSeconds(
    ComplaintPriority priority
)
{
    /*switch (priority)
    {
        case PRIORITY_LOW:
            return LOW_ESCALATION_HOURS *
                   SECONDS_PER_HOUR;

        case PRIORITY_MEDIUM:
            return MEDIUM_ESCALATION_HOURS *
                   SECONDS_PER_HOUR;

        case PRIORITY_HIGH:
            return HIGH_ESCALATION_HOURS *
                   SECONDS_PER_HOUR;

        case PRIORITY_CRITICAL:
            return CRITICAL_ESCALATION_HOURS *
                   SECONDS_PER_HOUR;

        default:
            return MEDIUM_ESCALATION_HOURS *
                   SECONDS_PER_HOUR;
    }*/
    (void)priority;

    return 60L;
}
/* =========================================================
 * ADMIN AUTHENTICATION
 * ========================================================= */

int authenticateAdmin(
    const char *username,
    const char *password
)
{
    if (username == NULL || password == NULL)
    {
        return 0;
    }

    return strcmp(username, ADMIN_USERNAME) == 0 &&
           strcmp(password, ADMIN_PASSWORD) == 0;
}

/* =========================================================
 * STATUS WORKFLOW
 * ========================================================= */

int isValidStatusTransition(
    ComplaintStatus currentStatus,
    ComplaintStatus newStatus
)
{
    if (currentStatus == STATUS_OPEN)
    {
        return newStatus == STATUS_IN_PROGRESS ||
               newStatus == STATUS_ESCALATED;
    }

    if (currentStatus == STATUS_IN_PROGRESS)
    {
        return newStatus == STATUS_ESCALATED ||
               newStatus == STATUS_RESOLVED;
    }

    if (currentStatus == STATUS_ESCALATED)
    {
        return newStatus == STATUS_IN_PROGRESS ||
               newStatus == STATUS_RESOLVED;
    }

    if (currentStatus == STATUS_RESOLVED)
    {
        return newStatus == STATUS_CLOSED;
    }

    return 0;
}

int updateComplaintPriority(
    ApplicationContext *app,
    unsigned int complaintId,
    ComplaintPriority newPriority
)
{
    ComplaintNode *node;

    if (app == NULL ||
        newPriority < PRIORITY_LOW ||
        newPriority > PRIORITY_CRITICAL)
    {
        return 0;
    }

    node = findComplaintById(
        app,
        complaintId
    );

    if (node == NULL ||
        node->data.status == STATUS_CLOSED)
    {
        return 0;
    }

    node->data.priority = newPriority;

    return saveComplaintUpdate(
        app,
        node,
        "Priority Updated",
        getPriorityText(newPriority),
        "PRIORITY_UPDATE"
    );
}

int updateComplaintStatus(
    ApplicationContext *app,
    unsigned int complaintId,
    ComplaintStatus newStatus
)
{
    ComplaintNode *node;

    if (app == NULL)
    {
        return 0;
    }

    node = findComplaintById(
        app,
        complaintId
    );

    if (node == NULL)
    {
        return 0;
    }

    if (!isValidStatusTransition(
            node->data.status,
            newStatus
        ))
    {
        writeComplaintAuditLog(
            "ADMIN",
            "STATUS_UPDATE",
            complaintId,
            "INVALID_TRANSITION"
        );

        return 0;
    }

    /*
     * Resolution and closure require additional
     * information, so they use separate functions.
     */
    if (newStatus == STATUS_RESOLVED ||
        newStatus == STATUS_CLOSED)
    {
        return 0;
    }

    node->data.status = newStatus;
    node->data.statusUpdatedEpoch =
    time(NULL);
    return saveComplaintUpdate(
        app,
        node,
        "Status Updated",
        getStatusText(newStatus),
        "STATUS_UPDATE"
    );
}

int addAdminRemark(
    ApplicationContext *app,
    unsigned int complaintId,
    const char *remark
)
{
    ComplaintNode *node;

    if (app == NULL ||
        remark == NULL ||
        remark[0] == '\0')
    {
        return 0;
    }

    node = findComplaintById(
        app,
        complaintId
    );

    if (node == NULL ||
        node->data.status == STATUS_CLOSED)
    {
        return 0;
    }

    (void)snprintf(
        node->data.adminRemark,
        sizeof(node->data.adminRemark),
        "%s",
        remark
    );

    return saveComplaintUpdate(
        app,
        node,
        "Remark Added",
        remark,
        "REMARK_ADDED"
    );
}

int escalateComplaint(
    ApplicationContext *app,
    unsigned int complaintId,
    unsigned int escalationLevel,
    const char *reason
)
{
    ComplaintNode *node;

    if (app == NULL ||
        reason == NULL ||
        reason[0] == '\0' ||
        escalationLevel < 1U ||
        escalationLevel > 3U)
    {
        return 0;
    }

    node = findComplaintById(
        app,
        complaintId
    );

    if (node == NULL ||
        node->data.status == STATUS_RESOLVED ||
        node->data.status == STATUS_CLOSED)
    {
        return 0;
    }

    node->data.status = STATUS_ESCALATED;
    node->data.escalationLevel = escalationLevel;
    node->data.statusUpdatedEpoch = time(NULL);
    if (node->data.priority < PRIORITY_HIGH)
    {
        node->data.priority = PRIORITY_HIGH;
    }

    (void)snprintf(
        node->data.escalationReason,
        sizeof(node->data.escalationReason),
        "%s",
        reason
    );

    return saveComplaintUpdate(
        app,
        node,
        "Escalated",
        reason,
        "COMPLAINT_ESCALATED"
    );
}

int resolveComplaint(
    ApplicationContext *app,
    unsigned int complaintId,
    const char *resolution
)
{
    ComplaintNode *node;

    if (app == NULL ||
        resolution == NULL ||
        resolution[0] == '\0')
    {
        return 0;
    }

    node = findComplaintById(
        app,
        complaintId
    );

    if (node == NULL)
    {
        return 0;
    }

    if (node->data.status != STATUS_IN_PROGRESS &&
        node->data.status != STATUS_ESCALATED)
    {
        return 0;
    }

    (void)snprintf(
        node->data.resolution,
        sizeof(node->data.resolution),
        "%s",
        resolution
    );

    node->data.status = STATUS_RESOLVED;
    node->data.statusUpdatedEpoch =
    time(NULL);
    getCurrentTimestamp(
        node->data.resolvedAt,
        sizeof(node->data.resolvedAt)
    );

    return saveComplaintUpdate(
        app,
        node,
        "Resolved",
        resolution,
        "COMPLAINT_RESOLVED"
    );
}

int closeComplaint(
    ApplicationContext *app,
    unsigned int complaintId
)
{
    ComplaintNode *node;

    if (app == NULL)
    {
        return 0;
    }

    node = findComplaintById(
        app,
        complaintId
    );

    if (node == NULL ||
        node->data.status != STATUS_RESOLVED ||
        node->data.resolution[0] == '\0')
    {
        return 0;
    }

    node->data.status = STATUS_CLOSED;
    node->data.status = STATUS_CLOSED;

    node->data.statusUpdatedEpoch = time(NULL);
    getCurrentTimestamp(
        node->data.closedAt,
        sizeof(node->data.closedAt)
    );

    return saveComplaintUpdate(
        app,
        node,
        "Closed",
        "Complaint lifecycle completed",
        "COMPLAINT_CLOSED"
    );
}
int checkAndEscalateOverdueComplaints(
    ApplicationContext *app
)
{
    ComplaintNode *current;
    time_t currentTime;
    double elapsedSeconds;
    long allowedSeconds;
    int escalatedCount;

    if (app == NULL)
    {
        return 0;
    }

    currentTime = time(NULL);

    if (currentTime == (time_t)-1)
    {
        writeApplicationLog(
            "AUTO_ESCALATION",
            "Unable to obtain current system time"
        );

        return 0;
    }

    escalatedCount = 0;
    current = app->complaintHead;

    while (current != NULL)
    {
        /*
         * Only unresolved Open or In Progress
         * complaints are checked.
         */
        if (current->data.status == STATUS_OPEN ||
            current->data.status ==
                STATUS_IN_PROGRESS)
        {
            /*
             * Handle old or incomplete records
             * with no stored status time.
             */
            if (current->data.statusUpdatedEpoch ==
                (time_t)0)
            {
                current->data.statusUpdatedEpoch =
                    currentTime;

                current = current->next;
                continue;
            }

            allowedSeconds =
                getEscalationLimitSeconds(
                    current->data.priority
                );

            elapsedSeconds = difftime(
                currentTime,
                current->data.statusUpdatedEpoch
            );

            if (elapsedSeconds >=
                (double)allowedSeconds)
            {
                current->data.status =
                    STATUS_ESCALATED;

                current->data.escalationLevel =
                    1U;

                (void)snprintf(
                    current->data.escalationReason,
                    sizeof(
                        current->data.escalationReason
                    ),
                    "%s",
                    "Automatically escalated "
                    "because the configured "
                    "processing duration was exceeded"
                );

                getCurrentTimestamp(
                    current->data.updatedAt,
                    sizeof(
                        current->data.updatedAt
                    )
                );

                current->data.statusUpdatedEpoch =
                    currentTime;

                (void)addComplaintHistory(
                    app,
                    current->data.complaintId,
                    "Automatically Escalated",
                    current->data.escalationReason
                );

                writeComplaintAuditLog(
                    "SYSTEM",
                    "AUTO_ESCALATION",
                    current->data.complaintId,
                    "SUCCESS"
                );

                escalatedCount++;
            }
        }

        current = current->next;
    }

    if (escalatedCount > 0)
    {
        if (!saveComplaints(app))
        {
            writeApplicationLog(
                "AUTO_ESCALATION",
                "Unable to save escalated complaints"
            );

            return 0;
        }
    }

    return escalatedCount;
}
/* =========================================================
 * COMPLAINT HISTORY
 * ========================================================= */

int addComplaintHistory(
    ApplicationContext *app,
    unsigned int complaintId,
    const char *action,
    const char *details
)
{
    HistoryNode *newNode;
    HistoryNode **current;

    if (app == NULL ||
        action == NULL ||
        details == NULL)
    {
        return 0;
    }

    newNode = calloc(
        1U,
        sizeof(*newNode)
    );

    if (newNode == NULL)
    {
        writeApplicationLog(
            "ADD_HISTORY",
            "Memory allocation failed"
        );

        return 0;
    }

    app->counters.lastHistoryId++;

    newNode->data.historyId =
        app->counters.lastHistoryId;

    newNode->data.complaintId =
        complaintId;

    (void)snprintf(
        newNode->data.action,
        sizeof(newNode->data.action),
        "%s",
        action
    );

    (void)snprintf(
        newNode->data.details,
        sizeof(newNode->data.details),
        "%s",
        details
    );

    getCurrentTimestamp(
        newNode->data.timestamp,
        sizeof(newNode->data.timestamp)
    );

    current = &app->historyHead;

    while (*current != NULL)
    {
        current = &(*current)->next;
    }

    *current = newNode;

    if (!saveHistory(app) ||
        !saveCounters(app))
    {
        writeApplicationLog(
            "ADD_HISTORY",
            "Unable to save history record"
        );

        return 0;
    }

    return 1;
}

void displayComplaintHistory(
    const ApplicationContext *app,
    unsigned int complaintId
)
{
    const HistoryNode *current;
    int found;

    if (app == NULL)
    {
        return;
    }

    found = 0;
    current = app->historyHead;

    puts(
        "\n========== COMPLAINT HISTORY =========="
    );

    while (current != NULL)
    {
        if (current->data.complaintId ==
            complaintId)
        {
            found = 1;

            printf(
                "[%s] %s: %s\n",
                current->data.timestamp,
                current->data.action,
                current->data.details
            );
        }

        current = current->next;
    }

    if (!found)
    {
        puts(
            "No history is available "
            "for this Complaint ID."
        );
    }
}

/* =========================================================
 * SUMMARY REPORT
 * ========================================================= */
void generateSummaryReport(
    const ApplicationContext *app
)
{
    int statusCount[6] = {0};
    int priorityCount[5] = {0};
    int categoryCount[10] = {0};

    int total;
    int pendingCount;
    int completedCount;
    int pendingHighCount;
    int pendingCriticalCount;

    int totalRating;
    int ratingCount;
    double averageRating;

    int index;

    const ComplaintNode *current;

    if (app == NULL)
    {
        return;
    }

    total = 0;
    pendingCount = 0;
    completedCount = 0;
    pendingHighCount = 0;
    pendingCriticalCount = 0;

    totalRating = 0;
    ratingCount = 0;
    averageRating = 0.0;

    current = app->complaintHead;

    while (current != NULL)
    {
        total++;

        if (current->data.status >= STATUS_OPEN &&
            current->data.status <= STATUS_CLOSED)
        {
            statusCount[current->data.status]++;
        }

        if (current->data.priority >= PRIORITY_LOW &&
            current->data.priority <= PRIORITY_CRITICAL)
        {
            priorityCount[current->data.priority]++;
        }

        if (current->data.category >=
                CATEGORY_DAMAGED_PRODUCT &&
            current->data.category <=
                CATEGORY_OTHER)
        {
            categoryCount[current->data.category]++;
        }

        if (current->data.status == STATUS_OPEN ||
            current->data.status == STATUS_IN_PROGRESS ||
            current->data.status == STATUS_ESCALATED)
        {
            pendingCount++;

            if (current->data.priority ==
                PRIORITY_HIGH)
            {
                pendingHighCount++;
            }

            if (current->data.priority ==
                PRIORITY_CRITICAL)
            {
                pendingCriticalCount++;
            }
        }
        else
        {
            completedCount++;
        }

        /* Rating Calculation */

        if (current->data.customerRating >= 1 &&
            current->data.customerRating <= 5)
        {
            totalRating +=
                current->data.customerRating;

            ratingCount++;
        }

        current = current->next;
    }

    puts("\n========== SUMMARY REPORT ==========");

    printf(
        "Total Complaints       : %d\n",
        total
    );

    printf(
        "Pending Complaints     : %d\n",
        pendingCount
    );

    printf(
        "Completed Complaints   : %d\n",
        completedCount
    );

    printf(
        "Pending High Priority  : %d\n",
        pendingHighCount
    );

    printf(
        "Pending Critical       : %d\n",
        pendingCriticalCount
    );

    puts("\nBy Status");

    for (index = STATUS_OPEN;
         index <= STATUS_CLOSED;
         index++)
    {
        printf(
            "%-14s : %d\n",
            getStatusText(
                (ComplaintStatus)index
            ),
            statusCount[index]
        );
    }

    puts("\nBy Priority");

    for (index = PRIORITY_LOW;
         index <= PRIORITY_CRITICAL;
         index++)
    {
        printf(
            "%-14s : %d\n",
            getPriorityText(
                (ComplaintPriority)index
            ),
            priorityCount[index]
        );
    }

    puts("\nBy Category");

    for (index = CATEGORY_DAMAGED_PRODUCT;
         index <= CATEGORY_OTHER;
         index++)
    {
        printf(
            "%-22s : %d\n",
            getCategoryText(
                (ComplaintCategory)index
            ),
            categoryCount[index]
        );
    }

    puts("\nCustomer Satisfaction");

    if (ratingCount > 0)
    {
        averageRating =
            (double)totalRating /
            ratingCount;

        printf(
            "Average Customer Rating : %.2f / 5\n",
            averageRating
        );

        printf(
            "Total Ratings Received  : %d\n",
            ratingCount
        );
    }
    else
    {
        puts(
            "Average Customer Rating : N/A"
        );
    }
}

/* =========================================================
 * APPLICATION INITIALIZATION
 * ========================================================= */

int initializeApplication(
    ApplicationContext *app
)
{
    FILE *file;
    Complaint complaint;
    ComplaintHistory history;

    if (app == NULL)
    {
        return 0;
    }

    (void)memset(
        app,
        0,
        sizeof(*app)
    );

    /*
     * Load ID counters.
     */
    file = fopen(COUNTER_FILE, "rb");

    if (file != NULL)
    {
        if (fread(
                &app->counters,
                sizeof(app->counters),
                1U,
                file
            ) != 1U)
        {
            app->counters.lastComplaintId =
                1000U;

            app->counters.lastHistoryId =
                0U;
        }

        (void)fclose(file);
    }
    else
    {
        app->counters.lastComplaintId =
            1000U;

        app->counters.lastHistoryId =
            0U;
    }

    /*
     * Load complaint records.
     */
    file = fopen(COMPLAINT_FILE, "rb");

    if (file != NULL)
    {
        while (fread(
                   &complaint,
                   sizeof(complaint),
                   1U,
                   file
               ) == 1U)
        {
            if (!isValidComplaintRecord(
                    &complaint
                ))
            {
                writeApplicationLog(
                    "LOAD_COMPLAINTS",
                    "Invalid complaint record skipped"
                );

                continue;
            }

            if (!appendComplaintNode(
                    app,
                    &complaint
                ))
            {
                (void)fclose(file);
                freeApplicationData(app);
                return 0;
            }
        }

        if (ferror(file))
        {
            writeApplicationLog(
                "LOAD_COMPLAINTS",
                "Error while reading complaint file"
            );
        }

        (void)fclose(file);
    }

    /*
     * Load complaint-history records.
     */
    file = fopen(HISTORY_FILE, "rb");

    if (file != NULL)
    {
        while (fread(
                   &history,
                   sizeof(history),
                   1U,
                   file
               ) == 1U)
        {
            if (history.historyId == 0U ||
                history.complaintId == 0U)
            {
                writeApplicationLog(
                    "LOAD_HISTORY",
                    "Invalid history record skipped"
                );

                continue;
            }

            if (!appendHistoryNode(
                    app,
                    &history
                ))
            {
                (void)fclose(file);
                freeApplicationData(app);
                return 0;
            }
        }

        if (ferror(file))
        {
            writeApplicationLog(
                "LOAD_HISTORY",
                "Error while reading history file"
            );
        }

        (void)fclose(file);
    }

    return 1;
}

/* =========================================================
 * FILE STORAGE
 * ========================================================= */

int saveComplaints(
    const ApplicationContext *app
)
{
    FILE *file;
    const ComplaintNode *current;

    if (app == NULL)
    {
        return 0;
    }

    file = fopen(
        COMPLAINT_FILE,
        "wb"
    );

    if (file == NULL)
    {
        writeApplicationLog(
            "SAVE_COMPLAINTS",
            "Unable to open complaint file"
        );

        return 0;
    }

    current = app->complaintHead;

    while (current != NULL)
    {
        if (fwrite(
                &current->data,
                sizeof(current->data),
                1U,
                file
            ) != 1U)
        {
            writeApplicationLog(
                "SAVE_COMPLAINTS",
                "Unable to write complaint record"
            );

            (void)fclose(file);
            return 0;
        }

        current = current->next;
    }

    if (fclose(file) != 0)
    {
        writeApplicationLog(
            "SAVE_COMPLAINTS",
            "Unable to close complaint file"
        );

        return 0;
    }

    return 1;
}

int saveHistory(
    const ApplicationContext *app
)
{
    FILE *file;
    const HistoryNode *current;

    if (app == NULL)
    {
        return 0;
    }

    file = fopen(
        HISTORY_FILE,
        "wb"
    );

    if (file == NULL)
    {
        writeApplicationLog(
            "SAVE_HISTORY",
            "Unable to open history file"
        );

        return 0;
    }

    current = app->historyHead;

    while (current != NULL)
    {
        if (fwrite(
                &current->data,
                sizeof(current->data),
                1U,
                file
            ) != 1U)
        {
            writeApplicationLog(
                "SAVE_HISTORY",
                "Unable to write history record"
            );

            (void)fclose(file);
            return 0;
        }

        current = current->next;
    }

    if (fclose(file) != 0)
    {
        writeApplicationLog(
            "SAVE_HISTORY",
            "Unable to close history file"
        );

        return 0;
    }

    return 1;
}

int saveCounters(
    const ApplicationContext *app
)
{
    FILE *file;

    if (app == NULL)
    {
        return 0;
    }

    file = fopen(
        COUNTER_FILE,
        "wb"
    );

    if (file == NULL)
    {
        writeApplicationLog(
            "SAVE_COUNTERS",
            "Unable to open counter file"
        );

        return 0;
    }

    if (fwrite(
            &app->counters,
            sizeof(app->counters),
            1U,
            file
        ) != 1U)
    {
        writeApplicationLog(
            "SAVE_COUNTERS",
            "Unable to write counter record"
        );

        (void)fclose(file);
        return 0;
    }

    if (fclose(file) != 0)
    {
        writeApplicationLog(
            "SAVE_COUNTERS",
            "Unable to close counter file"
        );

        return 0;
    }

    return 1;
}

/* =========================================================
 * LOGGING
 * ========================================================= */

void writeAuditLog(
    const char *action
)
{
    FILE *file;
    char timestamp[TIMESTAMP_LENGTH];

    if (action == NULL)
    {
        return;
    }

    file = fopen(AUDIT_FILE, "a");

    if (file == NULL)
    {
        return;
    }

    getCurrentTimestamp(
        timestamp,
        sizeof(timestamp)
    );

    (void)fprintf(
        file,
        "[%s] %s\n",
        timestamp,
        action
    );

    (void)fclose(file);
}

void writeComplaintAuditLog(
    const char *actor,
    const char *action,
    unsigned int complaintId,
    const char *result
)
{
    char message[250];

    if (actor == NULL ||
        action == NULL ||
        result == NULL)
    {
        return;
    }

    (void)snprintf(
        message,
        sizeof(message),
        "%s | %s | Complaint ID: %u | %s",
        actor,
        action,
        complaintId,
        result
    );

    writeAuditLog(message);
}

void writeApplicationLog(
    const char *operation,
    const char *errorMessage
)
{
    FILE *file;
    char timestamp[TIMESTAMP_LENGTH];

    if (operation == NULL ||
        errorMessage == NULL)
    {
        return;
    }

    file = fopen(
        APPLICATION_LOG,
        "a"
    );

    if (file == NULL)
    {
        return;
    }

    getCurrentTimestamp(
        timestamp,
        sizeof(timestamp)
    );

    (void)fprintf(
        file,
        "[%s] ERROR | %s | %s\n",
        timestamp,
        operation,
        errorMessage
    );

    (void)fclose(file);
}

/* =========================================================
 * MEMORY CLEANUP
 * ========================================================= */

void freeApplicationData(
    ApplicationContext *app
)
{
    ComplaintNode *complaintNode;
    HistoryNode *historyNode;

    if (app == NULL)
    {
        return;
    }

    while (app->complaintHead != NULL)
    {
        complaintNode =
            app->complaintHead;

        app->complaintHead =
            complaintNode->next;

        free(complaintNode);
    }

    while (app->historyHead != NULL)
    {
        historyNode =
            app->historyHead;

        app->historyHead =
            historyNode->next;

        free(historyNode);
    }
}