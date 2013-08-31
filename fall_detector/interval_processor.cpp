#include "interval_processor.h"

#include "interval_info.h"

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

void IntervalProcessor::Include(FrameInfo frameInfo)
{
    mFrameInfos.push_back(frameInfo);

    high_resolution_clock::time_point current_time = high_resolution_clock::now();
    int number_of_milliseconds = duration_cast<milliseconds>(current_time - mTimeOfPreviousSecond).count();

    if(number_of_milliseconds >= 1000)
    {
        FeatureCollection feature_collection;
        std::vector<optional<double> > angles;

        for(unsigned int i_frame_data = 0; i_frame_data < mFrameInfos.size(); i_frame_data++)
        {
            FrameInfo frame_info = mFrameInfos[i_frame_data];

            if(frame_info.IsObjectFound())
            {
                RotatedRect rotated_rectangle = frame_info.GetRotatedRectangle();
                feature_collection.Add(frame_info.GetCoefficientOfMotion(),
                                       rotated_rectangle.angle,
                                       rotated_rectangle.size.height / rotated_rectangle.size.width,
                                       rotated_rectangle.center.x,
                                       rotated_rectangle.center.y,
                                       rotated_rectangle.size.height,
                                       rotated_rectangle.size.width);

                angles.push_back(optional<double>(rotated_rectangle.angle));
            }
            else
            {
                angles.push_back(optional<double>());
            }
        }

        if(feature_collection.GetNumberOfSummands() > 0)
        {
            feature_collection.CalculateStandardDeviation();
        }

        // Determination of the fall
        // TODO: coefficient of motion
        if(feature_collection.GetOrientation().GetStandardDeviation() > 15
           || feature_collection.GetRatio().GetStandardDeviation() > 0.9)
        {
            mFiniteStateMachine.MoveToState(FALLING);
        }
        else
        {
            // TODO: && not working
            if(feature_collection.GetPositionX().GetStandardDeviation() <= 2
               || feature_collection.GetPositionY().GetStandardDeviation() <= 2
               || feature_collection.GetAxisA().GetStandardDeviation() <= 2
               || feature_collection.GetAxisB().GetStandardDeviation() <= 2)
            {
                if(mFiniteStateMachine.GetHumanState() == STANDING
                   || mFiniteStateMachine.GetHumanState() == WALKING)
                {
                    mFiniteStateMachine.MoveToState(STANDING);
                }
                else
                {
                    if(mFiniteStateMachine.GetHumanState() == FALLING || mFiniteStateMachine.GetHumanState() == LYING)
                    {
                        mFiniteStateMachine.MoveToState(LYING);
                    }
                }
            }
            else
            {
                mFiniteStateMachine.MoveToState(WALKING);
            }
        }

        // Data collection
        double fps = mFrameInfos.size() * 1000.0 / number_of_milliseconds;
        mFrameInfos.clear();
        mTimeOfPreviousSecond = current_time;

        mDataCollector.Collect(IntervalInfo(boost::posix_time::microsec_clock::local_time(),
                                            fps,
                                            feature_collection,
                                            mFiniteStateMachine.IsFallDetected(),
                                            mFiniteStateMachine.GetHumanState(),
                                            angles));
    }
}

void IntervalProcessor::Reset()
{
    mFiniteStateMachine.Reset();
}
}
