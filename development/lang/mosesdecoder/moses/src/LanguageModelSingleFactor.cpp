// $Id: LanguageModelSingleFactor.cpp 1227 2007-02-21 19:48:53Z hieuhoang1972 $

/***********************************************************************
Moses - factored phrase-based language decoder
Copyright (C) 2006 University of Edinburgh

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************/

#include <cassert>
#include <limits>
#include <iostream>
#include <sstream>

#include "LanguageModelSingleFactor.h"
#include "TypeDef.h"
#include "Util.h"
#include "FactorCollection.h"
#include "Phrase.h"
#include "StaticData.h"

using namespace std;

// static variable init
LanguageModelSingleFactor::State LanguageModelSingleFactor::UnknownState=0;

LanguageModelSingleFactor::LanguageModelSingleFactor(bool registerScore, ScoreIndexManager &scoreIndexManager)
:LanguageModel(registerScore, scoreIndexManager)
{
}
LanguageModelSingleFactor::~LanguageModelSingleFactor() {}


const std::string LanguageModelSingleFactor::GetScoreProducerDescription(int idx) const
{
	std::ostringstream oss;
	// what about LMs that are over multiple factors at once, POS + stem, for example?
	oss << GetNGramOrder() << "-gram LM score, factor-type=" << GetFactorType() << ", file=" << m_filePath;
	return oss.str();
} 



