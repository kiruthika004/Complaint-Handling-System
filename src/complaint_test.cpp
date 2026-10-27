#include <gtest/gtest.h>
#include <cstring>

extern "C"
{
    #include "common.h"
    #include "module1.h"
    #include "module2.h"
}
#define main complaint_main
#include "../src/main.c"
#undef main

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

    (void)freopen("input.txt", "r", stdin);

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

    (void)freopen("input2.txt", "r", stdin);

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

    (void)freopen("input3.txt", "r", stdin);

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
TEST(ReadLineTest, EndOfFileClearsBuffer)
{
    FILE *fp = fopen("readline_eof.txt", "w");
    ASSERT_NE(fp, nullptr);
    fclose(fp);

    ASSERT_NE(
        freopen("readline_eof.txt", "r", stdin),
        nullptr
    );

    char buffer[10] = "not empty";

    readLine("Prompt", buffer, sizeof(buffer));

    EXPECT_STREQ(buffer, "");
    remove("readline_eof.txt");
}
//read integer
TEST(ReadIntegerTest, ValidNumber)
{
    FILE *fp = fopen("int1.txt", "w");
    fprintf(fp, "5\n");
    fclose(fp);

    (void)freopen("int1.txt", "r", stdin);

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

    (void)freopen("int2.txt", "r", stdin);

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

    (void)freopen("int3.txt", "r", stdin);

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

    (void)freopen("pause.txt", "r", stdin);

    pauseScreen();

    SUCCEED();

    remove("pause.txt");
}

