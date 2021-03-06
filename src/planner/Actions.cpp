// The MIT License (MIT)

// Copyright (c) 2019, Microsoft

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
//
// Actions
//
///////////////////////////////////////////////////////////////////////////////

#include "Actions.h"

namespace Route13Plan
{

    // Create fine-grained Actions from every job
    Actions::Actions(JobList* jobs)
    {
        for (auto jobp : *jobs)
        {
            switch (jobp->type) 
            {
            case JobType::TRANSFER:
            {
                auto xferJob = static_cast<TransferJob*>(jobp);
                if (xferJob->state == TransferJobState::BEFORE_PICKUP) {
                    actions.push_back(std::unique_ptr<IAction>(
                        new PickupAction(jobp, xferJob->pickupLocation, xferJob->pickupAfter, xferJob->quantity)));
                    actions.push_back(std::unique_ptr<IAction>(
                        new DropoffAction(jobp, xferJob->dropoffLocation, xferJob->dropoffBefore, xferJob->quantity,
                            actions.size()-1)));
                }
                break;
            }
            case JobType::OUT_OF_SERVICE:
            {
                auto oosJob = static_cast<OutOfServiceJob*>(jobp);
                if (oosJob->state == OutOfServiceJobState::BEFORE_BREAK) {
                    actions.push_back(std::unique_ptr<IAction>(
                        new SuspendAction(jobp, oosJob->suspendLocation, oosJob->suspendTime, oosJob->resumeTime)));
                }
                break;
            }
            default:
                ;
            }
        }
    }

    // Output indented list of actions
    void Actions::print(std::ostream& out)
    {
        for (auto actioni = actions.begin(); actioni != actions.end(); ++actioni)
        {
            auto &action = *actioni;
            if (action) {
                out << "    ";
                action->print(out);
            }
        }
    }

}