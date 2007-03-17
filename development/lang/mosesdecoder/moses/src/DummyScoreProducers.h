// $Id: DummyScoreProducers.h 1227 2007-02-21 19:48:53Z hieuhoang1972 $

#ifndef _DUMMY_SCORE_PRODUCERS_H_
#define _DUMMY_SCORE_PRODUCERS_H_

#include "ScoreProducer.h"

class WordsRange;

/** Calculates Distortion scores
 */
class DistortionScoreProducer : public ScoreProducer {
public:
	DistortionScoreProducer(ScoreIndexManager &scoreIndexManager);

	float CalculateDistortionScore(const WordsRange &prev, const WordsRange &curr) const;

	size_t GetNumScoreComponents() const;
	const std::string GetScoreProducerDescription(int idx = 0) const;
};

/** Doesn't do anything but provide a key into the global
 * score array to store the word penalty in.
 */
class WordPenaltyProducer : public ScoreProducer {
public:
	WordPenaltyProducer(ScoreIndexManager &scoreIndexManager);

	size_t GetNumScoreComponents() const;
	const std::string GetScoreProducerDescription(int idx = 0) const;
};

/** unknown word penalty */
class UnknownWordPenaltyProducer : public ScoreProducer {
public:
	UnknownWordPenaltyProducer(ScoreIndexManager &scoreIndexManager);

	size_t GetNumScoreComponents() const;
	const std::string GetScoreProducerDescription(int idx = 0) const;
};

#endif
