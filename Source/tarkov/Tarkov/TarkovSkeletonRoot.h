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

/*
// Generated using ReClass 2016

class LocalGameWorld;
class N00000012;
class N0000003B;
class N0000004E;
class N0000005B;
class N0000006E;
class N0000007A;
class N0000008E;
class N000000A9;
class N000000CC;
class N000000DF;
class N000000F3;
class N00000107;
class N00000136;
class N00000149;
class N00000164;
class N00000177;
class N0000018A;
class N000001A5;
class N000001C0;
class N000001DB;
class N000001FF;
class N0000022E;
class N00000241;
class N00000255;
class N00000270;
class N00000273;
class N00000296;
class N000002A9;
class N000002C4;
class N000002D7;
class N000002EA;
class N000002FD;
class N00000318;
class N00000325;
class N0000036A;
class N0000036D;
class N00000389;
class N000003A6;
class N000003CB;
class N000003F7;
class N00000412;
class N00000562;
class N00000585;
class N000005A8;
class N000005C3;
class N00000617;
class N00000632;
class N0000064D;
class N000006F1;
class N00000715;
class N00000731;
class N0000074C;
class N0000074F;
class N00000769;
class N00000773;
class N000007A5;
class N000007A8;
class N000007AC;
class N00000848;
class N00000954;

class LocalGameWorld
{
public:
	char pad_0x0000[0x68]; //0x0000
	N00000012* PlayerList; //0x0068 
	char pad_0x0070[0x10]; //0x0070

}; //Size=0x0080

class N00000012
{
public:
	char pad_0x0000[0x10]; //0x0000
	N0000005B* PlayerArray; //0x0010 
	__int32 PlayerCount; //0x0018 
	char pad_0x001C[0xAC]; //0x001C

}; //Size=0x00C8

class N0000003B
{
public:
	char pad_0x0000[0x68]; //0x0000
	N0000004E* N00000049; //0x0068 
	char pad_0x0070[0x18]; //0x0070

}; //Size=0x0088

class N0000004E
{
public:
	char pad_0x0000[0x18]; //0x0000
	__int32 N00000052; //0x0018 
	char pad_0x001C[0x2C]; //0x001C

}; //Size=0x0048

class N0000005B
{
public:
	char pad_0x0000[0x20]; //0x0000
	N0000006E* FirstPlayer; //0x0020 
	N000007A5* SecondPlayer; //0x0028 
	N000007A8* ThirdPlayer; //0x0030 
	char pad_0x0038[0x50]; //0x0038

}; //Size=0x0088

class N0000006E
{
public:
	char pad_0x0000[0x38]; //0x0000
	N000007AC* MovementContext; //0x0038 
	char pad_0x0040[0x48]; //0x0040
	N00000562* PlayerBody; //0x0088 
	char pad_0x0090[0x4B0]; //0x0090

}; //Size=0x0540

class N0000007A
{
public:
}; //Size=0x0000

class N0000008E
{
public:
	char pad_0x0000[0x40]; //0x0000
	N0000036D* N00000097; //0x0040 
	char pad_0x0048[0x40]; //0x0048
	N000000A9* N000000A0; //0x0088 
	char pad_0x0090[0x38]; //0x0090

}; //Size=0x00C8

class N000000A9
{
public:
	char pad_0x0000[0x38]; //0x0000
	N0000036A* N000000B1; //0x0038 
	char pad_0x0040[0x48]; //0x0040

}; //Size=0x0088

class N000000CC
{
public:
	char pad_0x0000[0x28]; //0x0000
	N000000DF* N000000D2; //0x0028 
	char pad_0x0030[0x58]; //0x0030

}; //Size=0x0088

class N000000DF
{
public:
	char pad_0x0000[0x28]; //0x0000
	N000000F3* N000000E5; //0x0028 
	char pad_0x0030[0x60]; //0x0030

}; //Size=0x0090

class N000000F3
{
public:
	char pad_0x0000[0x10]; //0x0000
	N00000107* N000000F6; //0x0010 
	char pad_0x0018[0x70]; //0x0018

}; //Size=0x0088

class N00000107
{
public:
	char pad_0x0000[0x18]; //0x0000
	matrix3x4_t N0000012F; //0x0018 

}; //Size=0x0058

class N00000136
{
public:
	char pad_0x0000[0x18]; //0x0000
	N00000149* N0000013A; //0x0018 
	N0000018A* N0000013B; //0x0020 
	char pad_0x0028[0x60]; //0x0028

}; //Size=0x0088

class N00000149
{
public:
	char pad_0x0000[0x80]; //0x0000

}; //Size=0x0080

class N00000164
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N00000177
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N0000018A
{
public:
	N00000255* N0000018B; //0x0000 
	char pad_0x0008[0x10]; //0x0008
	__int64 N0000018E; //0x0018 
	N000001A5* N0000018F; //0x0020 
	N00000270* N00000190; //0x0028 
	N00000273* N00000191; //0x0030 
	char pad_0x0038[0x90]; //0x0038

}; //Size=0x00C8

class N000001A5
{
public:
	char pad_0x0000[0x28]; //0x0000
	N0000022E* N00000222; //0x0028 
	N00000241* N00000223; //0x0030 
	char pad_0x0038[0x48]; //0x0038

}; //Size=0x0080

class N000001C0
{
public:
	char pad_0x0000[0xC8]; //0x0000

}; //Size=0x00C8

class N000001DB
{
public:
	char pad_0x0000[0x10]; //0x0000
	N000001FF* N000001DE; //0x0010 
	char pad_0x0018[0xF0]; //0x0018

}; //Size=0x0108

class N000001FF
{
public:
	char pad_0x0000[0x10]; //0x0000
	__int32 N00000202; //0x0010 
	char pad_0x0014[0x34]; //0x0014

}; //Size=0x0048

class N0000022E
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N00000241
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N00000255
{
public:
	char pad_0x0000[0xC8]; //0x0000

}; //Size=0x00C8

class N00000270
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N00000273
{
public:
	char pad_0x0000[0x10]; //0x0000
	N000002A9* N00000276; //0x0010 
	char pad_0x0018[0x10]; //0x0018
	N00000296* N00000279; //0x0028 
	char pad_0x0030[0x58]; //0x0030

}; //Size=0x0088

class N00000296
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N000002A9
{
public:
	char pad_0x0000[0x30]; //0x0000
	N000002C4* N000002B0; //0x0030 
	char pad_0x0038[0x90]; //0x0038

}; //Size=0x00C8

class N000002C4
{
public:
	char pad_0x0000[0x30]; //0x0000
	N000002D7* N000002CB; //0x0030 
	char pad_0x0038[0x50]; //0x0038

}; //Size=0x0088

class N000002D7
{
public:
	char pad_0x0000[0x8]; //0x0000
	N000002EA* N000002D9; //0x0008 
	char pad_0x0010[0x78]; //0x0010

}; //Size=0x0088

class N000002EA
{
public:
	char pad_0x0000[0x38]; //0x0000
	N000002FD* N000002F2; //0x0038 
	char pad_0x0040[0x48]; //0x0040

}; //Size=0x0088

class N000002FD
{
public:
	char pad_0x0000[0x90]; //0x0000
	Vector3 N00000310; //0x0090 
	char pad_0x009C[0x2C]; //0x009C

}; //Size=0x00C8

class N00000318
{
public:
	char pad_0x0000[0x48]; //0x0000

}; //Size=0x0048

class N00000325
{
public:
	char pad_0x0000[0x1E0]; //0x0000
	Vector3 N00000362; //0x01E0 
	char pad_0x01EC[0x1C]; //0x01EC

}; //Size=0x0208

class N0000036A
{
public:
	char pad_0x0000[0x8]; //0x0000

}; //Size=0x0008

class N0000036D
{
public:
	char pad_0x0000[0xC8]; //0x0000

}; //Size=0x00C8

class N00000389
{
public:
	char pad_0x0000[0x40]; //0x0000
	N000003A6* N0000039B; //0x0040 
	char pad_0x0048[0x40]; //0x0048

}; //Size=0x0088

class N000003A6
{
public:
	char pad_0x0000[0x18]; //0x0000
	N000003CB* N000003BD; //0x0018 
	char pad_0x0020[0x60]; //0x0020

}; //Size=0x0080

class N000003CB
{
public:
	matrix3x4_t N000003CC; //0x0000 
	char pad_0x0040[0x48]; //0x0040

}; //Size=0x0088

class N000003F7
{
public:
	char pad_0x0000[0x100]; //0x0000

}; //Size=0x0100

class N00000412
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N00000562
{
public:
	char pad_0x0000[0x28]; //0x0000
	N00000585* SkeletonRoot; //0x0028 
	char pad_0x0030[0xD8]; //0x0030

}; //Size=0x0108

class N00000585
{
public:
	char pad_0x0000[0x28]; //0x0000
	N000005A8* BoneEnumerator; //0x0028 
	char pad_0x0030[0xD8]; //0x0030

}; //Size=0x0108

class N000005A8
{
public:
	char pad_0x0000[0x10]; //0x0000
	N000005C3* TransformArray; //0x0010 
	char pad_0x0018[0xB0]; //0x0018

}; //Size=0x00C8

class N000005C3
{
public:
	char pad_0x0000[0x20]; //0x0000
	N000006F1* Transform; //0x0020 
	char pad_0x0028[0x8]; //0x0028
	N00000617* N000005FC; //0x0030 
	char pad_0x0038[0xC8]; //0x0038

}; //Size=0x0100

class N00000617
{
public:
	char pad_0x0000[0xC8]; //0x0000

}; //Size=0x00C8

class N00000632
{
public:
	char pad_0x0000[0x40]; //0x0000
	N0000064D* N0000063B; //0x0040 
	char pad_0x0048[0x80]; //0x0048

}; //Size=0x00C8

class N0000064D
{
public:
	matrix3x4_t N0000064E; //0x0000 
	char pad_0x0040[0x48]; //0x0040

}; //Size=0x0088

class N000006F1
{
public:
	char pad_0x0000[0x10]; //0x0000
	N00000715* internalTransform; //0x0010 
	char pad_0x0018[0xB0]; //0x0018

}; //Size=0x00C8

class N00000715
{
public:
	char pad_0x0000[0x38]; //0x0000
	N00000731* transformAccessReadOnlyData; //0x0038 
	__int64 N0000071E; //0x0040 
	char pad_0x0048[0x80]; //0x0048

}; //Size=0x00C8

class N00000731
{
public:
	char pad_0x0000[0x18]; //0x0000
	N0000074C* transformDataArray; //0x0018 
	N0000074F* transformDataIndices ; //0x0020 
	char pad_0x0028[0xA0]; //0x0028

}; //Size=0x00C8

class N0000074C
{
public:
	matrix3x4_t N0000074D; //0x0000 
	matrix3x4_t N00000758; //0x0040 
	matrix3x4_t N00000760; //0x0080 
	matrix3x4_t N00000768; //0x00C0 
	matrix3x4_t N0001D033; //0x0100 
	matrix3x4_t N0001D03B; //0x0140 
	matrix3x4_t N0001D045; //0x0180 
	char pad_0x01C0[0x8]; //0x01C0

}; //Size=0x01C8

class N0000074F
{
public:
	char pad_0x0000[0x88]; //0x0000

}; //Size=0x0088

class N00000769
{
public:
	N00000773* N0000076A; //0x0000 
	char pad_0x0008[0x78]; //0x0008

}; //Size=0x0080

class N00000773
{
public:
	matrix3x4_t N00000774; //0x0000 
	char pad_0x0040[0x48]; //0x0040

}; //Size=0x0088

class N000007A5
{
public:
	char pad_0x0000[0x8]; //0x0000

}; //Size=0x0008

class N000007A8
{
public:
	char pad_0x0000[0x8]; //0x0000

}; //Size=0x0008

class N000007AC
{
public:
	char pad_0x0000[0x1E0]; //0x0000
	Vector3 N000007E9; //0x01E0 
	char pad_0x01EC[0x2DC]; //0x01EC

}; //Size=0x04C8

class N00000848
{
public:
	N00000954* N00000849; //0x0000 
	char pad_0x0008[0x1D8]; //0x0008
	Vector3 N00000885; //0x01E0 
	char pad_0x01EC[0x654]; //0x01EC

}; //Size=0x0840

class N00000954
{
public:
	char pad_0x0000[0x1E0]; //0x0000
	Vector3 N00000991; //0x01E0 
	char pad_0x01EC[0x65C]; //0x01EC

}; //Size=0x0848
*/