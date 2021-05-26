
#include "FramesStat.h"
#include "cs.h"
#include <assert.h>
#include <algorithm>


using namespace std;
using namespace cs;

FramesStat::FramesStat()
{
}

FramesStat::FramesStat(const CubeFrames & frames)
{
	analyze(frames);
}

bool FramesStat::analyze(const CubeFrames & frames)
{
	m_isAnalyzed = false;

	if (frames.empty())
		return false;


	double total_time = 0.0;
	int step = 0;
	int rot = 0;

	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		const cf_record_t& rec = frames.records[i];

		total_time += rec.ds_time / 1000.0;

	}


	m_isAnalyzed = true;
	return true;
}
