#include "Node_Manager.h"

HANDLE hMutex = 0;

ULONG64 external_current_NODE_SECTION_INDEX = 0;
PDynamic_NODE external_start_node = NULL;
PDynamic_NODE external_current_node = NULL;



// ��� �� ���� �Լ� 
PDynamic_NODE Build_up_Node(
	PUCHAR DATA,
	ULONG32 DATA_SIZE,

	BOOLEAN is_init,
	ULONG64* inout_NODE_SECTION_INDEX, // �����ϸ� ��� �ν� �ε�����ȣ�� �Ҵ�ްԵ� 
	PDynamic_NODE* output_Start_Node_of_NODE_SECTION // index ���� ������ �����ּ� 
) {
	if (is_init) {
		if (output_Start_Node_of_NODE_SECTION == NULL) return NULL;
	}
	else {
		if (inout_NODE_SECTION_INDEX == NULL) return NULL;
	}

	WaitForSingleObject(hMutex, INFINITE);

	if (external_current_node == NULL) {
		if (is_init) {
			external_current_NODE_SECTION_INDEX++;
			external_current_node = Create_Node(NULL, external_current_NODE_SECTION_INDEX, DATA, DATA_SIZE);
			external_start_node = external_current_node;
		}
		else {
			external_current_node = Create_Node(NULL, *inout_NODE_SECTION_INDEX, DATA, DATA_SIZE);
			external_start_node = external_current_node;
		}
		
	}
	else {
		if (is_init) {
			external_current_NODE_SECTION_INDEX++;
			external_current_node = Append_Node(external_current_node, external_current_NODE_SECTION_INDEX, DATA, DATA_SIZE);

		}
		else {
			external_current_node = Append_Node(external_current_node, *inout_NODE_SECTION_INDEX, DATA, DATA_SIZE);
		}
	}

	if (is_init) {
		*inout_NODE_SECTION_INDEX = external_current_NODE_SECTION_INDEX;
		*output_Start_Node_of_NODE_SECTION = external_current_node;
	}
	

	//print_node(external_start_node);

	ReleaseMutex(hMutex);
}



/// <summary>
/// 
/// </summary>
/// <param name="Previous_Node"></param>
/// <param name="NODE_SECTION_INDEX"></param>
/// <param name="DATA"></param>
/// <param name="DATA_SIZE"></param>
/// <returns></returns>
PDynamic_NODE Create_Node(PUCHAR Previous_Node, ULONG64 NODE_SECTION_INDEX, PUCHAR DATA, ULONG32 DATA_SIZE) {

	PDynamic_NODE New_Node = (PDynamic_NODE)malloc(sizeof(Dynamic_NODE));
	if (New_Node == NULL) return NULL;
	memset(New_Node, 0, sizeof(Dynamic_NODE));


	New_Node->Previous_Node = Previous_Node;

	New_Node->NODE_SECTION_INDEX = NODE_SECTION_INDEX;

	// ������ ��������
	New_Node->DATA = (PUCHAR)malloc(DATA_SIZE);
	if (New_Node->DATA == NULL) {
		free(New_Node);
		return NULL;
	}
	memcpy(New_Node->DATA, DATA, DATA_SIZE);

	New_Node->DATA_SIZE = DATA_SIZE;

	New_Node->Next_Node = NULL;

	return New_Node;

}

PDynamic_NODE Append_Node(PDynamic_NODE NODE, ULONG64 NODE_SECTION_INDEX, PUCHAR DATA, ULONG32 DATA_SIZE) {

	PDynamic_NODE New_Node = Create_Node((PUCHAR)NODE, NODE_SECTION_INDEX, DATA, DATA_SIZE);

	New_Node->Previous_Node = (PUCHAR)NODE;

	NODE->Next_Node = (PUCHAR)New_Node;

	return New_Node;

}

VOID print_node(){
	if (external_start_node == NULL || external_current_node == NULL) return;

	PDynamic_NODE current_node = external_start_node;

	do {
		printf("����ε���:[%d] /  ��� �ּ�: %p  /  �����ͻ�����: %d \n", current_node->NODE_SECTION_INDEX, current_node, current_node->DATA_SIZE);
		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node!=NULL);
	printf("\n\n");
}

//

BOOLEAN Remove_Node_internal(PDynamic_NODE Specified_Node);

