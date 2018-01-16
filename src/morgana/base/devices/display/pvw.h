#ifndef __MORGANA_BASE_DEVICES_DISPLAY_PVW_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_PVW_H__

#include "memath.h"
#include "rendepipelinedefs.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				struct DisplayPVWMatrices
				{
					DisplayPVWMatrices()
					{
						changes = 0;
						for(int i = 0; i < 3; i++)
							matrices[i] = Matrix::Identity;
						
						projectionViewWorld = Matrix::Identity;
						viewWorld = Matrix::Identity;
					}

					inline void				LoadMatrix(const int which, const Matrix* m) { matrices[which] = *m; changes++; }
					inline const Matrix&	GetMatrix(const int which) const { return matrices[which]; }

					inline void				PushMatrix(const int which) { stacks[which].Add(matrices[which]); }
					inline void				PopMatrix(const int which) { LoadMatrix(which, &stacks[which].Pop()); }

					inline void				MultMatrix(const int which, const Matrix* m) { matrices[which] = Matrix::Multiply(&matrices[which], m); }

					inline const Matrix&	GetViewWorld()
					{
						return viewWorld;
					}

					inline const Matrix&	GetProjectionViewWorld()
					{
						if(changes > 0)
						{
							UpdateProjectionViewWorld();
							changes = 0;
						}

						return projectionViewWorld;
					}

				private:
					Matrix			matrices[3];
					Array<Matrix>	stacks[3];

					Matrix			projectionViewWorld, viewWorld;
					int				changes;

					inline const void UpdateProjectionViewWorld()
					{
						Matrix::Multiply(&matrices[MATRIX_VIEW], &matrices[MATRIX_WORLD], &viewWorld);
						Matrix::Multiply(&matrices[MATRIX_PROJECTION], &viewWorld, &projectionViewWorld);
					}
				};
			}
		}
	}
}

#endif