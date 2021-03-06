#define BOOST_TEST_MODULE AES_Test_Module

#include "stdafx.h"
#include "../AES/AES.hpp"

struct AES_Fixture
{
public:
	AES_Fixture()
	{
		testAES = new AES;
	}
	~AES_Fixture()
	{
		delete testAES;
	}

	AES * testAES;
};

bool compare(uchar first[][4], uchar second[][4])
{
	int ret = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (first[i][j] != second[i][j])
			{
				ret = false;
			}
		}
	}

	return ret;
}


BOOST_FIXTURE_TEST_SUITE(AES_Test_Suite, AES_Fixture)

BOOST_AUTO_TEST_CASE( DncSubFunctions )  
{
     // SubBytes
	uchar state_subBytes[][4] = 
	{
		{ 0x19, 0xa0, 0x9a, 0xe9 },
		{ 0x3d, 0xf4, 0xc6, 0xf8 },
		{ 0xe3, 0xe2, 0x8d, 0x48 },
		{ 0xbe, 0x2b, 0x2a, 0x08 }
	};
	uchar outState_subBytes[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0x27, 0xbf, 0xb4, 0x41 },
		{ 0x11, 0x98, 0x5d, 0x52 },
		{ 0xae, 0xf1, 0xe5, 0x30 }
	};
	testAES->SubBytes(state_subBytes);
	BOOST_REQUIRE(compare(state_subBytes, outState_subBytes) == true);

	// ShiftRows
	uchar state_shiftRows[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0x27, 0xbf, 0xb4, 0x41 },
		{ 0x11, 0x98, 0x5d, 0x52 },
		{ 0xae, 0xf1, 0xe5, 0x30 }
	};
	uchar outState_shiftRows[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0xbf, 0xb4, 0x41, 0x27 },
		{ 0x5d, 0x52, 0x11, 0x98 },
		{ 0x30, 0xae, 0xf1, 0xe5 }
	};
	testAES->ShiftRows(state_shiftRows);
	BOOST_REQUIRE(compare(state_shiftRows, outState_shiftRows) == true);

	// MixColumns
	uchar state_mixColumns[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0xbf, 0xb4, 0x41, 0x27 },
		{ 0x5d, 0x52, 0x11, 0x98 },
		{ 0x30, 0xae, 0xf1, 0xe5 }
	};
	uchar outState_mixColumns[][4] =
	{
		{ 0x04, 0xe0, 0x48, 0x28 },
		{ 0x66, 0xcb, 0xf8, 0x06 },
		{ 0x81, 0x19, 0xd3, 0x26 },
		{ 0xe5, 0x9a, 0x7a, 0x4c }
	};
	testAES->MixColumns(state_mixColumns);
	BOOST_REQUIRE(compare(state_mixColumns, outState_mixColumns) == true);

	// AddRoundKey
	uchar state_addRoundKey[][4] =
	{
		{ 0x04, 0xe0, 0x48, 0x28 },
		{ 0x66, 0xcb, 0xf8, 0x06 },
		{ 0x81, 0x19, 0xd3, 0x26 },
		{ 0xe5, 0x9a, 0x7a, 0x4c }
	};
	uchar state_Key[][4] =
	{
		{ 0xa0, 0x88, 0x23, 0x2a },
		{ 0xfa, 0x54, 0xa3, 0x6c },
		{ 0xfe, 0x2c, 0x39, 0x76 },
		{ 0x17, 0xb1, 0x39, 0x05 }
	};
	uchar outState_addRoundKey[][4] =
	{
		{ 0xa4, 0x68, 0x6b, 0x02 },
		{ 0x9c, 0x9f, 0x5b, 0x6a },
		{ 0x7f, 0x35, 0xea, 0x50 },
		{ 0xf2, 0x2b, 0x43, 0x49 }
	};
	testAES->AddRoundKey(state_addRoundKey, state_Key);
	BOOST_REQUIRE(compare(state_addRoundKey, outState_addRoundKey) == true);

}


BOOST_AUTO_TEST_CASE( KeyExpansion )
{
	// KeyExpansion
	uchar key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uchar finalKey[16] ={ 0xd0, 0x14, 0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6 };
	testAES->KeyExpansion(key);
	uchar * retKey = retRoundKey(10, *testAES);
	for (int i = 0; i < 16; i++)
	{
		BOOST_REQUIRE(retKey[i] == finalKey[i]);
	}
	delete retKey;

}

