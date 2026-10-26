#include <gtest/gtest.h>
#include <cstring>

extern "C"
{
    #include "common.h"
    #include "module1.h"
    #include "module2.h"
}
TEST(EmailTest, NullEmail)
{
    EXPECT_FALSE(validateEmail(NULL));
}

TEST(EmailTest, EmailContainsSpace)
{
    EXPECT_FALSE(validateEmail("abc @gmail.com"));
}

TEST(EmailTest, MissingAtSymbol)
{
    EXPECT_FALSE(validateEmail("abcgmail.com"));
}

TEST(EmailTest, AtFirstPosition)
{
    EXPECT_FALSE(validateEmail("@gmail.com"));
}

TEST(EmailTest, MissingDotAfterAt)
{
    EXPECT_FALSE(validateEmail("abc@gmailcom"));
}

TEST(EmailTest, DotAtEnd)
{
    EXPECT_FALSE(validateEmail("abc@gmail."));
}

TEST(EmailTest, ValidEmail)
{
    EXPECT_TRUE(validateEmail("abc@gmail.com"));
}
//validate phone
TEST(PhoneTest, NullPhone)
{
    EXPECT_FALSE(validatePhone(NULL));
}

TEST(PhoneTest, TooShort)
{
    EXPECT_FALSE(validatePhone("123"));
}

TEST(PhoneTest, TooLong)
{
    EXPECT_FALSE(validatePhone("12345678901234567"));
}

TEST(PhoneTest, ContainsLetters)
{
    EXPECT_FALSE(validatePhone("98765abcde"));
}

TEST(PhoneTest, ContainsSpecialCharacter)
{
    EXPECT_FALSE(validatePhone("98765@1234"));
}

TEST(PhoneTest, ValidPhone)
{
    EXPECT_TRUE(validatePhone("9876543210"));
}
//get status text()
TEST(StatusText, Open)
{
    EXPECT_STREQ(getStatusText(STATUS_OPEN),"Open");
}

TEST(StatusText, InProgress)
{
    EXPECT_STREQ(getStatusText(STATUS_IN_PROGRESS),"In Progress");
}

TEST(StatusText, Escalated)
{
    EXPECT_STREQ(getStatusText(STATUS_ESCALATED),"Escalated");
}

TEST(StatusText, Resolved)
{
    EXPECT_STREQ(getStatusText(STATUS_RESOLVED),"Resolved");
}

TEST(StatusText, Closed)
{
    EXPECT_STREQ(getStatusText(STATUS_CLOSED),"Closed");
}

TEST(StatusText, Unknown)
{
    EXPECT_STREQ(
        getStatusText((ComplaintStatus)999),
        "Unknown"
    );
}
//get priority text
TEST(PriorityText, Low)
{
    EXPECT_STREQ(
        getPriorityText(PRIORITY_LOW),
        "Low"
    );
}

TEST(PriorityText, Medium)
{
    EXPECT_STREQ(
        getPriorityText(PRIORITY_MEDIUM),
        "Medium"
    );
}

TEST(PriorityText, High)
{
    EXPECT_STREQ(
        getPriorityText(PRIORITY_HIGH),
        "High"
    );
}

TEST(PriorityText, Critical)
{
    EXPECT_STREQ(
        getPriorityText(PRIORITY_CRITICAL),
        "Critical"
    );
}

TEST(PriorityText, Unknown)
{
    EXPECT_STREQ(
        getPriorityText((ComplaintPriority)999),
        "Unknown"
    );
}
//get category text
TEST(CategoryText, DamagedProduct)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_DAMAGED_PRODUCT
        ),
        "Damaged Product"
    );
}

TEST(CategoryText, WrongDelivery)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_WRONG_DELIVERY
        ),
        "Wrong Delivery"
    );
}

TEST(CategoryText, MissingItem)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_MISSING_ITEM
        ),
        "Missing Item"
    );
}

TEST(CategoryText, DeliveryDelay)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_DELIVERY_DELAY
        ),
        "Delivery Delay"
    );
}

TEST(CategoryText, PaymentIssue)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_PAYMENT_ISSUE
        ),
        "Payment Issue"
    );
}

TEST(CategoryText, RefundDelay)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_REFUND_DELAY
        ),
        "Refund Delay"
    );
}

TEST(CategoryText, CancellationIssue)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_CANCELLATION_ISSUE
        ),
        "Cancellation Issue"
    );
}

TEST(CategoryText, ProductQuality)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_PRODUCT_QUALITY
        ),
        "Product Quality"
    );
}

