#ifndef __MORGANA_BASE_STATS_FRAME_STATS_H__
#define __MORGANA_BASE_STATS_FRAME_STATS_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Stats
		{
			class FrameStats
			{
			public:

				struct Data_t
				{
					int numDrawCalls;
					int numTriangles;
					int numRenderables;
					int numShaderChanges;
					int numTexChanges;
					int numVBOChanges;
					int numIBOChanges;
					int numGameObjects;
					int numCanvasBatches;

					Data_t()
					{
						Reset();
					}

					void Reset()
					{
						numDrawCalls = 0;
						numTriangles = 0;
						numRenderables = 0;
						numShaderChanges = 0;
						numTexChanges = 0;
						numVBOChanges = 0;
						numIBOChanges = 0;
						numGameObjects = 0;
						numCanvasBatches = 0;
					}
				};

				static Data_t current, prev;
				
				static void Reset()
				{
					prev = current;
					current.Reset();
				}

				static void Update()
				{

				}
			};
		}
	}
}

#endif