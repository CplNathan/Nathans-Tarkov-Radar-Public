#pragma once
#include "Base.h"
#include "TarkovSkeletonRoot.h"

class TarkovSkeletonRoot : public MemoryObject
{
public:
	TarkovSkeletonRoot(WinProcess *GameProcess, uint64_t Address)
		: MemoryObject(GameProcess, Address)
	{
	}

	Matrix3x4 GetLocationMatrixTest()
	{
		uint64_t m_BoneEnum = GameProcess->Read<uint64_t>(Address + 0x28);
		uint64_t m_TransformArray = GameProcess->Read<uint64_t>(m_BoneEnum + 0x10);
		uint64_t m_TransformA = GameProcess->Read<uint64_t>(m_TransformArray + 0x20);
		uint64_t m_internalTransformA = GameProcess->Read<uint64_t>(m_TransformA + 0x10);
		uint64_t m_transformAccessReadOnlyData = GameProcess->Read<uint64_t>(m_internalTransformA + 0x38);

		uint64_t m_transformDataArray = GameProcess->Read<uint64_t>(m_transformAccessReadOnlyData + 0x18);
		//uint64_t m_transformDataIndices = GameProcess->Read<uint64_t>(m_transformAccessReadOnlyData + 0x20);

		Matrix3x4 locMatrixDebug = GameProcess->Read<Matrix3x4>(m_transformDataArray); /* 0x40 between matrix's */

		/*
        https://www.unknowncheats.me/forum/2332931-post688.html
        https://www.unknowncheats.me/forum/2432035-post995.html
        */
	   
		return locMatrixDebug;
	}

	Vector3 GetCoordsFromMatrixTest()
	{
		Vector3 newCoords;
		Matrix3x4 oldMatrix = GetLocationMatrixTest();

		newCoords.x = oldMatrix.A.x;
		newCoords.y = oldMatrix.A.y;
		newCoords.z = oldMatrix.A.z;

		return newCoords;
	}
};
