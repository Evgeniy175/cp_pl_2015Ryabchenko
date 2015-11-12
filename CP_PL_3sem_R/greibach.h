#pragma once

#include "errors.h"

typedef short GRALPHABET;

namespace Greibach{
	class Chain{
	public:
		Chain();
		Chain(
			short			numberOfSymbols,					// number of symbols in chain
			GRALPHABET		symbol, ...							// symbols
			);

		char*				getChain(char* b);
		static GRALPHABET	terminal(char terminal);
		static GRALPHABET	nonterminal(char nonterminal);
		static bool			isTerminal(GRALPHABET s);
		static bool			isNonterminal(GRALPHABET s);
		static char			alphabetToChar(GRALPHABET s);

		~Chain();

	private:
		short				size_;
		GRALPHABET*			chain_;								// chain of terminals & nonterminals
	};

	class Rule{
	public:
		Rule();
		Rule(
			GRALPHABET		nonterminal,						// 
			int				errorId,							// 
			short			numberOfChains,
			Chain*			chain, ...							// 
			);

		short getNextChain(
			GRALPHABET		firstSymbol,
			Chain&			chain,
			short			index								//
			);

		~Rule();

	private:
		Chain* chains_;
		GRALPHABET			nonterminal_;						// nonterminal < 0
		int					errorId_;
		short				size_;								// number of chains - right part of rule

	};

	class Greibach
	{
	public:
		Greibach();
		Greibach(
			GRALPHABET		startSymbol,
			GRALPHABET		stackBottom,
			short			numberOfRules,
			Rule			rule, ...
			);

		short getRule(
			GRALPHABET		leftSymbol,							//
			Rule&			rule
			);
		Rule				getRule(short index);

		~Greibach();

	private:
		short				size_;
		GRALPHABET			startSymbol_;
		GRALPHABET			stackBottom_;
		Rule*				rules_;
	};
}