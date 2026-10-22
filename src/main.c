#include <stdio.h>

#include "common.h"
#include "module1.h"
#include "module2.h"

static unsigned int readComplaintId(void)
{
    return (unsigned int)readInteger(
        "Complaint ID: ",
        1,
        2147483647
    );
}

static void customerMenu(ApplicationContext *app)
{
    int choice;

    do
    {
        puts("\n========== CUSTOMER MENU ==========");

        printf(
            "Helpdesk: %s | %s\n",
            HELPDESK_NUMBER,
            HELPDESK_EMAIL
        );

        puts("===================================");
        puts("1. Register Complaint");
        puts("2. Track Complaint");
        puts("3. Back to Main Menu");

        choice = readInteger(
            "Enter choice: ",
            1,
            3
        );

        switch (choice)
        {
            case 1:
                (void)registerComplaint(app);
                pauseScreen();
                break;

            case 2:
                (void)trackComplaint(app);
                pauseScreen();
                break;

            case 3:
                break;

            default:
                break;
        }
    }
    while (choice != 3);
}
static int adminLogin(void)
{
    char username[40];
    char password[40];
    char auditMessage[150];
    int attempt;

    for (attempt = 1; attempt <= 3; attempt++)
    {
        printf(
            "\nAdmin Login Attempt %d of 3\n",
            attempt
        );

        readLine(
            "Admin username: ",
            username,
            sizeof(username)
        );

        readLine(
            "Admin password: ",
            password,
            sizeof(password)
        );

        if (authenticateAdmin(
                username,
                password
            ))
        {
            (void)snprintf(
                auditMessage,
                sizeof(auditMessage),
                "ADMIN_LOGIN | Attempt: %d | SUCCESS",
                attempt
            );

            writeAuditLog(auditMessage);

            puts("Admin login successful.");
            return 1;
        }

        (void)snprintf(
            auditMessage,
            sizeof(auditMessage),
            "ADMIN_LOGIN | Attempt: %d | FAILED",
            attempt
        );

        writeAuditLog(auditMessage);

        printf(
            "Invalid Admin credentials. "
            "%d attempt(s) remaining.\n",
            3 - attempt
        );
    }

    puts(
        "Maximum login attempts reached. "
        "Returning to the Main Menu."
    );

    writeAuditLog(
        "ADMIN_LOGIN | Maximum attempts reached | ACCESS_DENIED"
    );

    return 0;
}
static void adminMenu(
    ApplicationContext *app
)
{
    int choice;
    int value;
    unsigned int complaintId;
    char text[RESOLUTION_LENGTH];
    ComplaintNode *node;

    do
    {
        puts("\n========== ADMIN MENU ==========");
        puts("1. View All Complaints");
        puts("2. Search Complaint by ID");
        puts("3. Filter Complaints");
        puts("4. Update Priority");
        puts("5. Update Status");
        puts("6. Add Admin Remark");
        puts("7. Escalate Complaint");
        puts("8. Resolve Complaint");
        puts("9. Close Complaint");
        puts("10. View Complaint History");
        puts("11. Generate Summary Report");
        puts("12. Logout");

        choice = readInteger(
            "Enter choice: ",
            1,
            12
        );

        switch (choice)
        {
            case 1:
                viewAllComplaints(app);
                break;

            case 2:
                complaintId =
                    readComplaintId();

                node = findComplaintById(
                    app,
                    complaintId
                );

                if (node != NULL)
                {
                    displayComplaint(
                        &node->data
                    );
                }
                else
                {
                    puts("Complaint not found.");
                }
                break;

            case 3:
                filterComplaints(app);
                break;

            case 4:
                complaintId =
                    readComplaintId();

                value = readInteger(
                    "1.Low 2.Medium "
                    "3.High 4.Critical: ",
                    1,
                    4
                );

                if (updateComplaintPriority(
                        app,
                        complaintId,
                        (ComplaintPriority)value
                    ))
                {
                    puts(
                        "Priority updated successfully."
                    );
                }
                else
                {
                    puts(
                        "Unable to update priority."
                    );
                }
                break;

            case 5:
                complaintId =
                    readComplaintId();

                puts(
                    "1.Open 2.In Progress "
                    "3.Escalated 4.Resolved 5.Closed"
                );

                value = readInteger(
                    "New status: ",
                    1,
                    5
                );

                if (value == STATUS_RESOLVED)
                {
                    puts(
                        "Use Resolve Complaint to "
                        "record resolution details."
                    );
                }
                else if (value == STATUS_CLOSED)
                {
                    puts(
                        "Use Close Complaint after "
                        "the complaint is resolved."
                    );
                }
                else if (updateComplaintStatus(
                             app,
                             complaintId,
                             (ComplaintStatus)value
                         ))
                {
                    puts(
                        "Status updated successfully."
                    );
                }
                else
                {
                    puts(
                        "Invalid status transition "
                        "or Complaint ID."
                    );
                }
                break;

            case 6:
                complaintId =
                    readComplaintId();

                readLine(
                    "Admin remark: ",
                    text,
                    REMARK_LENGTH
                );

                if (addAdminRemark(
                        app,
                        complaintId,
                        text
                    ))
                {
                    puts(
                        "Remark added successfully."
                    );
                }
                else
                {
                    puts(
                        "Unable to add remark."
                    );
                }
                break;

            case 7:
                complaintId =
                    readComplaintId();

                value = readInteger(
                    "Escalation level (1-3): ",
                    1,
                    3
                );

                readLine(
                    "Escalation reason: ",
                    text,
                    REMARK_LENGTH
                );

                if (escalateComplaint(
                        app,
                        complaintId,
                        (unsigned int)value,
                        text
                    ))
                {
                    puts(
                        "Complaint escalated successfully."
                    );
                }
                else
                {
                    puts(
                        "Unable to escalate complaint."
                    );
                }
                break;

            case 8:
                complaintId =
                    readComplaintId();

                readLine(
                    "Resolution details: ",
                    text,
                    sizeof(text)
                );

                if (resolveComplaint(
                        app,
                        complaintId,
                        text
                    ))
                {
                    puts(
                        "Complaint resolved successfully."
                    );
                }
                else
                {
                    puts(
                        "Only In Progress or Escalated "
                        "complaints can be resolved."
                    );
                }
                break;

            case 9:
                complaintId =
                    readComplaintId();

                if (closeComplaint(
                        app,
                        complaintId
                    ))
                {
                    puts(
                        "Complaint closed successfully."
                    );
                }
                else
                {
                    puts(
                        "Only a resolved complaint "
                        "can be closed."
                    );
                }
                break;

            case 10:
                complaintId =
                    readComplaintId();

                displayComplaintHistory(
                    app,
                    complaintId
                );
                break;

            case 11:
                generateSummaryReport(app);
                break;

            case 12:
                writeAuditLog(
                    "Admin logged out"
                );
                break;

            default:
                break;
        }

        if (choice != 12)
        {
            pauseScreen();
        }
    }
    while (choice != 12);
}

