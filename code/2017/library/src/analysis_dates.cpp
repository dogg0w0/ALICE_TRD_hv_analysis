#include "analysis_dates.hpp"

/* List of avaible Dates
2017/05/09
*/


dates::dates(std::string date)
{
    if (date == (std::string)"2017/05/09")
    {
        // individual set timestamps for run 2017/05/09
        std::cout << "Chosen Date 2017/05/09" << std::endl;
        timestamps = {
            {1, 1504576500, 1504579200},
            {2, 1504579800, 1504583700},
            {3, 1504584000, 1504587900},
            {4, 1504589100, 1504592400},
            {5, 1504605300, 1504607400},
            {6, 1504608000, 1504610700},
            {7, 1504611900, 1504615800},
            {8, 1504617600, 1504626600}};
        luminosity_labels = {"2.6 Hz/#mub", "9 Hz/#mub", "15 Hz/#mub", "30 Hz/#mub", "40 Hz/#mub", "52 Hz/#mub", "64 Hz/#mub", "70 Hz/#mub"};
        luminosity_points = {2.6, 9.0, 15.0, 30.0, 40.0, 52.0, 64.0, 70.0};
    }
    else
    {
        std::cout << "No Date choosen, abort execution" << std::endl;
    }
    
}