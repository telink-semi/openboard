/********************************************************************************************************
 * @file    app_rsa.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
/********* pke version:1.1 *********/
#include "app_config.h"


#if(MCU_CORE_B92||  MCU_CORE_B93)
#if (PKE_TEST_MODE == PKE_RSA)

#if 1

unsigned int rsa_modexp_1024_test(void)
{
	unsigned int n[]=
	{
		0xFEA18675,0x1538B245,0x84E8E165,0x91BEB0D8,0x9D0B6997,0x1D7B833A,0xEFA2C3F6,0x7DF45F14,
		0x55565D78,0x69230B98,0x591F158B,0xE106BDD8,0x26AF59F0,0xE430C55E,0xB9EE5D48,0xC74A11EF,
		0x16D22D06,0xC453225E,0xEF660075,0x7119E5DB,0x57111778,0x66DAC542,0x0E2A4D2E,0xA6A47646,
		0x04D816F2,0xB5704D41,0x282C21EE,0x9D950126,0xA69A2B1C,0xEADB452B,0x0CF439F0,0xBEC7A940,
	};
	unsigned int e[]= {0x00010001};
	unsigned int d[]=
	{
		0x39CD7DE1,0x765A9FBE,0x28A3F778,0x9C0BBB36,0x92F2B6D6,0xA7B0D6E6,0x4AC3AC9D,0x495F0629,
		0x2B15D590,0x1DEA94F0,0x09655DB7,0x4A2A74BE,0xA830AEE4,0xA5C64770,0x36013850,0xF0667BA8,
		0xF32A533E,0x882FD8F7,0x045FBEC6,0x132E91D0,0xC83A5CE5,0xEBCEC702,0xE2698BC8,0xA8CE2AE4,
		0xB91FAE10,0xA06B202B,0xD8013303,0x1E06979A,0xC5FAE3E4,0xDAD7E244,0xD8366EDA,0xBC5D2053,
	};
	unsigned int p[]=
	{
		0x0C3B82FD,0x563A7F00,0x4C43666F,0xBC274F20,0x30247539,0x071A594D,0x0E069034,0x184B665F,
		0x8421D20C,0xBD3B7A16,0x0F45B041,0x6B1BA853,0xEC556813,0x3B21167E,0x2D08AE4A,0xE5324D00,
	};
	unsigned int q[]=
	{
		0x7A70D6D9,0xD91EB87C,0xA610AF00,0x2664B357,0x250F0450,0x963FBF1F,0xC423CD7D,0xE12EA919,
		0x1B0EB235,0x7F77F947,0x4A17C833,0x84715F7A,0xF264953B,0xC0695F17,0x75796707,0xD5173E2C,
	};
	unsigned int dp[]=
	{
		0x8AF1926D,0xE6B3D129,0x2BCEE822,0x8F142AB6,0xA9A1922B,0x36FB2DD9,0x50BA4D2D,0x43D30545,
		0xF4A72E1B,0xFB17B678,0x117F6668,0x3E3DB8E1,0x49818B30,0x26BB5E12,0x949AB54F,0x14B89077,
	};
	unsigned int dq[]=
	{
		0x69E1C5B1,0x12B84DA8,0x823396C5,0xF1EB5D37,0x8E8FF40D,0x9260C390,0x48E1B1D6,0xFCA4FE82,
		0x2F02230F,0x38A57BF4,0x99B73F12,0xCA14A8FF,0xCB3D116D,0xDCAA4637,0x7C7AA349,0x4A1674B5,
	};
	unsigned int u[]=
	{
		0xA2964E5B,0xF37FB26D,0x8B15254C,0xCAFAAD33,0xE3B1270B,0xCD27513C,0x7385C488,0x48899F5B,
		0x2234D1AA,0x893B7E4D,0xB343300B,0xEDA155DE,0x53C472E3,0x1602F8E0,0xBFE44F44,0x3FB0D177,
	};
	unsigned int std_out[]=
	{
		0x6918BEE1,0x5A78A5AE,0x537C93F8,0xBADE3C88,0x906FD649,0x082DA994,0x10BC7D78,0xE02469F7,
		0x20F71F91,0x9D56188D,0xA1933733,0x3A0FE3FD,0xFA551EC2,0x0F7BD1AC,0xDA221E31,0x60DA6F4F,
		0xC424F841,0xDB256AAD,0xB3C3B259,0x5480DDC6,0xBCF51563,0x9A245963,0x3106369F,0xDAD4C504,
		0x9C960A3B,0x357044F9,0x95F5521A,0x9F6D9285,0xC13468B2,0x8CD09D47,0x45A4B220,0x5351A82E,
	};


	unsigned int in[128], out[128], out2[128];
	unsigned int eBitLen = 17;
	unsigned int nBitLen = 1024;
	unsigned int wordLen=(nBitLen+31)/32;
	unsigned int ret;

	//set input
	memset(in, 0x5a, wordLen<<2);
	//sign(non-CRT mode)
//printf("\r\n RSA_ModExp start!");
	ret = RSA_ModExp(in, d, n, out, get_valid_bits(d, wordLen), nBitLen);
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA MOD EXP 1024 test failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 1024 test success");
	}
	//sign(CRT mode)
	memset(out, 0, wordLen<<2);
