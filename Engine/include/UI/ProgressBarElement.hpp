/** @file "ProgressBarElement.hpp" */
#pragma once
#include "WidgetElement.hpp"

/**
 * @brief Element that displays a progress bar
 */
struct ProgressBarElement : WidgetElement
{
    DEFINE_RENDER_PASSTHROUGH

public:
    ProgressBarElement(const std::string& UID);

    /**
     * @brief Adds value to progress bar
     * @param v Value to add
     */
    void AddValue(float v);

    float value; ///< @brief value of progress bar @note Use AddValue to enforce max value capping
    float maxValue; ///< @brief max value of progress bar

    Color backgroundColor; ///< @brief Color of background bar
    Color barColor; ///< @brief Color of bar

};

BOOST_DESCRIBE_STRUCT(ProgressBarElement,
    (WidgetElement),
    (
        value,
        maxValue,
        backgroundColor,
        barColor
    )
)