//SrcModule1

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

    (void)freopen(
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

    (void)freopen(
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

    (void)freopen(
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

    (void)freopen(
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

    (void)freopen(
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

    (void)freopen(
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

    (void)freopen(
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
//Escalated complaint display
TEST(DisplayComplaintTest, EscalatedComplaint)
{
    Complaint complaint{};

    complaint.complaintId = 1001;
    complaint.status = STATUS_ESCALATED;
    complaint.escalationLevel = 2;

    strcpy(
        complaint.escalationReason,
        "Urgent issue"
    );

    displayComplaint(&complaint);

    SUCCEED();
}
//Customer feedback display
TEST(DisplayComplaintTest, FeedbackPresent)
{
    Complaint complaint{};

    complaint.complaintId = 1001;

    complaint.customerRating = 5;

    strcpy(
        complaint.customerFeedback,
        "Very good support"
    );

    displayComplaint(&complaint);

    SUCCEED();
}
//submit feedback
TEST(SubmitFeedbackTest, NullApp)
{
    EXPECT_EQ(
        submitFeedback(NULL),
        0
    );
}
//Complaint not found
TEST(SubmitFeedbackTest, ComplaintNotFound)
{
    ApplicationContext app{};

    FILE *fp = fopen(
        "feedback1.txt",
        "w"
    );

    fprintf(
        fp,
        "1001\n"
    );

    fclose(fp);

    (void)freopen(
        "feedback1.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        submitFeedback(&app),
        0
    );

    remove("feedback1.txt");
}
//cover unresolved complaint
TEST(SubmitFeedbackTest, NotResolved)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    FILE *fp = fopen(
        "feedback2.txt",
        "w"
    );

    fprintf(
        fp,
        "1001\n"
    );

    fclose(fp);

    (void)freopen(
        "feedback2.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        submitFeedback(&app),
        0
    );

    remove("feedback2.txt");
}
//cover successful feedback
TEST(SubmitFeedbackTest, Success)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_RESOLVED;

    app.complaintHead = &node;

    FILE *fp = fopen(
        "feedback3.txt",
        "w"
    );

    fprintf(
        fp,
        "1001\n"
        "Good service\n"
        "5\n"
    );

    fclose(fp);

    (void)freopen(
        "feedback3.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        submitFeedback(&app),
        1
    );

    remove("feedback3.txt");
}
//cover invalid email branch 
TEST(RegisterComplaintTest, InvalidEmailThenValid)
{
    ApplicationContext app{};

    FILE *fp =
        fopen("reg_email.txt","w");

    fprintf(
        fp,
        "John\n"
        "wrongemail\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "1\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen(
        "reg_email.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("reg_email.txt");
}
//cover invalid phone branch
TEST(RegisterComplaintTest, InvalidPhoneThenValid)
{
    ApplicationContext app{};

    FILE *fp =
        fopen("reg_phone.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "abcd\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "1\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen(
        "reg_phone.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("reg_phone.txt");
}
//Payment issue
TEST(RegisterComplaintTest, CategoryPaymentIssue)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat5.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "5\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat5.txt","r",stdin);

    EXPECT_EQ(registerComplaint(&app),1);

    remove("cat5.txt");
}
//Track complaint phone branch
TEST(TrackComplaintTest, MatchUsingPhone)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;

    strcpy(node.data.orderId,"ORD001");
    strcpy(node.data.email,"abc@gmail.com");
    strcpy(node.data.phone,"9876543210");

    app.complaintHead = &node;

    FILE *fp = fopen("phonematch.txt","w");

    fprintf(
        fp,
        "1001\n"
        "ORD001\n"
        "9876543210\n"
    );

    fclose(fp);

    (void)freopen(
        "phonematch.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        trackComplaint(&app),
        1
    );

    remove("phonematch.txt");
}
//Linked list traversal
TEST(RegisterComplaintTest, AppendToExistingList)
{
    ApplicationContext app{};

    ComplaintNode existing{};

    existing.data.complaintId = 1000;
    existing.next = NULL;

    app.complaintHead = &existing;

    FILE *fp = fopen("append.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "1\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen(
        "append.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("append.txt");
}
//Missing item
TEST(RegisterComplaintTest, CategoryMissingItem)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat3.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "3\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat3.txt","r",stdin);

    EXPECT_EQ(registerComplaint(&app),1);

    remove("cat3.txt");
}
//wrong delivery
TEST(RegisterComplaintTest, CategoryWrongDelivery)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat2.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "2\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat2.txt","r",stdin);

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("cat2.txt");
}
//delivery delay
TEST(RegisterComplaintTest, CategoryDeliveryDelay)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat4.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "4\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat4.txt","r",stdin);

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("cat4.txt");
}
//Refund delay
TEST(RegisterComplaintTest, CategoryRefundDelay)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat6.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "6\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat6.txt","r",stdin);

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("cat6.txt");
}
//cancellation issue
TEST(RegisterComplaintTest, CategoryCancellationIssue)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat7.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "7\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat7.txt","r",stdin);

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("cat7.txt");
}
//Product quality
TEST(RegisterComplaintTest, CategoryProductQuality)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat8.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "8\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat8.txt","r",stdin);

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("cat8.txt");
}
//other category
TEST(RegisterComplaintTest, CategoryOther)
{
    ApplicationContext app{};

    FILE *fp = fopen("cat9.txt","w");

    fprintf(
        fp,
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "9\n"
        "Title\n"
        "Description\n"
    );

    fclose(fp);

    (void)freopen("cat9.txt","r",stdin);

    EXPECT_EQ(
        registerComplaint(&app),
        1
    );

    remove("cat9.txt");
}
//SrcModule2

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
//escalate complaint
TEST(EscalateTest, NullApp)
{
    EXPECT_FALSE(
        escalateComplaint(
            NULL,
            1001,
            2,
            "Urgent"
        )
    );
}
TEST(EscalateTest, NullReason)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        escalateComplaint(
            &app,
            1001,
            2,
            NULL
        )
    );
}
TEST(EscalateTest, EmptyReason)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        escalateComplaint(
            &app,
            1001,
            2,
            ""
        )
    );
}
TEST(EscalateTest, InvalidLevelLow)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        escalateComplaint(
            &app,
            1001,
            0,
            "Urgent"
        )
    );
}
TEST(EscalateTest, InvalidLevelHigh)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        escalateComplaint(
            &app,
            1001,
            4,
            "Urgent"
        )
    );
}
TEST(EscalateTest, ComplaintNotFound)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        escalateComplaint(
            &app,
            1001,
            2,
            "Urgent"
        )
    );
}
TEST(EscalateTest, ResolvedComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_RESOLVED;

    app.complaintHead = &node;

    EXPECT_FALSE(
        escalateComplaint(
            &app,
            1001,
            2,
            "Urgent"
        )
    );
}
TEST(EscalateTest, Success)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;
    node.data.priority = PRIORITY_LOW;

    app.complaintHead = &node;

    EXPECT_TRUE(
        escalateComplaint(
            &app,
            1001,
            2,
            "Critical issue"
        )
    );
}
//resolve Complaint
TEST(ResolveTest, NullApp)
{
    EXPECT_FALSE(
        resolveComplaint(
            NULL,
            1001,
            "Resolved"
        )
    );
}
TEST(ResolveTest, NullResolution)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        resolveComplaint(
            &app,
            1001,
            NULL
        )
    );
}
TEST(ResolveTest, EmptyResolution)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        resolveComplaint(
            &app,
            1001,
            ""
        )
    );
}
TEST(ResolveTest, ComplaintNotFound)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        resolveComplaint(
            &app,
            1001,
            "Done"
        )
    );
}
TEST(ResolveTest, InvalidStatus)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    EXPECT_FALSE(
        resolveComplaint(
            &app,
            1001,
            "Done"
        )
    );
}
TEST(ResolveTest, InProgressSuccess)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_IN_PROGRESS;

    app.complaintHead = &node;

    EXPECT_TRUE(
        resolveComplaint(
            &app,
            1001,
            "Fixed"
        )
    );
}
//close complaint
TEST(CloseTest, NullApp)
{
    EXPECT_FALSE(
        closeComplaint(
            NULL,
            1001
        )
    );
}
TEST(CloseTest, ComplaintNotFound)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        closeComplaint(
            &app,
            1001
        )
    );
}
TEST(CloseTest, NotResolved)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    EXPECT_FALSE(
        closeComplaint(
            &app,
            1001
        )
    );
}
TEST(CloseTest, EmptyResolution)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_RESOLVED;

    app.complaintHead = &node;

    EXPECT_FALSE(
        closeComplaint(
            &app,
            1001
        )
    );
}
TEST(CloseTest, Success)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_RESOLVED;

    strcpy(
        node.data.resolution,
        "Resolved"
    );

    app.complaintHead = &node;

    EXPECT_TRUE(
        closeComplaint(
            &app,
            1001
        )
    );
}
//addComplaintHistory
TEST(HistoryTest, NullApp)
{
    EXPECT_FALSE(
        addComplaintHistory(
            NULL,
            1001,
            "Created",
            "Registered"
        )
    );
}
TEST(HistoryTest, NullAction)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        addComplaintHistory(
            &app,
            1001,
            NULL,
            "Details"
        )
    );
}
TEST(HistoryTest, NullDetails)
{
    ApplicationContext app{};

    EXPECT_FALSE(
        addComplaintHistory(
            &app,
            1001,
            "Created",
            NULL
        )
    );
}
TEST(HistoryTest, Success)
{
    ApplicationContext app{};

    EXPECT_TRUE(
        addComplaintHistory(
            &app,
            1001,
            "Created",
            "Complaint Registered"
        )
    );
}
//dislayComplaintHistory
TEST(DisplayHistoryTest, NullApp)
{
    displayComplaintHistory(
        NULL,
        1001
    );

    SUCCEED();
}
TEST(DisplayHistoryTest, EmptyHistory)
{
    ApplicationContext app{};

    displayComplaintHistory(
        &app,
        1001
    );

    SUCCEED();
}
TEST(DisplayHistoryTest, MatchingHistory)
{
    ApplicationContext app{};
    HistoryNode node{};

    node.data.complaintId = 1001;

    strcpy(
        node.data.action,
        "Created"
    );

    strcpy(
        node.data.details,
        "Registered"
    );

    strcpy(
        node.data.timestamp,
        "2024-01-01"
    );

    app.historyHead = &node;

    displayComplaintHistory(
        &app,
        1001
    );

    SUCCEED();
}
TEST(DisplayHistoryTest, NonMatchingHistory)
{
    ApplicationContext app{};
    HistoryNode node{};

    node.data.complaintId = 2000;

    app.historyHead = &node;

    displayComplaintHistory(
        &app,
        1001
    );

    SUCCEED();
}
//generate summary report
TEST(SummaryReportTest, NullApp)
{
    generateSummaryReport(NULL);

    SUCCEED();
}
TEST(SummaryReportTest, EmptyList)
{
    ApplicationContext app{};

    generateSummaryReport(&app);

    SUCCEED();
}
TEST(SummaryReportTest, MultipleComplaints)
{
    ApplicationContext app{};

    ComplaintNode node1{};
    ComplaintNode node2{};
    ComplaintNode node3{};

    node1.data.status = STATUS_OPEN;
    node1.data.priority = PRIORITY_HIGH;
    node1.data.category = CATEGORY_DAMAGED_PRODUCT;

    node2.data.status = STATUS_RESOLVED;
    node2.data.priority = PRIORITY_LOW;
    node2.data.category = CATEGORY_PAYMENT_ISSUE;

    node3.data.status = STATUS_CLOSED;
    node3.data.priority = PRIORITY_CRITICAL;
    node3.data.category = CATEGORY_OTHER;

    node1.next = &node2;
    node2.next = &node3;

    app.complaintHead = &node1;

    generateSummaryReport(&app);

    SUCCEED();
}
//initialize Application
TEST(InitializeTest, NullApp)
{
    EXPECT_FALSE(
        initializeApplication(NULL)
    );
}
TEST(InitializeTest, Success)
{
    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
//SaveComplaint
TEST(SaveComplaintTest, NullApp)
{
    EXPECT_FALSE(
        saveComplaints(NULL)
    );
}
TEST(SaveComplaintTest, Success)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;

    app.complaintHead = &node;

    EXPECT_TRUE(
        saveComplaints(&app)
    );
}
//SaveHistory
TEST(SaveHistoryTest, NullApp)
{
    EXPECT_FALSE(
        saveHistory(NULL)
    );
}
TEST(SaveHistoryTest, Success)
{
    ApplicationContext app{};
    HistoryNode node{};

    node.data.historyId = 1;

    app.historyHead = &node;

    EXPECT_TRUE(
        saveHistory(&app)
    );
}
//Save counter
TEST(SaveCounterTest, NullApp)
{
    EXPECT_FALSE(
        saveCounters(NULL)
    );
}
TEST(SaveCounterTest, Success)
{
    ApplicationContext app{};

    EXPECT_TRUE(
        saveCounters(&app)
    );
}
//WriteAuditLog
TEST(AuditLogTest, NullAction)
{
    writeAuditLog(NULL);

    SUCCEED();
}
TEST(AuditLogTest, Success)
{
    writeAuditLog(
        "UNIT_TEST"
    );

    SUCCEED();
}
//writeComplaintAuditLog
TEST(ComplaintAuditTest, NullActor)
{
    writeComplaintAuditLog(
        NULL,
        "UPDATE",
        1001,
        "SUCCESS"
    );

    SUCCEED();
}
TEST(ComplaintAuditTest, Success)
{
    writeComplaintAuditLog(
        "ADMIN",
        "UPDATE",
        1001,
        "SUCCESS"
    );

    SUCCEED();
}
//writeApplicationLog
TEST(ApplicationLogTest, NullOperation)
{
    writeApplicationLog(
        NULL,
        "Error"
    );

    SUCCEED();
}
TEST(ApplicationLogTest, Success)
{
    writeApplicationLog(
        "UNIT_TEST",
        "No Error"
    );

    SUCCEED();
}
//freeApplicationData
TEST(FreeDataTest, NullApp)
{
    freeApplicationData(NULL);

    SUCCEED();
}
TEST(FreeDataTest, FreeAllocatedNodes)
{
    ApplicationContext app{};

    ComplaintNode *c = new ComplaintNode{};

    HistoryNode *h = new HistoryNode{};
            

    c->next = NULL;
    h->next = NULL;

    app.complaintHead = c;
    app.historyHead = h;

    freeApplicationData(&app);

    EXPECT_EQ(
        app.complaintHead,
        nullptr
    );

    EXPECT_EQ(
        app.historyHead,
        nullptr
    );
}
//validstatusTransition
TEST(StatusTransition, EscalatedToInProgress)
{
    EXPECT_TRUE(
        isValidStatusTransition(
            STATUS_ESCALATED,
            STATUS_IN_PROGRESS
        )
    );
}

