#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include <stdio.h>
#include <Windows.h>


typedef struct Dynamic_NODE {

	PUCHAR Previous_Node;

	ULONG64 NODE_SECTION_INDEX;// ��� ��� �νĿ� 

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
);// ��� �������� ( count ) 0���� �����Ѵ�. 

BOOLEAN is_valid_address(PDynamic_NODE NODE_SECTION_Start_Address); // �ּ� ��ȿ�� �˻� 

VOID print_node();

// ��� �� ���� �Լ� 
PDynamic_NODE Build_up_Node(
	PUCHAR DATA,
	ULONG32 DATA_SIZE,

	BOOLEAN is_init,
	ULONG64* inout_NODE_SECTION_INDEX, // �����ϸ� ��� �ν� �ε�����ȣ�� �Ҵ�ްԵȴ�. ( is_init �� ���� ) 
	PDynamic_NODE* output_Start_Node_of_NODE_SECTION // index ���� ���� ���� ��� �ּҸ� ��ȯ ( index���ŵ� �� ���� )  
);


#endif // 