TEST(CategoryText, Other)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_OTHER
        ),
        "Other"
    );
}

TEST(CategoryText, Unknown)
{
    EXPECT_STREQ(
        getCategoryText(
            (ComplaintCategory)999
        ),
        "Unknown"
    );
}

//get current timestamp
TEST(TimeStampTest, NullBuffer)
{
    getCurrentTimestamp(NULL,100);
    SUCCEED();
}

TEST(TimeStampTest, ZeroBufferSize)
{
    char buffer[100];
    getCurrentTimestamp(buffer,0);
    SUCCEED();
}

TEST(TimeStampTest, ValidTimestamp)
{
    char buffer[100];

    getCurrentTimestamp(
        buffer,
        sizeof(buffer)
    );

    EXPECT_GT(
        strlen(buffer),
        0
    );
}
//readline
TEST(ReadLineTest, NullBuffer)
{
    readLine("Prompt", NULL, 10);
    SUCCEED();
}
TEST(ReadLineTest, ZeroBufferSize)
{
    char buffer[10];

    readLine("Prompt", buffer, 0);

    SUCCEED();
}
TEST(ReadLineTest, ValidInput)
{
    FILE *fp = fopen("input.txt", "w");
    fprintf(fp, "Hello\n");
    fclose(fp);

    freopen("input.txt", "r", stdin);

    char buffer[50];

    readLine(
        "Prompt",
        buffer,
        sizeof(buffer)
    );

    EXPECT_STREQ(
        buffer,
        "Hello"
    );

    remove("input.txt");
}
TEST(ReadLineTest, EmptyThenValid)
{
    FILE *fp = fopen("input2.txt", "w");
    fprintf(fp, "\nWorld\n");
    fclose(fp);

    freopen("input2.txt", "r", stdin);

    char buffer[50];

    readLine(
        "Prompt",
        buffer,
        sizeof(buffer)
    );

    EXPECT_STREQ(
        buffer,
        "World"
    );

    remove("input2.txt");
}
TEST(ReadLineTest, LongInput)
{
    FILE *fp = fopen("input3.txt", "w");

    fprintf(
        fp,
        "abcdefghijklmnopqrstuvwxyz\n"
    );

    fclose(fp);

    freopen("input3.txt", "r", stdin);

    char buffer[10];

    readLine(
        "Prompt",
        buffer,
        sizeof(buffer)
    );

    EXPECT_STRNE(
        buffer,
        ""
    );

    remove("input3.txt");
}
//read integer
TEST(ReadIntegerTest, ValidNumber)
{
    FILE *fp = fopen("int1.txt", "w");
    fprintf(fp, "5\n");
    fclose(fp);

    freopen("int1.txt", "r", stdin);

    EXPECT_EQ(
        readInteger(
            "Prompt",
            1,
            10
        ),
        5
    );

    remove("int1.txt");
}
TEST(ReadIntegerTest, InvalidThenValid)
{
    FILE *fp = fopen("int2.txt", "w");

    fprintf(
        fp,
        "abc\n7\n"
    );

    fclose(fp);

    freopen("int2.txt", "r", stdin);

    EXPECT_EQ(
        readInteger(
            "Prompt",
            1,
            10
        ),
        7
    );

    remove("int2.txt");
}
TEST(ReadIntegerTest, BelowMinimumThenValid)
{
    FILE *fp = fopen("int3.txt", "w");

    fprintf(
        fp,
        "-5\n3\n"
    );

    fclose(fp);

    freopen("int3.txt", "r", stdin);

    EXPECT_EQ(
        readInteger(
            "Prompt",
            1,
            10
        ),
        3
    );

    remove("int3.txt");
}
TEST(PauseScreenTest, EnterPressed)
{
    FILE *fp = fopen("pause.txt", "w");

    fprintf(fp, "\n");

    fclose(fp);

    freopen("pause.txt", "r", stdin);

    pauseScreen();

    SUCCEED();

    remove("pause.txt");
}
//find complaint by id
TEST(FindComplaintTest, NullApp)
{
    EXPECT_EQ(
        findComplaintById(
            NULL,
            1001
        ),
        nullptr
    );
}
TEST(FindComplaintTest, EmptyList)
{
    ApplicationContext app{};

    EXPECT_EQ(
        findComplaintById(
            &app,
            1001
        ),
        nullptr
    );
}
TEST(FindComplaintTest, ExistingComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;

    app.complaintHead = &node;

    EXPECT_NE(
        findComplaintById(
            &app,
            1001
        ),
        nullptr
    );
}
TEST(FindComplaintTest, MissingComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;

    app.complaintHead = &node;

    EXPECT_EQ(
        findComplaintById(
            &app,
            9999
        ),
        nullptr
    );
}
// display complaint
TEST(DisplayComplaintTest, NullComplaint)
{
    displayComplaint(NULL);

    SUCCEED();
}
TEST(DisplayComplaintTest, EmptyRemarkAndResolution)
{
    Complaint complaint{};

    complaint.complaintId = 1001;

    displayComplaint(&complaint);

    SUCCEED();
}
TEST(DisplayComplaintTest, FilledRemarkAndResolution)
{
    Complaint complaint{};

    complaint.complaintId = 1001;

    strcpy(
        complaint.adminRemark,
        "Checked"
    );

    strcpy(
        complaint.resolution,
        "Resolved"
    );

    displayComplaint(&complaint);

    SUCCEED();
}
//view complaints
TEST(ViewComplaintTest, NullApp)
{
    viewAllComplaints(NULL);

    SUCCEED();
}
TEST(ViewComplaintTest, EmptyList)
{
    ApplicationContext app{};

    viewAllComplaints(&app);

    SUCCEED();
}
TEST(ViewComplaintTest, SingleComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    app.complaintHead = &node;

    viewAllComplaints(&app);

    SUCCEED();
}
TEST(ViewComplaintTest, MultipleComplaints)
{
    ApplicationContext app{};

    ComplaintNode node1{};
    ComplaintNode node2{};

    node1.next = &node2;
    node2.next = NULL;

    app.complaintHead = &node1;

    viewAllComplaints(&app);

    SUCCEED();
}
//Track complaints
TEST(TrackComplaintTest, NullApp)
{
    EXPECT_EQ(
        trackComplaint(NULL),
        0
    );
}
TEST(TrackComplaintTest, ValidComplaintFound)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;

    strcpy(
        node.data.orderId,
        "ORD001"
    );

    strcpy(
        node.data.email,
        "abc@gmail.com"
    );

    strcpy(
        node.data.phone,
        "9876543210"
    );

    app.complaintHead = &node;

    FILE *fp = fopen("track1.txt", "w");

    fprintf(
        fp,
        "1001\nORD001\nabc@gmail.com\n"
    );

    fclose(fp);

    freopen(
        "track1.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        trackComplaint(&app),
        1
    );

    remove("track1.txt");
}
TEST(TrackComplaintTest, InvalidOrderId)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;

    strcpy(
        node.data.orderId,
        "ORD001"
    );

    strcpy(
        node.data.email,
        "abc@gmail.com"
    );

    app.complaintHead = &node;

    FILE *fp = fopen("track2.txt", "w");

    fprintf(
        fp,
        "1001\nWRONG\nabc@gmail.com\n"
    );

    fclose(fp);

    freopen(
        "track2.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        trackComplaint(&app),
        0
    );

    remove("track2.txt");
}
//Filter complaints
TEST(FilterComplaintTest, NullApp)
{
    filterComplaints(NULL);

    SUCCEED();
}
TEST(FilterComplaintTest, StatusMatch)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    FILE *fp = fopen("filter1.txt","w");

    fprintf(fp,"1\n1\n");

    fclose(fp);

    freopen(
        "filter1.txt",
        "r",
        stdin
    );

    filterComplaints(&app);

    remove("filter1.txt");
}
TEST(FilterComplaintTest, PriorityMatch)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.priority = PRIORITY_HIGH;

    app.complaintHead = &node;

    FILE *fp = fopen("filter2.txt","w");

    fprintf(fp,"2\n3\n");

    fclose(fp);

    freopen(
        "filter2.txt",
        "r",
        stdin
    );

    filterComplaints(&app);

    remove("filter2.txt");
}
TEST(FilterComplaintTest, CategoryMatch)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.category =
        CATEGORY_PAYMENT_ISSUE;

    app.complaintHead = &node;

    FILE *fp = fopen("filter3.txt","w");

    fprintf(fp,"3\n5\n");

    fclose(fp);

    freopen(
        "filter3.txt",
        "r",
        stdin
    );

    filterComplaints(&app);

    remove("filter3.txt");
}
TEST(FilterComplaintTest, NoMatch)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.status =
        STATUS_OPEN;

    app.complaintHead = &node;

    FILE *fp = fopen("filter4.txt","w");

    fprintf(fp,"1\n5\n");

    fclose(fp);

    freopen(
        "filter4.txt",
        "r",
        stdin
    );

    filterComplaints(&app);

    remove("filter4.txt");
}
//register complaints
TEST(RegisterComplaintTest, NullApp)
{
    EXPECT_EQ(
        registerComplaint(NULL),
        0
    );
}
TEST(RegisterComplaintTest, SuccessfulRegistration)
{
    ApplicationContext app{};

    FILE *fp = fopen(
        "register.txt",
        "w"
    );

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "1\n"
        "Broken Product\n"
        "Screen cracked\n"
    );

    fclose(fp);

    freopen(
        "register.txt",
        "r",
        stdin
    );

    app.counters.lastComplaintId = 1000;

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("register.txt");
}
//AUTHENTICATE_ADMIN
TEST(AdminTest, ValidCredentials)
{
    EXPECT_EQ(
        authenticateAdmin(
            "admin",
            "EcomAdmin@2026"
        ),
        1
    );
}

