///////////////////////////////////////////////////////////////////////////////
//
// Job
//
///////////////////////////////////////////////////////////////////////////////

#include "Job.h"

JobId IJob::nextId = 0;

IJob::IJob(JobType jtype) : id(nextId++), type(jtype), assignedTo(nullptr)
{
};

TransferJob::TransferJob(int32_t number,
                         LocationId pickupLoc,
                         SimTime pickupTime,
                         LocationId dropLoc,
                         SimTime dropBefore) :
    IJob(IJob::TRANSFER),
    state(TransferJob::BEFORE_PICKUP),
    quantity(number),
    pickupLocation(pickupLoc),
    pickupAfter(pickupTime),
    dropoffLocation(dropLoc),
    dropoffBefore(dropBefore)
{
}

OutOfServiceJob::OutOfServiceJob(LocationId suspendLoc,
                                 SimTime suspendTime,
                                 SimTime resumeTime) :
    IJob(IJob::TRANSFER),
    state(OutOfServiceJob::BEFORE_BREAK),
    suspendLocation(suspendLoc),
    suspendTime(suspendTime),
    resumeTime(resumeTime)
{
}