TEST(StatusTransition, EscalatedToResolved)
{
    EXPECT_TRUE(
        isValidStatusTransition(
            STATUS_ESCALATED,
            STATUS_RESOLVED
        )
    );
}

TEST(StatusTransition, ResolvedToClosed)
{
    EXPECT_TRUE(
        isValidStatusTransition(
            STATUS_RESOLVED,
            STATUS_CLOSED
        )
    );
}

TEST(StatusTransition, ClosedToAnything)
{
    EXPECT_FALSE(
        isValidStatusTransition(
            STATUS_CLOSED,
            STATUS_OPEN
        )
    );
}
//updateComplaintPriority
TEST(PriorityTest, Success)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;

    app.complaintHead = &node;

    EXPECT_TRUE(
        updateComplaintPriority(
            &app,
            1001,
            PRIORITY_HIGH
        )
    );
}
//generateSummaryReportMissed
TEST(SummaryReportTest, CriticalPendingComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.status = STATUS_OPEN;
    node.data.priority = PRIORITY_CRITICAL;
    node.data.category = CATEGORY_OTHER;

    app.complaintHead = &node;

    generateSummaryReport(&app);

    SUCCEED();
}
//InitializeApplication
TEST(InitializeTest, LoadComplaintRecord)
{
    Complaint complaint{};

    complaint.complaintId = 1001;
    complaint.category = CATEGORY_DAMAGED_PRODUCT;
    complaint.priority = PRIORITY_HIGH;
    complaint.status = STATUS_OPEN;

    FILE *fp = fopen(COMPLAINT_FILE, "wb");

    fwrite(
        &complaint,
        sizeof(complaint),
        1,
        fp
    );

    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
//AppendHistoryNode
TEST(InitializeTest, LoadHistoryRecord)
{
    ComplaintHistory history{};

    history.historyId = 1;
    history.complaintId = 1001;

    FILE *fp = fopen(HISTORY_FILE, "wb");

    fwrite(
        &history,
        sizeof(history),
        1,
        fp
    );

    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
TEST(InitializeTest, LoadMultipleComplaintAndHistoryRecords)
{
    Complaint complaints[2]{};
    ComplaintHistory histories[2]{};

    complaints[0].complaintId = 1001;
    complaints[0].category = CATEGORY_DAMAGED_PRODUCT;
    complaints[0].priority = PRIORITY_HIGH;
    complaints[0].status = STATUS_OPEN;
    complaints[1] = complaints[0];
    complaints[1].complaintId = 1002;

    histories[0].historyId = 1;
    histories[0].complaintId = 1001;
    histories[1] = histories[0];
    histories[1].historyId = 2;
    histories[1].complaintId = 1002;

    FILE *fp = fopen(COMPLAINT_FILE, "wb");
    ASSERT_NE(fp, nullptr);
    ASSERT_EQ(
        fwrite(complaints, sizeof(complaints[0]), 2, fp),
        2U
    );
    fclose(fp);

    fp = fopen(HISTORY_FILE, "wb");
    ASSERT_NE(fp, nullptr);
    ASSERT_EQ(
        fwrite(histories, sizeof(histories[0]), 2, fp),
        2U
    );
    fclose(fp);

    ApplicationContext app{};

    ASSERT_TRUE(initializeApplication(&app));
    ASSERT_NE(app.complaintHead, nullptr);
    ASSERT_NE(app.complaintHead->next, nullptr);
    EXPECT_EQ(app.complaintHead->next->data.complaintId, 1002U);
    ASSERT_NE(app.historyHead, nullptr);
    ASSERT_NE(app.historyHead->next, nullptr);
    EXPECT_EQ(app.historyHead->next->data.historyId, 2U);

    freeApplicationData(&app);
    remove(COMPLAINT_FILE);
    remove(HISTORY_FILE);
}
//InvalidComplaintRecord
TEST(InitializeTest, InvalidComplaintRecord)
{
    Complaint complaint{};

    complaint.complaintId = 0;

    FILE *fp = fopen(COMPLAINT_FILE, "wb");

    fwrite(
        &complaint,
        sizeof(complaint),
        1,
        fp
    );

    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
//FreeApplicationData
TEST(FreeDataTest, MultipleNodes)
{
    ApplicationContext app{};

   ComplaintNode *c1 = new ComplaintNode{};

   ComplaintNode *c2 = new ComplaintNode{};


    c1->next = c2;
    c2->next = NULL;

    app.complaintHead = c1;

    freeApplicationData(&app);

    EXPECT_EQ(
        app.complaintHead,
        nullptr
    );
}
//CorruptedCounterFile
TEST(InitializeTest, CorruptedCounterFile)
{
    FILE *fp = fopen(
        COUNTER_FILE,
        "wb"
    );

    int x = 5;

    fwrite(
        &x,
        sizeof(int),
        1,
        fp
    );

    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
//InvalidHistoryRecord
TEST(InitializeTest, InvalidHistoryRecord)
{
    ComplaintHistory h{};

    h.historyId = 0;
    h.complaintId = 0;

    FILE *fp =
        fopen(HISTORY_FILE, "wb");

    fwrite(
        &h,
        sizeof(h),
        1,
        fp
    );

    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
//Complaint List Tranversal
TEST(SaveComplaintTest, MultipleNodes)
{
    ApplicationContext app{};

    ComplaintNode n1{};
    ComplaintNode n2{};

    n1.data.complaintId = 1;
    n2.data.complaintId = 2;

    n1.next = &n2;
    n2.next = NULL;

    app.complaintHead = &n1;

    EXPECT_TRUE(saveComplaints(&app));
}
//History list Traversal
TEST(SaveHistoryTest, MultipleNodes)
{
    ApplicationContext app{};

    HistoryNode h1{};
    HistoryNode h2{};

    h1.data.historyId = 1;
    h2.data.historyId = 2;

    h1.next = &h2;
    h2.next = NULL;

    app.historyHead = &h1;

    EXPECT_TRUE(saveHistory(&app));
}
//Invalid Priority
TEST(InitializeTest, InvalidPriorityRecord)
{
    Complaint c{};

    c.complaintId = 1001;
    c.category = CATEGORY_DAMAGED_PRODUCT;
    c.priority = (ComplaintPriority)99;
    c.status = STATUS_OPEN;

    FILE *fp = fopen(COMPLAINT_FILE, "wb");
    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(initializeApplication(&app));

    freeApplicationData(&app);
}
//Invalid status
TEST(InitializeTest, InvalidStatusRecord)
{
    Complaint c{};

    c.complaintId = 1001;
    c.category = CATEGORY_DAMAGED_PRODUCT;
    c.priority = PRIORITY_HIGH;
    c.status = (ComplaintStatus)99;

    FILE *fp = fopen(COMPLAINT_FILE, "wb");
    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);

    ApplicationContext app{};

    EXPECT_TRUE(initializeApplication(&app));

    freeApplicationData(&app);
}
//Missing Counter file
TEST(InitializeTest, MissingCounterFile)
{
    remove(COUNTER_FILE);

    ApplicationContext app{};

    EXPECT_TRUE(
        initializeApplication(&app)
    );

    freeApplicationData(&app);
}
//auto escalation
TEST(AutoEscalationTest, NullApp)
{
    EXPECT_EQ(
        checkAndEscalateOverdueComplaints(NULL),
        0
    );
}
TEST(AutoEscalationTest, EmptyList)
{
    ApplicationContext app{};

    EXPECT_EQ(
        checkAndEscalateOverdueComplaints(&app),
        0
    );
}
TEST(AutoEscalationTest, ClosedComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.status = STATUS_CLOSED;

    app.complaintHead = &node;

    EXPECT_EQ(
        checkAndEscalateOverdueComplaints(&app),
        0
    );
}
TEST(AutoEscalationTest, EscalateOverdueComplaint)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.complaintId = 1001;
    node.data.status = STATUS_OPEN;
    node.data.priority = PRIORITY_LOW;

    node.data.statusUpdatedEpoch =
        time(NULL) - (60 * 60 * 24 * 10);

    app.complaintHead = &node;

    EXPECT_GT(
        checkAndEscalateOverdueComplaints(&app),
        0
    );
}
//average rating logic
TEST(SummaryReportTest, AverageRating)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.status = STATUS_CLOSED;
    node.data.customerRating = 5;

    app.complaintHead = &node;

    generateSummaryReport(&app);

    SUCCEED();
}
TEST(SummaryReportTest, MultipleRatings)
{
    ApplicationContext app{};

    ComplaintNode n1{};
    ComplaintNode n2{};

    n1.data.customerRating = 4;
    n2.data.customerRating = 5;

    n1.next = &n2;

    app.complaintHead = &n1;

    generateSummaryReport(&app);

    SUCCEED();
}
//Linked list traversal
TEST(SaveHistoryTest, MultipleHistoryNodes)
{
    ApplicationContext app{};

    HistoryNode h1{};
    HistoryNode h2{};

    h1.next = &h2;

    app.historyHead = &h1;

    EXPECT_TRUE(
        saveHistory(&app)
    );
}
TEST(SaveComplaintTest, ThreeNodesTraversal)
{
    ApplicationContext app{};

    ComplaintNode c1{};
    ComplaintNode c2{};
    ComplaintNode c3{};

    c1.next = &c2;
    c2.next = &c3;
    c3.next = NULL;

    app.complaintHead = &c1;

    EXPECT_TRUE(saveComplaints(&app));
}
TEST(SaveHistoryTest, ThreeNodesTraversal)
{
    ApplicationContext app{};

    HistoryNode h1{};
    HistoryNode h2{};
    HistoryNode h3{};

    h1.next = &h2;
    h2.next = &h3;
    h3.next = NULL;

    app.historyHead = &h1;

    EXPECT_TRUE(saveHistory(&app));
}
TEST(AutoEscalationTest, ZeroStatusUpdatedEpoch)
{
    ApplicationContext app{};
    ComplaintNode node{};

    node.data.status = STATUS_OPEN;
    node.data.priority = PRIORITY_LOW;
    node.data.statusUpdatedEpoch = 0;

    app.complaintHead = &node;

    EXPECT_EQ(
        checkAndEscalateOverdueComplaints(&app),
        0
    );
}
/*
//Exit immediately
TEST(MainTest, ExitImmediately)
{
    FILE *fp = fopen("main1.txt","w");

    fprintf(fp,"3\n");

    fclose(fp);

    freopen("main1.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main1.txt");
}
//customer menu back
TEST(MainTest, CustomerMenuBack)
{
    FILE *fp = fopen("main2.txt","w");

    fprintf(
        fp,
        "1\n"
        "4\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "main2.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main2.txt");
}
//Track complaint customer menu
TEST(MainTest, CustomerTrackComplaint)
{
    FILE *fp = fopen("main3.txt","w");

    fprintf(
        fp,
        "1\n"
        "2\n"
        "1001\n"
        "ORD1\n"
        "email@test.com\n"
        "\n"
        "4\n"
        "3\n"
    );

    fclose(fp);

    freopen("main3.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main3.txt");
}
//submit feedback
TEST(MainTest, CustomerSubmitFeedback)
{
    FILE *fp = fopen("main4.txt","w");

    fprintf(
        fp,
        "1\n"
        "3\n"
        "1001\n"
        "\n"
        "4\n"
        "3\n"
    );

    fclose(fp);

    freopen("main4.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main4.txt");
}
//admin login failure
TEST(MainTest, AdminLoginFailure)
{
    FILE *fp = fopen("main5.txt","w");

    fprintf(
        fp,
        "2\n"
        "bad\n"
        "bad\n"
        "bad\n"
        "bad\n"
        "bad\n"
        "bad\n"
        "3\n"
    );

    fclose(fp);

    freopen("main5.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main5.txt");
}
//admin login success
TEST(MainTest, AdminLoginSuccessLogout)
{
    FILE *fp = fopen("main6.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "admin123\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen("main6.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main6.txt");
}
//view all complaints
TEST(MainTest, AdminMenuViewAll)
{
    FILE *fp = fopen("main7.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "admin123\n"
        "1\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen("main7.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main7.txt");
}
//search complaint not found
TEST(MainTest, SearchComplaintNotFound)
{
    FILE *fp = fopen("main8.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "admin123\n"
        "2\n"
        "9999\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen("main8.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main8.txt");
}
//filter complaints
TEST(MainTest, FilterComplaints)
{
    FILE *fp = fopen("main9.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "admin123\n"
        "3\n"
        "1\n"
        "1\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen("main9.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main9.txt");
}
//generate summary report
TEST(MainTest, SummaryReport)
{
    FILE *fp = fopen("main10.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "admin123\n"
        "11\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen("main10.txt","r",stdin);

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main10.txt");
}
    */