//printf("\r\n RSA_CRTModExp start!");
	ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA CRT MOD EXP 1024 test failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA CRT MOD EXP 1024 test success");
	}

	//verify
//printf("\r\n RSA_ModExp start!");
	ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out2, wordLen, in, wordLen))
	{
		printf("\r\n RSA MOD EXP 1024 test failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 1024 test success");
	}

	return 0;
}
#if (MCU_CORE_B93)
unsigned int rsa_modexp_2048_test()
{
	//n is 0xA7C5D82068EC9949...3958A753BB5248B9
	unsigned int n[]=
	{
		0xBB5248B9,0x3958A753,0x8374E33D,0x361B77F2,0xCB85F949,0xCA8E668C,0x66EDE655,0x3DDEE69E,
		0x8AACE682,0x99F46E90,0x4492985F,0x29F0F9C0,0xEBF0722B,0xE0179832,0xCF18AE20,0xE665C7C1,
		0x0CBE0F3A,0x40B1FD85,0x55F30FEC,0x667DD0CF,0x0445664A,0x324C8E9A,0x86ACD519,0xBB6B6E72,
		0xF2B5D9FE,0x147B9465,0xD1A21022,0xCBA4B564,0xFD868F9A,0x5FBDFE51,0x66635C00,0xD477C1B2,
		0xD154302A,0x60BD6712,0x40451294,0xBD3D93D6,0xB99E7133,0x42BF8DF8,0x481A2235,0x4F4290A2,
		0xFE90B45E,0x892B8244,0x0A33F5FA,0x381E10F9,0xA2B41B34,0xFCD8D1E4,0x865B96DA,0xF46D1760,
		0xCC5085D7,0x88EA540B,0xED35377E,0x07160284,0xFA1C1603,0xFB6CC61F,0x0C76B0BE,0x56353B18,
		0x3ED0F658,0x4C9FA8B4,0x59CC2863,0xC2128C81,0x371A8B13,0x6C807C1E,0x68EC9949,0xA7C5D820,
	};
	unsigned int e[]= {0x00010001};
	unsigned int d[]=
	{
		0xBFBF5039,0xE52AE46D,0x52610295,0x72AAF83F,0x28D40FA1,0x270A858B,0x8DEAB989,0x78ED0FB3,
		0x29946A0E,0xC56355E4,0x3F186B53,0xD486B4B3,0x63F56868,0xE450D0AE,0x1722616A,0x77199EFC,
		0x02FAEA03,0xBE0DA0CA,0xB94C869D,0xA9A94DD5,0x17082AB9,0xE71BEE19,0xDDF45A1D,0x7D1BE624,
		0x65397435,0x40B8B56E,0x4B591AA6,0x13FC9A6D,0x0FD48ABF,0xD7B78EDB,0x71C18BCF,0x073EA51C,
		0x6583789F,0xFA407806,0xEDC5285D,0x6DEE416C,0x5A2C7BF6,0x71BBAFF4,0xEEB3A6B3,0xA9ABFDBC,
		0x8391DB1F,0x667ACF54,0x1687748F,0x50ECE2F3,0xF7E9B284,0x9B0EDECC,0x3D4EE14C,0x5C7EE93B,
		0x0B048AC1,0x2DCE686A,0xE03B91A2,0xECBB1EF3,0xD88E255B,0xEE2863B5,0x8C9D97F5,0x062BC46F,
		0x8BE63E86,0x37A59DA4,0x126551A2,0x3B462FDE,0xEDB1B18C,0x5E487B35,0x92D2FDE8,0x7FBA29CE,
	};
	unsigned int p[]=
	{
		0xD2C5399F,0xF3EF5110,0x04970D25,0xE64CAEC5,0x5D5A068D,0xA7CE6463,0x80E71F9D,0x69331C79,
		0x45871E7C,0xA0B6B1D0,0xB5B27E5B,0x538F84A9,0x8DB003F7,0x68356ACE,0x42252C1D,0xDD889308,
		0x9D55A1E2,0xB51B0532,0x6D605FBA,0xEDAE1122,0xB5F34AF5,0xB552403E,0x7CC23ED4,0xA10075A3,
		0xB88DD8E3,0xDA3A1A7C,0x46942D7E,0x718EBCF2,0x339F1881,0xFF79A8CA,0x394262E7,0xD38DFA8E,
	};
	unsigned int q[]=
	{
		0xC39D0EA7,0x7C85F825,0x479D951E,0x65D3CC95,0xB544E463,0x5D704D69,0x7D3B641D,0xE5D7637B,
		0x3BB96DE8,0x3CF3EAFE,0x6E36C52E,0xB8148229,0xBB82F49E,0xE6A13EFE,0x5BA6BCA1,0xD0C9CCF0,
		0xE3B07AE5,0x67EEF090,0x6B9D44C0,0x6AF163AC,0x53749666,0xB90FAA34,0x770B3AB0,0x727A2482,
		0x0633A781,0x767780EA,0x1A0CBC6A,0xEB01038D,0x1E1FD9C1,0xA2D9284B,0xDED4E7A4,0xCB052788,
	};
	unsigned int dp[]=
	{
		0xDB507F89,0x23B7FCFD,0x5C1C714E,0xC6D07B7A,0x9357ABA1,0x6739F404,0x929A6045,0xCD381F53,
		0x14E8EB0D,0x0EA5CD18,0xBFCF414E,0x0067D48B,0xF19AC916,0x5A536CA3,0xD45FA412,0x47C0DF86,
		0x0B4A9B2E,0xB9644313,0xB1C99B7B,0xFA188B68,0x59063167,0x9DF56E18,0xF133AC96,0x0A299C90,
		0x61CE43C5,0x2F6C834A,0x8A572FDF,0x8EE58583,0xE8151555,0x4AE7E982,0xC79D7147,0x4CD6F8F6,
	};
	unsigned int dq[]=
	{
		0x10101715,0x0F98249C,0xBB96FB4E,0xE5BFD64A,0x6BA46F1C,0x9940C563,0x8F40084E,0xA37A7032,
		0xEDA54D31,0xAE6AC537,0x7DC043DF,0xCA9EA769,0x8E982024,0xBD410FA8,0x971F310C,0xA5EA69CF,
		0x95F7FDDA,0x7783167C,0xABA3CECF,0x0F3B41FD,0xA45AFA22,0x89F0CCF2,0xE173F472,0xB7283956,
		0x257EE399,0xDE489159,0xD8C9E2D1,0x1E7364EB,0x22564A75,0x7EBD126C,0x8041927F,0x847AF297,
	};
	unsigned int u[]=
	{
		0x273134B8,0x811F3B02,0x61385B16,0x4940D62E,0xAC11E62E,0x90657139,0xE0BA820D,0x3517F5E3,
		0xA0152677,0xFC65FE57,0xCF28A8AF,0x65E9080E,0xD88F479E,0xF9775F4D,0x60BF9490,0xB2BAEE6E,
		0x5A68CD17,0xB549B32F,0xF86E8A0F,0x0376A3B5,0xB34A8B3D,0x93B8C51A,0xE4A04408,0x2250EA7A,
		0xD905105E,0x2367E8BE,0x8C6F6643,0x0123C7C4,0xE9C2A57D,0xE3939FE8,0xFF8A801C,0xC99AC5E1,
	};
	unsigned int std_out[]=
	{
		0x65DE5107,0xEC26699D,0x0D76CE00,0x397AD40D,0x0DBD4F97,0xF30D4311,0x48CA76D0,0x5F2723C7,
		0xE3A21752,0xCB6F36A3,0xDA1204DB,0x66DF3D61,0x0334BACC,0xEC127801,0x90AD6D35,0x9307AE25,
		0x4B6F3716,0x74B04B2C,0x61E072D6,0x9DA1A085,0x4E685D85,0x640EC696,0x6AAF6343,0x87351877,
		0x70C97CE5,0x7B035CED,0xCB2C2C9F,0x17620FA2,0x724EBFBE,0x9F61B6F8,0xD58F829B,0x2F299CC5,
		0xCA5D6B61,0x2095905B,0xB3C8958A,0x36DA96E2,0x0DD9993C,0x8C8A76EC,0x91B75607,0xB25A39A3,
		0x8EDB1559,0xE5D2A164,0x3053F1B2,0x4AB1E95E,0x1EEB0CAB,0xD6EF57D0,0x5913D85F,0x64EEC60E,
		0xCFF42207,0x1F8065E2,0x6ED6A7AB,0x705F2F9E,0xD67FFDE5,0x03DAAFED,0x3372EFF6,0x4F58C1A8,
		0x3A1B21D6,0x1AD49793,0x3589FF12,0xA222521F,0x8A78096C,0x749AC189,0x411577B9,0x7FD2D10E,
	};

	unsigned int in[256], out[256], out2[256];
	unsigned int eBitLen = 17;
	unsigned int nBitLen = 2048;
	unsigned int wordLen=(nBitLen+31)/32;
	unsigned int ret;

	//get_H(n, out, wordLen);print_BN_buf_U32(n, wordLen, "n");print_BN_buf_U32(out, wordLen, "out");
	//set input
	memset(in, 0x5a, wordLen<<2);

	//sign(non-CRT mode with pub)
//printf("\r\n begin     ");
//for(int j=0;j<28;j++){
	ret = RSA_ModExp(in, d, n, out, nBitLen, nBitLen);
//}printf("\r\n end     ");
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA MOD EXP 2048 test 1 failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 2048 test 1 success");
	}

	//sign(CRT mode with pub)
	memset(out, 0, wordLen<<2);
