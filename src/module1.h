#ifndef MODULE1_H
#define MODULE1_H

#include "common.h"

int registerComplaint(ApplicationContext *app);
int trackComplaint(const ApplicationContext *app);
int submitFeedback(ApplicationContext *app);

ComplaintNode *findComplaintById(
    ApplicationContext *app,
    unsigned int complaintId
);

void displayComplaint(const Complaint *complaint);
void viewAllComplaints(const ApplicationContext *app);
void filterComplaints(const ApplicationContext *app);

#endif