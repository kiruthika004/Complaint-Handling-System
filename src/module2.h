#ifndef MODULE2_H
#define MODULE2_H

#include "common.h"

/*
 * Admin authentication
 */
int authenticateAdmin(
    const char *username,
    const char *password
);

/*
 * Complaint status-transition validation
 */
int isValidStatusTransition(
    ComplaintStatus currentStatus,
    ComplaintStatus newStatus
);

/*
 * Complaint priority management
 */
int updateComplaintPriority(
    ApplicationContext *app,
    unsigned int complaintId,
    ComplaintPriority newPriority
);

/*
 * Complaint status management
 */
int updateComplaintStatus(
    ApplicationContext *app,
    unsigned int complaintId,
    ComplaintStatus newStatus
);

/*
 * Admin remark management
 */
int addAdminRemark(
    ApplicationContext *app,
    unsigned int complaintId,
    const char *remark
);

/*
 * Complaint escalation
 */
int escalateComplaint(
    ApplicationContext *app,
    unsigned int complaintId,
    unsigned int escalationLevel,
    const char *reason
);

/*
 * Complaint resolution
 */
int resolveComplaint(
    ApplicationContext *app,
    unsigned int complaintId,
    const char *resolution
);

/*
 * Complaint closure
 */
int closeComplaint(
    ApplicationContext *app,
    unsigned int complaintId
);

/*
 * Complaint-history management
 */
int addComplaintHistory(
    ApplicationContext *app,
    unsigned int complaintId,
    const char *action,
    const char *details
);

void displayComplaintHistory(
    const ApplicationContext *app,
    unsigned int complaintId
);

/*
 * Summary-report generation
 */
void generateSummaryReport(
    const ApplicationContext *app
);

/*
 * Application initialization
 */
int initializeApplication(
    ApplicationContext *app
);

/*
 * File-storage operations
 */
int saveComplaints(
    const ApplicationContext *app
);

int saveHistory(
    const ApplicationContext *app
);

int saveCounters(
    const ApplicationContext *app
);

/*
 * Audit-log operations
 */
void writeAuditLog(
    const char *action
);

void writeComplaintAuditLog(
    const char *actor,
    const char *action,
    unsigned int complaintId,
    const char *result
);

/*
 * Technical application-error logging
 */
void writeApplicationLog(
    const char *operation,
    const char *errorMessage
);

int checkAndEscalateOverdueComplaints(
    ApplicationContext *app
);

/*
 * Dynamic-memory cleanup
 */
void freeApplicationData(
    ApplicationContext *app
);

#endif