//printf("\r\n begin     ");
//for(int j=0;j<28;j++){
	ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
//}printf("\r\n end     ");
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA CRT MOD EXP 2048 test 1 failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA CRT MOD EXP 2048 test 1 success");
	}

	//verify
//printf("\r\n begin     ");
//for(int j=0;j<800;j++){
	ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
//}printf("\r\n end     ");
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out2, wordLen, in, wordLen))
	{
		printf("\r\n RSA MOD EXP 2048 test 2 failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 2048 test 2 success");
	}

	return 0;
}

unsigned int rsa_modexp_4096_test()
{
	unsigned int n[]=
	{
		0xBB3E4243,0x27ACB0B7,0x41408F95,0x1CEBE95F,0x0AA58A89,0x9B6CF8BE,0x483629C0,0xE8921BCB,
		0x5697DB7C,0x663E2EA9,0xDC36E607,0xA738A884,0x222A3517,0x2394B61E,0x2619FAA2,0x8438C2E9,
		0x6778D284,0xA3BFAC4A,0x36B33248,0xFFFA3623,0xE9609AED,0x0B564C2F,0x1A46A2F7,0xAA09F2BF,
		0xEBB7AE47,0x0E46F157,0x9EE119FF,0xA814B8DB,0x557D4917,0x8F732EE6,0x41593C91,0x07B12ABD,
		0x6A61BD10,0x6826B484,0x661EF3F1,0xDF8E183F,0xE4EC805D,0x53A6664A,0xB5699FC4,0xF34486C6,
		0xFF728227,0x80231905,0x09FCE35E,0xAF60A847,0x3901D332,0xEB9D1233,0x922A0426,0x8863DB9E,
		0x0C2B22E8,0x4476D10C,0xE673F506,0x8EB0A4A2,0xF8A4FB18,0x2B01ACF0,0xA2D122AC,0x97E2FBC3,
		0xA40A1CE5,0x00E47563,0x25493C75,0x3C6F5989,0xAC7EE05D,0x762338B3,0xC3E03977,0x98471761,
		0xAF92D844,0xAA761798,0x49DE310E,0x795A4076,0xB6E04817,0xD7D646F9,0xED3E62B2,0xE5519DC2,
		0xA15E0C96,0x70291DD7,0xC60F6581,0xCD8F68A2,0xEB9C54A0,0x510CB72A,0xE987B728,0x13A9C60C,
		0x15AFEE45,0x16E321A5,0xEDA6CDEF,0x41D97761,0x867CB4D0,0x268B851F,0x98C1F841,0x50D86CDC,
		0xB32D5A65,0x23DF0CF1,0xF173431D,0xC03BFE22,0x96A1EF85,0x912244F7,0x784C47A0,0x244EB554,
		0xA2306EF5,0xF70DF39F,0x9A2B4CD1,0x61AC28CF,0x49C57F03,0x3DBB842E,0x62C9F6BC,0x019350C5,
		0x12EC3188,0x088DEAEA,0xB2A67CD7,0x296A1121,0x3DA0620E,0xF0F46C56,0x7237CF1C,0xAE606B95,
		0x4EEEA4B2,0xCC068682,0x318F467A,0xC029306A,0x4184C1AC,0x47DD2F6D,0xB3CDD4AE,0x6297D772,
		0x327F0C78,0x6CCF3487,0xB0E7EA88,0x41397D62,0xF7E79557,0x653F652A,0x0584075B,0x9D818C14,
	};
	unsigned int e[]= {0x00010001};
	unsigned int d[]=
	{
		0xEFC58C59,0xEF0F0517,0x2B3A0BAF,0xB50800B6,0xF4B15DBB,0x8148F0FE,0xF0140EFA,0x074E2749,
		0x41FDDFFE,0x0D89D041,0xDDEE0182,0x40535700,0xD9F22675,0xFA090359,0x61FF7C8D,0xCC133368,
		0xD559AA85,0xE6990C5A,0x628C4653,0x820DBF26,0xAE4C264B,0xF5496C09,0x20CD7BC4,0xC0F6BADA,
		0xD878EF55,0x982ECC2A,0xB7AD4CA5,0xCB015CB4,0x9DFA63B7,0xA79B6229,0xDE7ED010,0x37F5C848,
		0x50F06DC1,0xCE2AE04E,0xC0877F02,0x13983C93,0x7D29EF2F,0xE6F3BE3E,0x772BBA8C,0x78917DFA,
		0x9882F310,0x977F92EF,0xCB07458F,0x26B00182,0x5C9AF156,0x60B82EE8,0x4E2466E6,0xB635CF10,
		0xC954B4F2,0xE643F335,0x0D607F4A,0x21EADEBC,0xD0BEB2F4,0xDEB2E2D8,0xD562BAA6,0xF69C6864,
		0xB02C8342,0xDBD5B492,0x1626BB77,0xB62126A2,0x6FC8F209,0x8B6D0E1E,0xECC0CCA6,0xB4F7542A,
		0x26C43A84,0xB2CD6959,0xC8E8DAAD,0xAD99F6E3,0xECD93D00,0x97FF3F07,0x998760CF,0x3186FF73,
		0x7BF0E5EF,0x5C57C410,0x9845F32B,0xA02A9A5E,0x7C9E2482,0x4C79835A,0xD39A8B44,0xB350A8F2,
		0xBAF2E92C,0x256D00D7,0x22845A02,0x5150B016,0xF70363ED,0xE6961A30,0xFFD35B2F,0x0719767E,
		0xA8FAFEF4,0x46A4A91D,0x4240E2AE,0x710282D6,0xCBB3D733,0xFBE86F48,0x8E766BB4,0xC17CAFBF,
		0x75A2389A,0x1FFBA005,0x0E08A779,0xCFF9884A,0xC7B66636,0x67C56D89,0x7A35A643,0xE5CA3D55,
		0x1D812EBB,0x7617D532,0x1807D44F,0x34C659F1,0x05D154CB,0xEEB620F7,0xD619A46D,0xECD796E9,
		0xE036D79D,0x9D1E1EDB,0xB15D68DF,0xC6939937,0x243CAF13,0x074F98D6,0x33C267DC,0x759D2472,
		0x64E45AD9,0x150B4912,0xFBBCF752,0x755F02A0,0xD5FF377B,0xC250D628,0x3E3696BD,0x827CEC27,
	};
	unsigned int p[]=
	{
		0x6E398BED,0x32AB03AF,0xB38E38F2,0x3F2E7A3F,0x565D8581,0x8F57E4C1,0xA0E13806,0xFE3780EF,
		0x1ACE9C56,0xFC1DEC74,0x54E53C69,0xA6D87112,0x0D7A9AB5,0xD6E4E405,0x3BD3148A,0x6FA6C209,
		0xFC458EFC,0xCFFDDA6F,0x4FF3BE98,0x0A4A1C8E,0x474AFE4A,0x322433EE,0x9F0107C7,0x2FE3D0B3,
		0x3395B427,0xC92B0265,0x183A7871,0x87D42CC5,0xB24AD74F,0x7F0816CF,0x1ACDBD59,0x412DA54F,
		0x547BF34C,0x476F3BAD,0xBCC8BDC0,0x366EBDE9,0xA33EA735,0x71EB0880,0x42A68EAC,0xF3F2A351,
		0x9481A948,0xFA77BE2C,0xBF2F7316,0x78BB6637,0x542FCFB7,0xF0919C7A,0x54031904,0x09C9C73F,
		0x19F4CE39,0x8CD72063,0x0A8543D7,0xF00183F4,0x437258DB,0x0D209BB2,0xBD5E1365,0x786E14AF,
		0x6AF35B8D,0xEF139552,0x0A6C1839,0x0F30CB7E,0xB2A5F6F9,0xA13362C3,0xC54164B6,0xCCC44439,
	};
	unsigned int q[]=
	{
		0xAF4320EF,0x8ECCD1FA,0xBE1F8AE4,0x9D0C0C56,0x49025CFA,0x1F6224A5,0x72B40453,0xC62E01FF,
		0x6F146D59,0x8AB8A843,0xD7F6F422,0xCA147CD9,0x78625EA6,0xD91BC4B6,0x2954BE7C,0xFA4C673D,
		0x75427994,0xD29DC754,0x2E82F1F2,0xFAFC8024,0x96CE9658,0x38CA488F,0xC513CDC9,0x5FCB6D50,
		0x40B6717F,0x04E87686,0xF08CA191,0x1962EA84,0x1DA3F1E9,0x62A6F4E4,0x090B757D,0xAB6845AD,
		0x0E004FB5,0x9F58016B,0x204B6BD7,0xA2EF5817,0x49640C37,0xC5B18DB6,0x35484B6D,0x45722C94,
		0x8A5AAAE2,0x1DF3FDEA,0x4E56A319,0xF04E82C7,0x9CC01B3A,0x4AB13C56,0x11B6F605,0x20B95F23,
		0x26A2C3C1,0x3A00D23A,0xF4B1614A,0x9C37D84E,0x038A1DF0,0xA334B566,0x992C2A3E,0xB1975139,
		0x0A62D321,0x49177AEF,0x71C6A5EC,0x0A645045,0x95C04A77,0xB8979ED9,0x446AE998,0xC4EA238F,
	};
	unsigned int dp[]=
	{
		0xE5799D39,0x9A1CC297,0xA31A2792,0x5FC2387C,0xF063C4CC,0x11928FF9,0x572A0E1F,0xFB6ADCA2,
		0xFA228739,0x4D28D038,0xD4A42B6D,0xAD65B32D,0x3DDC43FC,0xA4B162C2,0xFA4DF45E,0xB817EB15,
		0xE9EADBAF,0x3BA295EC,0x6034FFFD,0xF03707C0,0x6D0A8D24,0x0B63136E,0xB29B290B,0xC9931094,
		0x05BFD648,0xFD76800F,0x584AF029,0x8D4F815C,0xEFF5B2A9,0x0C7E9112,0xC5EB9B9D,0x175C767D,
		0x301EC8E9,0xCB285B32,0x765B8889,0x2236C830,0x53297C20,0x1CC15923,0x4DA00C39,0x7636298D,
		0x3C480D6B,0x4EA88CA2,0xC4CFF6F3,0x43452C1F,0x985F829C,0x3316F568,0xBDC8FB67,0x149F922A,
		0x4F771B80,0x16EA159B,0x9E4ACB16,0xC2A1D4BD,0x5E7511AD,0x0859A11F,0xAD78A1F8,0x365B2FE4,
		0xD73808D7,0x634E3E3F,0x0021D18B,0xA1A46B47,0x1D7186C0,0x7A6AA52C,0x08B67EB3,0x1D39DE2B,
	};
	unsigned int dq[]=
	{
		0xB31A719D,0x1797E4C4,0xA6890501,0x7786645D,0x88BC8431,0xF498F8F0,0x21836AB3,0x906E95AB,
		0xB7BD61E5,0x8F44CCDC,0x667B457F,0x12832C92,0x566AA2EE,0xF2E68BBB,0x310EB5F0,0xE7BF713C,
		0x2693A35B,0xAF107371,0x496A52FA,0x1374C5B8,0x48D4865E,0xA7AF6175,0xCA011C9B,0x2BE02F07,
		0x8BD2C776,0x4877EC77,0x66EEA0AE,0xBC682A55,0xA8E694D6,0x513016EB,0x6ED22EBB,0x73152E2B,
		0x12A818C8,0x1297C082,0x62AC07C4,0xB2F75991,0x0FBC57B8,0xC8295594,0x82659456,0xE9350A5C,
		0x843D2C04,0x2F9AD4B9,0x35667E91,0x6E97B01A,0x4808E49A,0x7509019B,0x1C4A7D11,0x895F7503,
		0x6CBBD5DF,0x1A0A6B79,0x6DDFC198,0x6D87F259,0xD4223808,0x4CBFE0F0,0xA1CEB423,0xC896876A,
		0xFFA5D1C8,0xA9206F71,0x3E98BD0F,0xB65296F5,0xACDFC0D6,0x632D2830,0xD1E7C2FA,0xC43E9C45,
	};
	unsigned int u[]=
	{
		0xEF4906CD,0x535332A5,0x964B8FC6,0x3BF17B0A,0x6F2A359E,0x413C68CA,0x892EA1E3,0x437E2C90,
		0xDC837BEB,0xF9E80AA2,0x6DEB95F9,0x55C85E78,0x71EB8333,0x5F815BF2,0xA6A46DB6,0x3BEA94D0,
		0x4D3F774D,0xBAE32518,0xB5505E0A,0xE30BEAC9,0x9E14C3E5,0xB037D667,0x141DF9F8,0xA28924D9,
		0xFC2918EA,0x291CE43B,0xA92B80DE,0xF8F0CA55,0x8CF11EB7,0xFC990287,0x6F65740E,0x2A2EEB7D,
		0xCB61AF03,0x8F970D5B,0x8D1B9A26,0x224F7708,0x679D6076,0xBA69497F,0x4D1E1030,0xAB541089,
		0x1F94E5C6,0x569D9272,0xC72B348D,0x251E38D5,0xE8C73EF0,0x2B352D4A,0x3E8F8C1F,0x6468A993,
		0x832F1372,0x7B752EF0,0x4A3D700B,0x1A21EBDD,0x25EACFFC,0xECEFC3FA,0xD0EC4C79,0xE0FC2FBC,
		0xA7E65DE2,0x9A2B594C,0xC43C2C06,0x2D0C822F,0x3DC22B69,0x3DBFDF17,0x28F46E9A,0x8A88EF32,
	};
	unsigned int std_out[]=
	{
		0x5DFBF726,0x5EDF920F,0x2C6FBC35,0x216BB3DF,0x676C3526,0xE7891D10,0x8F1E3961,0x1DC36F63,
		0xC276A87A,0xB5681B77,0x360156B1,0x62456253,0xAAA85E9C,0x3ECADB05,0x9912DA41,0xD623F7DC,
		0xF5F18B71,0x8816DA8A,0xEA5C26DF,0xE25396F4,0xA58FDD02,0x0B16A18F,0xBBDC579B,0xF74B6D37,
		0xD554F13D,0x49923779,0xB76E6EF4,0xF2F7E936,0xBCB81F39,0x255C8F9D,0x6C1C4AE5,0xC4D2EEC7,
		0xDFFA2FE4,0xE82769D8,0x991E9AF1,0xA5C2ABAB,0xAD7DCF43,0x90F733BF,0x3AA1C1FC,0x8DBB1417,
		0xE72A29E1,0x5AD59D45,0x30EE4D52,0x6CFEE5FD,0x744E6583,0x2957B491,0xB3FA767E,0x25A3897D,
		0x94D059E6,0x45ED9A1C,0x5DBB5DF5,0xFF692B25,0x5A10F7AA,0xF28F4F55,0xFDE5CBA2,0x479843B0,
		0x49E3175D,0x79B2CBCE,0x1FB0C6BC,0xED739AF8,0xDC10E5C8,0x5139E7FA,0x63872534,0x073E5F90,
		0xFE2D17DF,0xB17D111B,0xFA57F75D,0x704982D2,0xEC79126B,0x6BAC2356,0xD5220343,0xC7C0F9AA,
		0x6C7CF5A2,0xCFBF0582,0x788C305C,0x3990C845,0xEBA12CA4,0x82D420E7,0x55A712C5,0xDE767648,
		0x6AAEA5A6,0xC1F13A35,0xEBAABDE7,0x5DD445F7,0xBEA107B4,0x4802EDDE,0x7ACF3C03,0x9B6F674D,
		0x90770900,0xAFB6EDCC,0xA85B52B2,0x5A7C9970,0x9B7D7653,0xA5BD01C8,0x357B0BC3,0xAADD26AA,
		0xDB7FE6FA,0x14623989,0xAA00B6F6,0x388A5C56,0x73230AB8,0xC3BF5E53,0x04D34F4D,0xFB9F84DF,
		0xFF1C4407,0xC77EE6FB,0xC8C5419B,0x43499208,0x9C4E55E8,0xE8483B5E,0x7EAB2875,0x22D75118,
		0x662FE1E6,0x512B5999,0x26D252C3,0x7BEA1D49,0x6467B0FF,0x4879FEB5,0x02B2DCBF,0x518CE56B,
		0x67DE0CBB,0xF1CF8E0D,0x6C1C2703,0x628D8B87,0x0BA9EE79,0x5CB74C74,0x7CD3B62F,0x0C5A1274,
	};

	unsigned int in[256], out[256], out2[256];
	unsigned int eBitLen = 17;
	unsigned int nBitLen = 4096;
	unsigned int wordLen=(nBitLen+31)/32;
	unsigned int ret;

	//get_H(n, out, wordLen);print_BN_buf_U32(n, wordLen, "n");print_BN_buf_U32(out, wordLen, "out");
	//set input
	memset(in, 0x5a, wordLen<<2);

	//sign(non-CRT mode with pub)
//printf("\r\n begin     ");
//for(int j=0;j<28;j++){
	ret = RSA_ModExp(in, d, n, out, nBitLen, nBitLen);
//}printf("\r\n end     ");
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA MOD EXP 4096 test 1 failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 4096 test 1 success");
	}

	//sign(CRT mode with pub)
	memset(out, 0, wordLen<<2);
