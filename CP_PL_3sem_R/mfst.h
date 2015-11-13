#pragma once

#include "greibach.h"
#include "lexAnalyser.h"
#include <stack>

#define MFST_DIAGNOSTIC_NUMBER 3

typedef std::stack<short> STACK;

namespace MFST{
	class State{
	public:
		State();
		State(
			short				tapePosition,
			STACK				stack,
			short				ruleNumber
			);

	private:
		short					tapePosition_;			// position in a tape
		short					ruleNumber_;			// number of current rule
	};



	class Mfst{
	public:
		enum STEP_RC{
			NS_OK,
			NS_NO_RULE,
			NS_NO_CHAIN_RULE,
			NS_ERROR,
			TS_OK,
			TS_NOT_OK,
			TAPE_END,
			SURPRISE
		};

		Mfst();
		Mfst(
			LA::LexAnalyser		la,
			Greibach::Greibach	greibachGrammar
			);

		char*					getStack(char* buf);
		char*					getTape(char* buf, short pos, short n = 25);
		char*					getDiagnostics(short n, char* buf);
		
		bool					saveState();
		bool					restoreState();
		bool					pushChain(
										Greibach::Chain chain
										);

		STEP_RC					step();
		bool					start();
		bool					savedDiagnostics(
												STEP_RC stepEndCode
												);

	private:
		GRALPHABET*				tape_;
		short					tapePosition_;
		short					ruleNumber_;
		short					ruleChainNumber_;
		short					tapeSize_;
		Greibach::Greibach		greibach_;
		//LA::LexAnalyser			la_;
		STACK					stack_;
		std::stack<State>		stateStack_;

		class Diagnostics{
		public:
			Diagnostics();
			Diagnostics(
				short			tapePosition,
				STEP_RC			stepRc,
				short			ruleNumber,
				short			ruleChainNumber
				);

		private:
			short				tapePosition_;
			STEP_RC				stepRc_;
			short				ruleNumber_;
			short				ruleChainNumber_;
		}						diagnostics_;
	};
};