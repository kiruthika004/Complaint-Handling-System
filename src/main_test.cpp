#include <gtest/gtest.h>

extern "C"
{
    #include "common.h"
    #include "module1.h"
    #include "module2.h"
}

#define main complaint_main
#include "../src/main.c"
#undef main

TEST(MainTest, ExitImmediately)
{
    FILE *fp = fopen("main_exit.txt", "w");

    fprintf(fp, "3\n");

    fclose(fp);

    freopen(
        "main_exit.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("main_exit.txt");
}
//customer menu back
TEST(MainTest, CustomerMenuBack)
{
    FILE *fp = fopen("custback.txt","w");

    fprintf(
        fp,
        "1\n"
        "4\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "custback.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("custback.txt");
}
//admin login failure
TEST(MainTest, AdminLoginFailure)
{
    FILE *fp = fopen("adminfail.txt","w");

    fprintf(
        fp,
        "2\n"
        "bad\nbad\n"
        "bad\nbad\n"
        "bad\nbad\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "adminfail.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("adminfail.txt");
}
//admin login success
TEST(MainTest, AdminLoginSuccessLogout)
{
    FILE *fp = fopen("adminok.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "EcomAdmin@2026\n" 
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "adminok.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("adminok.txt");
}
//view all complaints
TEST(MainTest, AdminViewAll)
{
    FILE *fp = fopen("viewall.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "EcomAdmin@2026\n" 
        "1\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "viewall.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("viewall.txt");
}
//search complaint not found
TEST(MainTest, SearchComplaintNotFound)
{
    FILE *fp = fopen("search.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "EcomAdmin@2026\n" 
        "2\n"
        "9999\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "search.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("search.txt");
}
//filter complaints
TEST(MainTest, FilterComplaints)
{
    FILE *fp = fopen("filter.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "EcomAdmin@2026\n" 
        "3\n"
        "1\n"
        "1\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "filter.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("filter.txt");
}
//summary report
TEST(MainTest, SummaryReport)
{
    FILE *fp = fopen("summary.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "EcomAdmin@2026\n" 
        "11\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);

    freopen(
        "summary.txt",
        "r",
        stdin
    );

    EXPECT_EQ(
        complaint_main(),
        0
    );

    remove("summary.txt");
}
//customer register
TEST(MainTest, CustomerRegisterComplaint)
{
    FILE *fp = fopen("cust_reg.txt","w");

    fprintf(
        fp,
        "1\n"                    // Customer menu
        "1\n"                    // Register
        "John\n"
        "john@gmail.com\n"
        "9876543210\n"
        "ORD001\n"
        "PROD001\n"
        "1\n"
        "Broken Product\n"
        "Screen Cracked\n"
        "\n"
        "4\n"                    // Back
        "3\n"                    // Exit
    );

    fclose(fp);

    freopen("cust_reg.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("cust_reg.txt");
}
//track complaint
TEST(MainTest, CustomerTrackComplaint)
{
    FILE *fp = fopen("cust_track.txt","w");

    fprintf(
        fp,
        "1\n"
        "2\n"
        "1001\n"
        "ORD001\n"
        "john@gmail.com\n"
        "\n"
        "4\n"
        "3\n"
    );

    fclose(fp);

    freopen("cust_track.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("cust_track.txt");
}
//customer feedback
TEST(MainTest, CustomerFeedback)
{
    FILE *fp = fopen("cust_feedback.txt","w");

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

    freopen("cust_feedback.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("cust_feedback.txt");
}
//update priorty
TEST(MainTest, AdminUpdatePriority)
{
    FILE *fp = fopen("ap.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "YOUR_PASSWORD\n"
        "4\n"
        "1001\n"
        "3\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);
    freopen("ap.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("ap.txt");
}
//update status
TEST(MainTest, AdminUpdateStatus)
{
    FILE *fp = fopen("as.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "YOUR_PASSWORD\n"
        "5\n"
        "1001\n"
        "2\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);
    freopen("as.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("as.txt");
}
//admin remark
TEST(MainTest, AdminRemark)
{
    FILE *fp = fopen("remark.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "YOUR_PASSWORD\n"
        "6\n"
        "1001\n"
        "Checked by admin\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);
    freopen("remark.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("remark.txt");
}
//admin escalate
TEST(MainTest, AdminEscalate)
{
    FILE *fp = fopen("esc.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "YOUR_PASSWORD\n"
        "7\n"
        "1001\n"
        "2\n"
        "Urgent issue\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);
    freopen("esc.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("esc.txt");
}
//adin resolve
TEST(MainTest, AdminResolve)
{
    FILE *fp = fopen("resolve.txt","w");

    fprintf(
        fp,
        "2\n"
        "admin\n"
        "YOUR_PASSWORD\n"
        "8\n"
        "1001\n"
        "Resolved successfully\n"
        "\n"
        "12\n"
        "3\n"
    );

    fclose(fp);
    freopen("resolve.txt","r",stdin);

    EXPECT_EQ(complaint_main(),0);

    remove("resolve.txt");
}
