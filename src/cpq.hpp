#pragma once
/* mixture of centis, price, and quantity
 */

// TODO make this module more pervasive

#include <centis.hpp>
#include <price.hpp>
#include <quantity.hpp>

price operator/(const centis& c, const quantity& q);

