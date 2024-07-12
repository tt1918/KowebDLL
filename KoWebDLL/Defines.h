#pragma once

#define MAX_CROSS_WIDTH		2048
#define MAX_DEFECT			99
#define MAX_CANDIDATE_AREA	1280

#define CANDI_WHITE_LOW		10
#define CANDI_PRESS_LOW		20

#define BAD_IMG_WIDTH		128
#define BAD_IMG_HEIGHT		128

#define USE_CHECK_SIZE		8

#define MAX_LEVEL			10

#define MILLION				1000000
#define CLASSDEV			10
#define OPTICDEV			100
#define PERIODDEV			10000
#define MAX_OPTIC			10

#define MAX_SCRATCH			10

#define MAX_BADIMAGE_FILENAME	100

#define PARAM_NAME			"Parameter.ini"
#define SYSTEM_FILE_NAME	"System.ini"

#define COS1			0x01	// SH 크로스
#define COS2			0x02	// 투영반사
#define COS3			0x03	// 미분투과 
#define COS4			0x04	// 정투과
#define COS5			0x05	// 투영/경계투과
#define COS6			0x06	// 정반사B(보호면검사)
#define COS7			0x07	// 정반사A(이형면검사) 
#define COS8			0x08	// 경계투과  
#define COS9			0x09	// 크로스
#define COSA			0x00	// 투영반사  =========================>원래 0x0A를 사용해야 하나 LG에서 이렇게 사용함.
#define COSB			0x0B	// 크로스2 (2동코팅3에만 사용함)
#define COSC			0x0C	//
#define COSD			0x0D	//
#define COSE			0x0E	//
#define CBCR			0x0F	//


