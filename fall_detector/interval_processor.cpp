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
        Parameters features;
        std::vector<optional<double> > angles;
        unsigned int frame_data_size = mFrameDataCollection.size();

        for(unsigned int iEllipse = 0; iEllipse < frame_data_size; iEllipse++)
        {
            if(mFrameDataCollection[iEllipse].ObjectFound())
            {
                RotatedRect object = mFrameDataCollection[iEllipse].GetObject();
                features.AddSummands(frameData.GetCoefficientOfMotion(), object.angle,
                                     object.size.height / object.size.width, object.center.x,
                                     object.center.y, object.size.height, object.size.width);

                angles.push_back(optional<double>(object.angle));
            }
            else
            {
                angles.push_back(optional<double>());
            }
        }

        if(features.GetNumberOfSummands() > 0)
            features.CalculateStandardDeviation();

        // Determination of the fall
        // TODO: coefficient of motion
        if(features.GetOrientation().GetStandardDeviation() > 15
                || features.GetRatio().GetStandardDeviation() > 0.9)
        {
            mFiniteStateMachine.Fall();
        }
        else
        {
            // TODO: && not working
            if(features.GetPositionX().GetStandardDeviation() <= 2
                    || features.GetPositionY().GetStandardDeviation() <= 2
                    || features.GetAxisA().GetStandardDeviation() <= 2
                    || features.GetAxisB().GetStandardDeviation() <= 2)
            {
                if(mFiniteStateMachine.GetState() == STANDING || mFiniteStateMachine.GetState() == WALKING)
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
                                                fps, features, mFiniteStateMachine.FallDetected(),
                                                mFiniteStateMachine.GetState(),
                                                angles));
    }
}

void IntervalProcessor::Reset()
{
    mFiniteStateMachine.Reset();
}
}
