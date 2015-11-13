#pragma once

#include "greibach.h"

#define GREIBACH_ERROR_SERIES 300

namespace Greibach{
	#define NS(n) Chain::nonterminal(n)
	#define TS(n) Chain::terminal(n)

// 	Greibach greibach(NS('S'), TS('$'),
// 		1,
// 		Rule(NS('S'), GREIBACH_ERROR_SERIES+0,
// 		1,
// 
// 		)
// 
// 
// 
// 		);
};