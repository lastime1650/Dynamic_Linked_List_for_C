#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include <stdio.h>
#include <Windows.h>


typedef struct Dynamic_NODE {

	PUCHAR Previous_Node;

	ULONG64 NODE_SECTION_INDEX;// 노드 덩어리 인식용 

	PUCHAR DATA;
	ULONG32 DATA_SIZE;

	PUCHAR Next_Node;

}Dynamic_NODE, *PDynamic_NODE;

extern HANDLE hMutex;

PDynamic_NODE Create_Node(PUCHAR Previous_Node, ULONG64 NODE_SECTION_INDEX, PUCHAR DATA, ULONG32 DATA_SIZE);

PDynamic_NODE Append_Node(PDynamic_NODE NODE, ULONG64 NODE_SECTION_INDEX, PUCHAR DATA, ULONG32 DATA_SIZE);

PDynamic_NODE Remove_Node(PDynamic_NODE NODE_SECTION_Start_Address);

PDynamic_NODE Get_Node(
	PDynamic_NODE NODE_SECTION_Start_Address, 
	ULONG32 node_count_for_field
);// 노드 가져오기 ( count ) 0부터 시작한다. 

BOOLEAN is_valid_address(PDynamic_NODE NODE_SECTION_Start_Address); // 주소 유효성 검사 

VOID print_node();

// 노드 총 관리 함수 
PDynamic_NODE Build_up_Node(
	PUCHAR DATA,
	ULONG32 DATA_SIZE,

	BOOLEAN is_init,
	ULONG64* inout_NODE_SECTION_INDEX, // 성공하면 노드 인식 인덱스번호를 할당받게된다. ( is_init 일 때만 ) 
	PDynamic_NODE* output_Start_Node_of_NODE_SECTION // index 섹션 시작 점의 노드 주소를 반환 ( index갱신될 때 대비용 )  
);


#endif // 
