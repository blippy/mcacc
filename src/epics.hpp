#pragma once

#include "etran.hpp"
#include "stend.hpp"

//void epics_main(const etran_ts& es);
void epics_main(const detran_cs& es, const stend_ts& stends);