int main(void)
{
    ApplicationContext app;
    int choice;

    if (!initializeApplication(&app))
    {
        puts(
            "Unable to initialize "
            "the application."
        );

        return 1;
    }

    writeAuditLog(
        "Application started"
    );

    do
    {
      puts(
    "\n================================================"
);

puts(
    "       E-COMMERCE COMPLAINT HANDLING SYSTEM"
);

printf(
    "       Toll-Free Helpdesk: %s\n",
    HELPDESK_NUMBER
);

printf(
    "       Helpdesk Email    : %s\n",
    HELPDESK_EMAIL
);

puts(
    "================================================"
);
        puts("1. Customer Interface");
        puts("2. Admin Login");
        puts("3. Exit");

        choice = readInteger(
            "Enter choice: ",
            1,
            3
        );

        switch (choice)
        {
            case 1:
                customerMenu(&app);
                break;

            case 2:
                if (adminLogin())
                {
                    adminMenu(&app);
                }
                break;

            case 3:
                break;

            default:
                break;
        }
    }
    while (choice != 3);

    (void)saveComplaints(&app);
    (void)saveHistory(&app);
    (void)saveCounters(&app);

    writeAuditLog(
        "Application closed"
    );

    freeApplicationData(&app);

    puts("Application closed safely.");
    return 0;
}