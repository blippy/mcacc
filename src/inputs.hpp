#pragma once

#include "comm.hpp"
#include "etran.hpp"
#include "nacc.hpp"
#include "ntran.hpp"


typedef struct inputs_t {
	comm_ts comms;
	etran_ts etrans;
	nacc_ts naccs;
	ntran_ts ntrans;
	period p;
	stends_t stends;
} inputs_t;

inputs_t read_inputs();
