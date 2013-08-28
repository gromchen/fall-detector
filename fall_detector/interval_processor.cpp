#include "interval_processor.h"

#include <cmath>

#include "interval_data.h"

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

    high_resolution_clock::time_point current_time =
            high_resolution_clock::now();
    int number_of_milliseconds =
            duration_cast<milliseconds>(
                current_time - mTimeOfPreviousSecond).count();

    if(number_of_milliseconds >= 1000)
    {
        int number_of_frames_with_found_object = 0;
        double standard_deviation_of_orientation = 0;
        double standard_deviatioin_of_ratio = 0;

        double sum_of_angles = 0;
        double sum_of_ratios = 0;
        vector<double> angles;
        vector<double> ratios;
        unsigned int frame_data_size = mFrameDataCollection.size();

        for(unsigned int iEllipse = 0; iEllipse < frame_data_size; iEllipse++)
        {
            if(mFrameDataCollection[iEllipse].ObjectFound())
            {
                RotatedRect object = mFrameDataCollection[iEllipse].GetObject();
                angles.push_back(object.angle);
                sum_of_angles += object.angle;

                double ratio = object.size.height / object.size.width;
                ratios.push_back(ratio);
                sum_of_ratios += ratio;

                number_of_frames_with_found_object++;
            }
        }

        if(number_of_frames_with_found_object > 0)
        {
            double average_of_angles =
                    sum_of_angles/number_of_frames_with_found_object;
            double average_of_ratios =
                    sum_of_ratios/number_of_frames_with_found_object;
            double mean_square_sum_of_angles = 0;
            double mean_square_sum_of_ratios = 0;
            unsigned int angles_size = angles.size();

            for(unsigned int iFoundObject = 0;
                iFoundObject < angles_size;
                iFoundObject++)
            {
                mean_square_sum_of_angles +=
                        pow(angles[iFoundObject] - average_of_angles, 2);
                mean_square_sum_of_ratios +=
                        pow(ratios[iFoundObject] - average_of_ratios, 2);
            }

            standard_deviation_of_orientation =
                    sqrt(mean_square_sum_of_angles
                         / number_of_frames_with_found_object);
            standard_deviatioin_of_ratio =
                    sqrt(mean_square_sum_of_ratios
                         / number_of_frames_with_found_object);
        }
        else
        {
            standard_deviation_of_orientation = 0;
            standard_deviatioin_of_ratio = 0;
        }

        // Determination of the fall
//        if(standard_deviation_of_orientation > 15
//                || standard_deviatioin_of_ratio > 0.9)
//            mFiniteStateMachine.Fall();
//        else
//            mFiniteStateMachine.Walk();

        // Data collection
        double fps = mFrameDataCollection.size() * 1000.0 / number_of_milliseconds;
        mFrameDataCollection.clear();
        mTimeOfPreviousSecond = current_time;

        mDataCollector.CollectData(IntervalData(boost::posix_time::microsec_clock::local_time(),
                                                fps,
                                                standard_deviation_of_orientation,
                                                standard_deviatioin_of_ratio,
                                                number_of_frames_with_found_object));
    }
}
}
