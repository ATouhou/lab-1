/*
 * LogicProcessor.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICPROCESSOR_H_
#define LOGICPROCESSOR_H_

/**
 * LogicProcessor evaluates the truth tables provided in the form of characters
 * or other LogicProcessor objects. The best part is that it allows you to
 * combine truth tables in fun ways like, NOT ( X AND Y )
 */
class LogicProcessor {
public:
	/**
	 * Constructor
	 * @param Takes a LogicFunctionT pointer
	 * @return Returns a LogicProcessor object pointer
	 */
	LogicProcessor(LogicFunctionT *function);
	/**
	 * Destructor
	 */
	~LogicProcessor();

	/**
	 * Add another LogicProcessor object to the set of inputs
	 * @param input is number of the input to set
	 * @param takes a LogicProcessor pointer
	 */
	void setInput(int input, LogicProcessor *lf);
	/**
	 * Add another truth string the the array of inputfunctions
	 * @param input is the value to change
	 * @param the character pointer to set it to
	 */
	void setInput(int input, char* source);

	/**
	 * Take the current logic functions and evaluate the logic
	 * @return Return a character representing the result, 't', 'f', or 'x'
	 */
	char process();

private:
	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	LogicFunctionT *m_logicfunction;
};

#endif /* LOGICPROCESSOR_H_ */