PDynamic_NODE Remove_Node(PDynamic_NODE NODE_SECTION_Start_Address) {
	WaitForSingleObject(hMutex, INFINITE);
	print_node();
	PDynamic_NODE current_node = external_start_node;
	ULONG64 NODE_SECTION_INDEX = 0xFFFFFFFF;

	ULONG64 current_Maximum_of_NODE_SECTION_INDEX = 0;// ��ü Section��� ������ ���� ū ���� ���ϰ�, �ű⿡�� ���� ���� ��ȣ ������Ʈ

	do {

		PDynamic_NODE tmp = (PDynamic_NODE)current_node->Next_Node;// �̸� next��� ���.

		// �����ּҺ��� ���� index����
		if ((PUCHAR)NODE_SECTION_Start_Address == (PUCHAR)current_node) {
			NODE_SECTION_INDEX = NODE_SECTION_Start_Address->NODE_SECTION_INDEX;
		}



		// index����� �� ��� ������� ����.
		 if (NODE_SECTION_INDEX != 0xFFFFFFFF) {
			 if (current_node->NODE_SECTION_INDEX == NODE_SECTION_INDEX) {
				 /*
					 ���� ���Ḯ��Ʈ ���� �������ְ� �Ҵ������ϱ�.
				 */
				 Remove_Node_internal(current_node);


			 }
			 else {
				 /* NODE_SECTION_INDEX �Ķ���� ������ ū ��� - 1 �� �ϱ�.*/
				 if (current_node->NODE_SECTION_INDEX > NODE_SECTION_INDEX) {
				 	current_node->NODE_SECTION_INDEX--;
				 }
			 }
		}

		 // �������� - ��� ���� �ε��� �������� ������Ʈ�� ����,
		 if (current_Maximum_of_NODE_SECTION_INDEX < current_node->NODE_SECTION_INDEX) {
			 current_Maximum_of_NODE_SECTION_INDEX = current_node->NODE_SECTION_INDEX;
		 }
		

		current_node = tmp;
	} while (current_node != NULL);

	// ���� ��ȣ ������Ʈ 
	external_current_NODE_SECTION_INDEX = current_Maximum_of_NODE_SECTION_INDEX;

	print_node();
	ReleaseMutex(hMutex);
}

BOOLEAN Remove_Node_internal(PDynamic_NODE Specified_Node) {
	if ((external_start_node == NULL) || (external_current_node == NULL))return FALSE;

	if (Specified_Node->Previous_Node == NULL && Specified_Node->Next_Node == NULL) {
		external_start_node = NULL;
		external_current_node = NULL;
	}
	else if(Specified_Node->Previous_Node == NULL && Specified_Node->Next_Node) {
		((PDynamic_NODE)Specified_Node->Next_Node)->Previous_Node = NULL;
		external_start_node = (PDynamic_NODE)Specified_Node->Next_Node;
	}
	else if(Specified_Node->Previous_Node && Specified_Node->Next_Node == NULL) {
		((PDynamic_NODE)Specified_Node->Previous_Node)->Next_Node = NULL;
		external_current_node = (PDynamic_NODE)Specified_Node->Previous_Node;
	}
	else if(Specified_Node->Previous_Node && Specified_Node->Next_Node) {
		((PDynamic_NODE)Specified_Node->Previous_Node)->Next_Node = Specified_Node->Next_Node;
		((PDynamic_NODE)Specified_Node->Next_Node)->Previous_Node = Specified_Node->Previous_Node;
	}

	free(Specified_Node->DATA);
	free(Specified_Node);

	return TRUE;
}


BOOLEAN is_valid_address(PDynamic_NODE NODE_SECTION_Start_Address) {
	if (external_start_node == NULL || external_current_node == NULL) return FALSE;

	PDynamic_NODE current_node = external_start_node;

	do {
		
		if (NODE_SECTION_Start_Address == current_node) {
			return TRUE;
		}

		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node != NULL);

	return FALSE;
}


PDynamic_NODE Get_Node(
	PDynamic_NODE NODE_SECTION_Start_Address,
	ULONG32 node_count_for_field
) {// ��� �������� ( count )

	if (external_start_node == NULL || external_current_node == NULL ) return FALSE;

	PDynamic_NODE current_node = external_start_node;
	BOOLEAN is_found_start_node = FALSE;


	ULONG64 remember_NODE_SECTION_INDEX = 0xFFFFFFFF;
	ULONG32 count = 0;

	do {

		if (NODE_SECTION_Start_Address == current_node) {
			is_found_start_node =  TRUE;
			remember_NODE_SECTION_INDEX = current_node->NODE_SECTION_INDEX;
		}
		
		if (is_found_start_node && remember_NODE_SECTION_INDEX!= 0xFFFFFFFF && current_node->NODE_SECTION_INDEX == remember_NODE_SECTION_INDEX) {
			printf("node_count_for_field -> %d / %d \n", node_count_for_field, count);
			if (node_count_for_field == count) {
				return current_node; // ��� ���� 
			}
			count++;
		}

		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node != NULL);

	return NULL;
}