//printf("\r\n begin     ");
//for(int j=0;j<28;j++){
	ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
//printf("\r\n end     ");
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA CRT MOD EXP 4096 test 1 failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA CRT MOD EXP 4096 test 1 success");
	}

	//verify
//printf("\r\n begin     ");
//for(int j=0;j<800;j++){
	ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
//}printf("\r\n end     ");
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out2, wordLen, in, wordLen))
	{
		printf("\r\n RSA MOD EXP 4096 test 2 failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 4096 test 2 success");
	}

	return 0;
}
#endif
unsigned int rsa_test(void)
{
	unsigned int e[128]={0};
	unsigned int d[128];
	unsigned int n[128];
	unsigned int p[128];
	unsigned int q[128];
	unsigned int dp[128];
	unsigned int dq[128];
	unsigned int u[128];
	unsigned int in[128];
	unsigned int out[128];
	unsigned int out2[128];
	unsigned int eBitLen=17;
	unsigned int nBitLen=1024;
//	unsigned int eWordLen=(eBitLen+0x1F)>>5;
	unsigned int nWordLen=(nBitLen+0x1F)>>5;
//	unsigned int pWordLen=(nBitLen/2+0x1F)>>5;
	unsigned int ret;
	unsigned int i;

#if 1
	//RSA NON-CRT
	eBitLen=17;
	nBitLen=1024;//only B93 supports 2048 and 4096
//	eWordLen=(eBitLen+0x1F)>>5;
	nWordLen=(nBitLen+0x1F)>>5;
//	pWordLen=(nBitLen/2+0x1F)>>5;
	for(i=0;i<2;i++)
	{
		ret = RSA_GetKey(e, d, n, eBitLen, nBitLen);
		if(ret)
		{
			printf("\r\n RSA_GetKey failure, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_GetKey");
#if 1
		//test encrypt & decrypt
		uint32_set(in, 0x1, nWordLen);
		ret = RSA_ModExp(in, d, n, out, get_valid_bits(d, nWordLen), nBitLen);
		if(ret)
		{
			printf("\r\n RSA_ModExp error, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_ModExp1");
		ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
		if(ret)
		{
			printf("\r\n RSA_ModExp error, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_ModExp2");
		if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
		{
			printf("\r\n RSA encrypt & decrypt error");
			return 1;
		}
		else
		{
			printf("\r\n RSA encrypt & decrypt success");
		}
		printf("\r\n uint32_BigNumCmp");
		//corner case: 0^0 mod n
		uint32_clear(in, nWordLen);
		ret = RSA_ModExp(in, in, n, out, 0, nBitLen);
		if(RSA_SUCCESS == ret)
		{
			printf("\r\n RSA_ModExp corner test 1 error, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_ModExp3");
		//corner case: base is n
		ret = RSA_ModExp(n, e, n, out, eBitLen, nBitLen);
		if(RSA_SUCCESS == ret)
		{
			printf("\r\n RSA_ModExp corner test 2 error, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_ModExp4");
#endif
		printf("\r\n RSA_GetKey times = %d", i+1);
	}
#endif

#if 1
	//RSA CRT
	eBitLen=17;
	nBitLen=1024;//only B93 supports 2048 and 4096
//	eWordLen=(eBitLen+0x1F)>>5;
	nWordLen=(nBitLen+0x1F)>>5;
//	pWordLen=(nBitLen/2+0x1F)>>5;
	for(i=0;i<2;i++)
	{
		ret = RSA_GetCRTKey(e, p, q, dp, dq, u, n, eBitLen, nBitLen);
		if(ret)
		{
			printf("\r\n RSA_GetCRTKey failure, ret = %d", ret);
			return 1;
		}
#if 1
		//test encrypt & decrypt
		uint32_set(in, 0x1, nWordLen);
//printf("\r\n begin     ");
//for(int j=0;j<25000;j++){
		ret = RSA_ModExp(in, e, n, out, eBitLen, nBitLen);
//}
//printf("\r\n end     ");
		if(ret)
		{
			printf("\r\n RSA_ModExp error, ret = %d", ret);
			return 1;
		}

//printf("\r\n begin     ");
//for(int j=0;j<2500;j++){
		ret = RSA_CRTModExp(out, p, q, dp, dq, u, out2, nBitLen);
//}
//printf("\r\n end     ");
		if(ret)
		{
			printf("\r\n RSA_CRTModExp error, ret = %d", ret);
			return 1;
		}

		if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
		{
			printf("\r\n RSA CRT encrypt & decrypt error");
			return 1;
		}
		else
		{
			printf("\r\n RSA CRT encrypt & decrypt success");
		}

		//corner case: base is 0
		uint32_clear(in, nWordLen);
		ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
		if(RSA_SUCCESS != ret || (1 != uint32_BigNum_Check_Zero(out, nWordLen)))
		{
			printf("\r\n RSA_CRTModExp corner test 1 error, ret = %d", ret);
			return 1;
		}

		//corner case: base is 1
		uint32_clear(in, nWordLen);
		in[0]=1;
		ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
		if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out, nWordLen, in, nWordLen)))
		{
			printf("\r\n RSA_CRTModExp corner test 2 error, ret = %d", ret);
			return 1;
		}

		//corner case: base is 2
		uint32_clear(in, nWordLen);
		in[0]=2;
		ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
		ret |= RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
		if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)))
		{
			printf("\r\n RSA_CRTModExp corner test 3 error, ret = %d", ret);
			return 1;
		}

		//corner case: base is n-2
		uint32_copy(in, n, nWordLen);
		in[0]-=2;
		ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
		ret |= RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
		if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)))
		{
			printf("\r\n RSA_CRTModExp corner test 4 error, ret = %d", ret);
			return 1;
		}

		//corner case: base is n-1
		uint32_copy(in, n, nWordLen);
		in[0]-=1;
		ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
		if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out, nWordLen, in, nWordLen)))
		{
			printf("\r\n RSA_CRTModExp corner test 5 error, ret = %d", ret);
			return 1;
		}

		//corner case: base is n
		ret = RSA_CRTModExp(n, p, q, dp, dq, u, out, nBitLen);
		if(RSA_SUCCESS == ret)
		{
			printf("\r\n RSA_CRTModExp corner test 6 error, ret = %d", ret);
			return 1;
		}
#endif
		printf("\r\n RSA_GetCRTKey times = %d", i+1);
	}

#endif

	return 0;
}


unsigned char RSA_all_test(void)
{
	printf("\r\n\r\n =================== rsa test =================== \r\n");

	if(rsa_modexp_1024_test())
		return 1;
#if (MCU_CORE_B93)
	if(rsa_modexp_2048_test())
		return 1;

	if(rsa_modexp_4096_test())
		return 1;
#endif
	delay_ms(5);
	if(rsa_test())
		return 1;

	return 0;
}

#endif


void user_init(void)
{
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED1); 		//enable output
	gpio_output_en(LED2);
	gpio_output_en(LED3);
	gpio_input_dis(LED1);		//disable input
	gpio_input_dis(LED2);
	gpio_input_dis(LED3);
	gpio_set_high_level(LED1);

	RSA_all_test();
}

void main_loop(void)
{
	delay_ms(500);

	gpio_toggle(LED1);
}
#endif

#endif
