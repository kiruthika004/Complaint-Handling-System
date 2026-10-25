#include <gtest/gtest.h>

extern "C"{
#include "common.h"
#include "module1.h"
#include "module2.h"
}

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

TEST(EmailTest, ValidEmail)
{
    EXPECT_TRUE(
        validateEmail(
            "siva@gmail.com"
        )
    );
}
TEST(EmailTest, InvalidEmail)
{
    EXPECT_FALSE(
        validateEmail(
            "abcgmail.com"
        )
    );
}
TEST(PhoneTest, ValidPhone)
{
    EXPECT_TRUE(
        validatePhone(
            "6380604173"
        )
    );
}

TEST(PhoneTest, InvalidPhoneLetters)
{
    EXPECT_FALSE(
        validatePhone(
            "98ab543210"
        )
    );
}
TEST(StatusTransition, OpenToProgress)
{
    EXPECT_TRUE(
        isValidStatusTransition(
            STATUS_OPEN,
            STATUS_IN_PROGRESS
        )
    );
}
TEST(PriorityText, Critical)
{
    EXPECT_STREQ(
        getPriorityText(
            PRIORITY_CRITICAL
        ),
        "Critical"
    );
}
TEST(PriorityText, High)
{
    EXPECT_STREQ(
        getPriorityText(
            PRIORITY_HIGH
        ),
        "High"
    );
}
TEST(CategoryText, Damaged)
{
    EXPECT_STREQ(
        getCategoryText(
            CATEGORY_DAMAGED_PRODUCT
        ),
        "Damaged Product"
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

TEST(ComplaintTest, FindComplaint)
{
    ApplicationContext app;
    memset(&app,0,sizeof(app));
    ComplaintNode node;
    node.data.complaintId=1001;
    node.next=NULL;
    app.complaintHead=&node;
    ComplaintNode *result;
    result=
        findComplaintById(
            &app,
            1001
        );
    EXPECT_NE(
        result,
        nullptr
    );
}
TEST(PriorityTest, UpdatePriority)
{
    ApplicationContext app;
    memset(&app,0,sizeof(app));
    ComplaintNode node;
    node.data.complaintId=1001;
    node.data.status=STATUS_OPEN;
    app.complaintHead=&node;
    EXPECT_TRUE(
        updateComplaintPriority(
            &app,
            1001,
            PRIORITY_HIGH
        )
    );
}
TEST(RemarkTest, AddRemark)
{
    ApplicationContext app;
    memset(&app,0,sizeof(app));
    ComplaintNode node;
    node.data.complaintId=1001;
    node.data.status=STATUS_OPEN;
    app.complaintHead=&node;
    EXPECT_TRUE(
        addAdminRemark(
            &app,
            1001,
            "Need investigation"
        )
    );
}