TEST(AdminTest, NullUsername)
{
    EXPECT_EQ(
        authenticateAdmin(
            NULL,
            "pass"
        ),
        0
    );
}

TEST(AdminTest, InvalidCredentials)
{
    EXPECT_EQ(
        authenticateAdmin(
            "wrong",
            "wrong"
        ),
        0
    );
}
//is valid status transition
TEST(StatusTransition, OpenToProgress)
{
    EXPECT_TRUE(
        isValidStatusTransition(
            STATUS_OPEN,
            STATUS_IN_PROGRESS
        )
    );
}

TEST(StatusTransition, OpenToEscalated)
{
    EXPECT_TRUE(
        isValidStatusTransition(
            STATUS_OPEN,
            STATUS_ESCALATED
        )
    );
}

TEST(StatusTransition, InvalidTransition)
{
    EXPECT_FALSE(
        isValidStatusTransition(
            STATUS_OPEN,
            STATUS_CLOSED
        )
    );
}
//update complaint priority
TEST(PriorityTest, NullApp)
{
    EXPECT_FALSE(
        updateComplaintPriority(
            NULL,
            1,
            PRIORITY_HIGH
        )
    );
}
TEST(PriorityTest, InvalidPriority)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        updateComplaintPriority(
            &app,
            1,
            (ComplaintPriority)99
        )
    );
}
TEST(PriorityTest, ComplaintNotFound)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        updateComplaintPriority(
            &app,
            1001,
            PRIORITY_HIGH
        )
    );
}
TEST(PriorityTest, ClosedComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_CLOSED;

    app.complaintHead = &node;

    EXPECT_FALSE(
        updateComplaintPriority(
            &app,
            1001,
            PRIORITY_HIGH
        )
    );
}
//update complaint status
TEST(StatusTest, NullApp)
{
    EXPECT_FALSE(
        updateComplaintStatus(
            NULL,
            1001,
            STATUS_IN_PROGRESS
        )
    );
}
TEST(StatusTest, ComplaintNotFound)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        updateComplaintStatus(
            &app,
            1001,
            STATUS_IN_PROGRESS
        )
    );
}
TEST(StatusTest, InvalidTransition)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    EXPECT_FALSE(
        updateComplaintStatus(
            &app,
            1001,
            STATUS_CLOSED
        )
    );
}
TEST(StatusTest, ResolvedNotAllowedHere)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_IN_PROGRESS;

    app.complaintHead = &node;

    EXPECT_FALSE(
        updateComplaintStatus(
            &app,
            1001,
            STATUS_RESOLVED
        )
    );
}
TEST(StatusTest, ValidTransitionSuccess)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    EXPECT_TRUE(
        updateComplaintStatus(
            &app,
            1001,
            STATUS_IN_PROGRESS
        )
    );
}
//add admin mark
TEST(RemarkTest, NullApp)
{
    EXPECT_FALSE(
        addAdminRemark(
            NULL,
            1001,
            "Remark"
        )
    );
}
TEST(RemarkTest, NullRemark)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        addAdminRemark(
            &app,
            1001,
            NULL
        )
    );
}
TEST(RemarkTest, EmptyRemark)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        addAdminRemark(
            &app,
            1001,
            ""
        )
    );
}
TEST(RemarkTest, ComplaintNotFound)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        addAdminRemark(
            &app,
            1001,
            "Remark"
        )
    );
}
TEST(RemarkTest, ClosedComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_CLOSED;

    app.complaintHead = &node;

    EXPECT_FALSE(
        addAdminRemark(
            &app,
            1001,
            "Remark"
        )
    );
}
TEST(RemarkTest, Success)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    EXPECT_TRUE(
        addAdminRemark(
            &app,
            1001,
            "Checked"
        )
    );
}