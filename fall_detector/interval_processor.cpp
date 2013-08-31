#include "interval_processor.h"

#include "interval_data.h"

using namespace boost;
using namespace boost::chrono;
using namespace cv;
using namespace FallDetector;

namespace FallDetector
{
IntervalProcessor::IntervalProcessor()
{
}

void IntervalProcessor::StartTracking()
{
    mTimeOfPreviousSecond = high_resolution_clock::now();
}

void IntervalProcessor::IncludeObject(FrameData frameData)
{
    mFrameDataCollection.push_back(frameData);

    high_resolution_clock::time_point current_time = high_resolution_clock::now();
    int number_of_milliseconds = duration_cast<milliseconds>(current_time - mTimeOfPreviousSecond).count();

    if(number_of_milliseconds >= 1000)
    {
        Parameters parameters;
        std::vector<optional<double> > angles;

        unsigned int frame_data_size = mFrameDataCollection.size();

        for(unsigned int i_frame_data = 0; i_frame_data < frame_data_size; i_frame_data++)
        {
            if(mFrameDataCollection[i_frame_data].ObjectFound())
            {
                RotatedRect object = mFrameDataCollection[i_frame_data].GetObject();
                parameters.AddSummands(mFrameDataCollection[i_frame_data].GetCoefficientOfMotion(),
                                     object.angle,
                                     object.size.height / object.size.width,
                                     object.center.x,
                                     object.center.y,
                                     object.size.height,
                                     object.size.width);

                angles.push_back(optional<double>(object.angle));
            }
            else
            {
                angles.push_back(optional<double>());
            }
        }

        if(parameters.GetNumberOfSummands() > 0)
        {
            parameters.CalculateStandardDeviation();
        }

        // Determination of the fall
        // TODO: coefficient of motion
        if(parameters.GetOrientation().GetStandardDeviation() > 15
           || parameters.GetRatio().GetStandardDeviation() > 0.9)
        {
            mFiniteStateMachine.Fall();
        }
        else
        {
            // TODO: && not working
            if(parameters.GetPositionX().GetStandardDeviation() <= 2
               || parameters.GetPositionY().GetStandardDeviation() <= 2
               || parameters.GetAxisA().GetStandardDeviation() <= 2
               || parameters.GetAxisB().GetStandardDeviation() <= 2)
            {
                if(mFiniteStateMachine.GetState() == STANDING
                   || mFiniteStateMachine.GetState() == WALKING)
                {
                    mFiniteStateMachine.Stand();
                }
                else
                {
                    if(mFiniteStateMachine.GetState() == FALLING || mFiniteStateMachine.GetState() == LYING)
                    {
                        mFiniteStateMachine.Lie();
                    }
                }
            }
            else
            {
                mFiniteStateMachine.Walk();
            }
        }

        // Data collection
        double fps = mFrameDataCollection.size() * 1000.0 / number_of_milliseconds;
        mFrameDataCollection.clear();
        mTimeOfPreviousSecond = current_time;

        mDataCollector.CollectData(IntervalData(boost::posix_time::microsec_clock::local_time(),
                                                fps,
                                                parameters,
                                                mFiniteStateMachine.FallDetected(),
                                                mFiniteStateMachine.GetState(),
                                                angles));
    }
}

void IntervalProcessor::Reset()
{
    mFiniteStateMachine.Reset();
}
}
