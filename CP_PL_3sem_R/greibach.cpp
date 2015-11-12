#include "stdafx.h"
#include "greibach.h"

namespace Greibach{
	Chain::Chain(){
		this->size_ = NULL;
		this->chain_ = NULL;
	}

	Chain::Chain(short numberOfSymbols, GRALPHABET symbol, ...){
		this->size_ = numberOfSymbols;

		for (int i = 0; i < numberOfSymbols; i++){
			this->chain_[i] = *(&symbol + i);
		};
	}

// 	char* Chain::getChain(char* b){
// 
// 	};

	GRALPHABET Chain::terminal(char terminal){
		return static_cast<GRALPHABET> (terminal);
	};

	GRALPHABET Chain::nonterminal(char nonterminal){
		return -(static_cast<GRALPHABET> (nonterminal));
	};

	bool Chain::isTerminal(GRALPHABET s){
		return s > 0;
	};

	bool Chain::isNonterminal(GRALPHABET s){
		return !isTerminal(s);
	};

	char Chain::alphabetToChar(GRALPHABET s){
		return isTerminal(s) ? static_cast<char> (s) : static_cast<char> (-s);
	};

	Rule::Rule(){
		this->nonterminal_ = NULL_STR;
		this->size_ = NULL;
	}

	Rule::Rule(GRALPHABET nonterminal, int errorId, short numberOfChains, Chain* chain, ...){
		this->nonterminal_ = nonterminal;
		this->errorId_ = errorId;
		this->size_ = numberOfChains;
		for (int i = 0; i < numberOfChains; i++){
			this->chains_ = (chain + i);
		};
	}
};