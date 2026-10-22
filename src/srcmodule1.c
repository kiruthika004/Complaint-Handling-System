#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "module1.h"
#include "module2.h"

ComplaintNode *findComplaintById(
    ApplicationContext *app,
    unsigned int complaintId
)
{
    ComplaintNode *current;

    if (app == NULL)
    {
        return NULL;
    }

    current = app->complaintHead;

    while (current != NULL)
    {
        if (current->data.complaintId ==
            complaintId)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void displayComplaint(
    const Complaint *complaint
)
{
    if (complaint == NULL)
    {
        return;
    }

    puts("\n----------------------------------------");
    printf("Complaint ID    : %u\n",
           complaint->complaintId);
    printf("Customer Name   : %s\n",
           complaint->customerName);
    printf("Order ID        : %s\n",
           complaint->orderId);
    printf("Product ID      : %s\n",
           complaint->productId);
    printf("Category        : %s\n",
           getCategoryText(complaint->category));
    printf("Title           : %s\n",
           complaint->title);
    printf("Description     : %s\n",
           complaint->description);
    printf("Priority        : %s\n",
           getPriorityText(complaint->priority));
    printf("Status          : %s\n",
           getStatusText(complaint->status));

    printf(
        "Admin Remark    : %s\n",
        complaint->adminRemark[0] != '\0'
            ? complaint->adminRemark
            : "Not available"
    );

    printf(
        "Resolution      : %s\n",
        complaint->resolution[0] != '\0'
            ? complaint->resolution
            : "Not available"
    );

    printf("Created At      : %s\n",
           complaint->createdAt);
    printf("Last Updated At : %s\n",
           complaint->updatedAt);

    puts("----------------------------------------");
}

int registerComplaint(
    ApplicationContext *app
)
{
    ComplaintNode *newNode;
    ComplaintNode **current;

    if (app == NULL)
    {
        return 0;
    }

    newNode = (ComplaintNode *)calloc(1U, sizeof(*newNode));

    if (newNode == NULL)
    {
        puts("Memory allocation failed.");
        return 0;
    }

    readLine(
        "Customer name: ",
        newNode->data.customerName,
        sizeof(newNode->data.customerName)
    );

    do
    {
        readLine(
            "Email: ",
            newNode->data.email,
            sizeof(newNode->data.email)
        );

        if (!validateEmail(newNode->data.email))
        {
            puts("Invalid email format.");
        }
    }
    while (!validateEmail(newNode->data.email));

    do
    {
        readLine(
            "Phone: ",
            newNode->data.phone,
            sizeof(newNode->data.phone)
        );

        if (!validatePhone(newNode->data.phone))
        {
            puts(
                "Phone number must contain "
                "8 to 15 digits."
            );
        }
    }
    while (!validatePhone(newNode->data.phone));

    readLine(
        "Order ID: ",
        newNode->data.orderId,
        sizeof(newNode->data.orderId)
    );

    readLine(
        "Product ID: ",
        newNode->data.productId,
        sizeof(newNode->data.productId)
    );

    puts("\nComplaint Categories");
    puts("1. Damaged Product");
    puts("2. Wrong Delivery");
    puts("3. Missing Item");
    puts("4. Delivery Delay");
    puts("5. Payment Issue");
    puts("6. Refund Delay");
    puts("7. Cancellation Issue");
    puts("8. Product Quality");
    puts("9. Other");

    newNode->data.category =
        (ComplaintCategory)readInteger(
            "Category: ",
            1,
            9
        );

    readLine(
        "Complaint title: ",
        newNode->data.title,
        sizeof(newNode->data.title)
    );

    readLine(
        "Description: ",
        newNode->data.description,
        sizeof(newNode->data.description)
    );

    app->counters.lastComplaintId++;

    newNode->data.complaintId =
        app->counters.lastComplaintId;

    newNode->data.priority =
        PRIORITY_MEDIUM;

    newNode->data.status =
        STATUS_OPEN;

    getCurrentTimestamp(
        newNode->data.createdAt,
        sizeof(newNode->data.createdAt)
    );

    (void)snprintf(
        newNode->data.updatedAt,
        sizeof(newNode->data.updatedAt),
        "%s",
        newNode->data.createdAt
    );

    current = &app->complaintHead;

    while (*current != NULL)
    {
        current = &(*current)->next;
    }

    *current = newNode;

    if (!saveComplaints(app) ||
        !saveCounters(app))
    {
        puts("Unable to save complaint.");
        return 0;
    }

    (void)addComplaintHistory(
        app,
        newNode->data.complaintId,
        "Registered",
        "Complaint created with Open status"
    );

    writeAuditLog(
        "Customer registered a complaint"
    );

   puts("\n===========================================");
puts("       COMPLAINT REGISTERED SUCCESSFULLY");
puts("===========================================");

printf(
    "  COMPLAINT ID : %u\n",
    newNode->data.complaintId
);

printf(
    "  REGISTERED AT: %s\n",
    newNode->data.createdAt
);

puts("-------------------------------------------");
puts("Please note this Complaint ID carefully.");
puts("The Complaint ID is required for tracking");
puts("and all future complaint references.");
puts("===========================================");

    return 1;
}

int trackComplaint(
    const ApplicationContext *app
)
{
    unsigned int complaintId;
    char orderId[ID_LENGTH];
    char contact[CONTACT_LENGTH];
    const ComplaintNode *current;

    if (app == NULL)
    {
        return 0;
    }

    complaintId =
        (unsigned int)readInteger(
            "Complaint ID: ",
            1,
            2147483647
        );

    readLine(
        "Order ID: ",
        orderId,
        sizeof(orderId)
    );

    readLine(
        "Email or phone: ",
        contact,
        sizeof(contact)
    );

    current = app->complaintHead;

    while (current != NULL)
    {
        if (current->data.complaintId ==
                complaintId &&
            strcmp(
                current->data.orderId,
                orderId
            ) == 0 &&
            (strcmp(
                 current->data.email,
                 contact
             ) == 0 ||
             strcmp(
                 current->data.phone,
                 contact
             ) == 0))
        {
            displayComplaint(&current->data);
            return 1;
        }

        current = current->next;
    }

    puts(
        "Complaint not found or "
        "verification failed."
    );

    return 0;
}

void viewAllComplaints(
    const ApplicationContext *app
)
{
    const ComplaintNode *current;

    if (app == NULL ||
        app->complaintHead == NULL)
    {
        puts("No complaints are available.");
        return;
    }

    current = app->complaintHead;

    while (current != NULL)
    {
        displayComplaint(&current->data);
        current = current->next;
    }
}

void filterComplaints(
    const ApplicationContext *app
)
{
    int filterType;
    int value;
    int found = 0;
    const ComplaintNode *current;

    if (app == NULL)
    {
        return;
    }

    filterType = readInteger(
        "Filter by "
        "1.Status 2.Priority 3.Category: ",
        1,
        3
    );

    if (filterType == 1)
    {
        value = readInteger(
            "Status value (1-5): ",
            1,
            5
        );
    }
    else if (filterType == 2)
    {
        value = readInteger(
            "Priority value (1-4): ",
            1,
            4
        );
    }
    else
    {
        value = readInteger(
            "Category value (1-9): ",
            1,
            9
        );
    }

    current = app->complaintHead;

    while (current != NULL)
    {
        if ((filterType == 1 &&
             (int)current->data.status == value) ||
            (filterType == 2 &&
             (int)current->data.priority == value) ||
            (filterType == 3 &&
             (int)current->data.category == value))
        {
            displayComplaint(&current->data);
            found = 1;
        }

        current = current->next;
    }

    if (!found)
    {
        puts("No matching complaints found.");
    }
}