BOOST_AUTO_TEST_CASE( DecFunctions )
{
	// InvSubBytes
	uchar state_invSubBytes[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0x27, 0xbf, 0xb4, 0x41 },
		{ 0x11, 0x98, 0x5d, 0x52 },
		{ 0xae, 0xf1, 0xe5, 0x30 }
	};
	uchar outState_invSubBytes[][4] =
	{
		{ 0x19, 0xa0, 0x9a, 0xe9 },
		{ 0x3d, 0xf4, 0xc6, 0xf8 },
		{ 0xe3, 0xe2, 0x8d, 0x48 },
		{ 0xbe, 0x2b, 0x2a, 0x08 }
	};
	testAES->InvSubBytes(state_invSubBytes);
	BOOST_REQUIRE(compare(state_invSubBytes, outState_invSubBytes) == true);

	// InvShiftRows
	uchar state_invShiftRows[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0xbf, 0xb4, 0x41, 0x27 },
		{ 0x5d, 0x52, 0x11, 0x98 },
		{ 0x30, 0xae, 0xf1, 0xe5 }
	};
	uchar outState_invShiftRows[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0x27, 0xbf, 0xb4, 0x41 },
		{ 0x11, 0x98, 0x5d, 0x52 },
		{ 0xae, 0xf1, 0xe5, 0x30 }
	};
	testAES->InvShiftRows(state_invShiftRows);
	BOOST_REQUIRE(compare(state_invShiftRows, outState_invShiftRows) == true);

	// InvMixColumns
	uchar state_invMixColumns[][4] =
	{
		{ 0x04, 0xe0, 0x48, 0x28 },
		{ 0x66, 0xcb, 0xf8, 0x06 },
		{ 0x81, 0x19, 0xd3, 0x26 },
		{ 0xe5, 0x9a, 0x7a, 0x4c }
	};
	uchar outState_invMixColumns[][4] =
	{
		{ 0xd4, 0xe0, 0xb8, 0x1e },
		{ 0xbf, 0xb4, 0x41, 0x27 },
		{ 0x5d, 0x52, 0x11, 0x98 },
		{ 0x30, 0xae, 0xf1, 0xe5 }
	};
	testAES->InvMixColumns(state_invMixColumns);
	BOOST_REQUIRE(compare(state_invMixColumns, outState_invMixColumns) == true);

}

BOOST_AUTO_TEST_CASE( Encription )
{
	// Key
	uchar key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

	// Case 1
	uchar text1[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	uchar comCipher1[16] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
	uint len = 16;
	uchar * outCipher1 = testAES->Cipher(len, text1, key);
	for (int i = 0; i < 16; i++)
	{
		BOOST_REQUIRE(outCipher1[i] == comCipher1[i]);
	}
	delete outCipher1;

	// Case 2
	uchar text2[32] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34, 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	uchar comCipher2[32] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32, 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
	len = 32;
	uchar * outCipher2 = testAES->Cipher(len, text2, key);
	for (int i = 0; i < 32; i++)
	{
		BOOST_REQUIRE(outCipher2[i] == comCipher2[i]);
	}
	delete outCipher2;

	// Case 3
	uchar text3[27] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34, 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98 };
	uchar comCipher3[32] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32, 0xe7, 0xde, 0x87, 0x64, 0x8c, 0xa1, 0xa0, 0x0d, 0x40, 0x5a, 0xed, 0xcb, 0x1e, 0x9f, 0x6d, 0x7d };
	len = 27;
	uchar * outCipher3 = testAES->Cipher(len, text3, key);
	for (int i = 0; i < len; i++)
	{
		BOOST_REQUIRE(outCipher3[i] == comCipher3[i]);
	}
	delete outCipher3;

}

BOOST_AUTO_TEST_CASE( Decription )
{
	// Key
	uchar key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

	// Case 1
	uchar cipher1[16] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
	uchar comText1[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	uint len = 16;
	uchar * outText1 = testAES->InvCipher(len, cipher1, key);
	for (int i = 0; i < 16; i++)
	{
		BOOST_REQUIRE(outText1[i] == comText1[i]);
	}
	delete outText1;

	// Case 2
	uchar cipher2[32] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32, 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
	uchar comText2[32] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34, 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	len = 32;
	uchar * outText2 = testAES->InvCipher(len, cipher2, key);
	for (int i = 0; i < 32; i++)
	{
		BOOST_REQUIRE(outText2[i] == comText2[i]);
	}
	delete outText2;

	// Case 3
	uchar cipher3[32] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32, 0xe3, 0xdd, 0xb5, 0xeb, 0xcf, 0x8f, 0xc1, 0x90, 0x2b, 0x08, 0xc0, 0x1b, 0x6c, 0x79, 0x95, 0xdf };
	uchar comText3[32] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34, 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98 };
	len = 32;
	uchar * outText3 = testAES->InvCipher(len, cipher3, key);
	for (int i = 0; i < len; i++)
	{
		BOOST_REQUIRE(outText3[i] == comText3[i]);
	}
	delete outText3;

}

BOOST_AUTO_TEST_CASE( Another )
{
	// Key
	uchar key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uchar text[11] = { 'x', 'i', 'e', 'h', 'o', 'n', 'g', 'f', 'e', 'n', 'g'};
	uint len = 11;
	uchar cipher[16];
	uchar * outCipher = testAES->Cipher(len, text, key);
	for (int i = 0; i < len; i++)
	{
		cipher[i] = outCipher[i];
	}
	delete outCipher;

	uchar * outText = testAES->InvCipher(len, cipher, key);
	for (int i = 0; i < len; i++)
	{
		text[i] = outText[i];
	}
	delete outText;

}


BOOST_AUTO_TEST_SUITE_END()