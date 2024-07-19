#include "Node_Manager.h"

HANDLE hMutex = 0;

ULONG64 external_current_NODE_SECTION_INDEX = 0;
PDynamic_NODE external_start_node = NULL;
PDynamic_NODE external_current_node = NULL;



// 노드 총 관리 함수 
PDynamic_NODE Build_up_Node(
	PUCHAR DATA,
	ULONG32 DATA_SIZE,

	BOOLEAN is_init,
	PDynamic_NODE* output_Start_Node_of_NODE_SECTION, // index 섹션 노드들의 시작주소 

	ULONG32 Node_Search_VALUE
	
) {
	if (output_Start_Node_of_NODE_SECTION == NULL) return NULL;

	WaitForSingleObject(hMutex, INFINITE);

	if (external_current_node == NULL) {
		if (is_init) {
			external_current_NODE_SECTION_INDEX++;
			external_current_node = Create_Node(NULL, NULL, external_current_NODE_SECTION_INDEX, DATA, DATA_SIZE, Node_Search_VALUE);
			external_start_node = external_current_node;
		}
		else {
			external_current_node = Create_Node(NULL, NULL, ((PDynamic_NODE)*output_Start_Node_of_NODE_SECTION)->NODE_SECTION_INDEX , DATA, DATA_SIZE, Node_Search_VALUE);
			external_start_node = external_current_node;
		}
		
	}
	else {
		if (is_init) {
			external_current_NODE_SECTION_INDEX++;
			external_current_node = Append_Node(NULL,external_current_node, external_current_NODE_SECTION_INDEX, DATA, DATA_SIZE, Node_Search_VALUE);

		}
		else {
			external_current_node = Append_Node(((PDynamic_NODE)*output_Start_Node_of_NODE_SECTION)->NODE_SECTION_START_NODE_ADDRESS,external_current_node, ((PDynamic_NODE)*output_Start_Node_of_NODE_SECTION)->NODE_SECTION_INDEX, DATA, DATA_SIZE, Node_Search_VALUE);
		}
	}

	if (is_init) {
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
PDynamic_NODE Create_Node(PUCHAR SECTION_START_NODE_ADDRESS, PUCHAR Previous_Node, ULONG64 NODE_SECTION_INDEX, PUCHAR DATA, ULONG32 DATA_SIZE, ULONG32 Node_Search_VALUE) {

	PDynamic_NODE New_Node = (PDynamic_NODE)malloc(sizeof(Dynamic_NODE));
	if (New_Node == NULL) return NULL;
	memset(New_Node, 0, sizeof(Dynamic_NODE));

	/*
		항상 모든 노드들은 자신의 섹션에서 "시작노드" 주소 값을 가지고 있어야한다. (바뀌면 안되며, 일관성유지)
	*/
	if (SECTION_START_NODE_ADDRESS==NULL) {
		//최초 노드
		New_Node->NODE_SECTION_START_NODE_ADDRESS = (PUCHAR)New_Node; // 섹션 노드들의 시작 주소 저장
	}
	else {
		//Append 노드 
		New_Node->NODE_SECTION_START_NODE_ADDRESS = (PUCHAR)SECTION_START_NODE_ADDRESS;
	}

	New_Node->Previous_Node = Previous_Node;

	New_Node->NODE_SECTION_INDEX = NODE_SECTION_INDEX;

	// 데이터 새로저장
	New_Node->DATA = (PUCHAR)malloc(DATA_SIZE);
	if (New_Node->DATA == NULL) {
		free(New_Node);
		return NULL;
	}
	memcpy(New_Node->DATA, DATA, DATA_SIZE);

	New_Node->DATA_SIZE = DATA_SIZE;

	//
	New_Node->Node_Search_VALUE = Node_Search_VALUE;

	New_Node->is_end_node = TRUE; // 일단 마지막 노드일 수 밖에 없으니까,,

	New_Node->Next_Node = NULL;

	return New_Node;

}

PDynamic_NODE Append_Node(PUCHAR SECTION_START_NODE_ADDRESS, PDynamic_NODE NODE, ULONG64 NODE_SECTION_INDEX, PUCHAR DATA, ULONG32 DATA_SIZE, ULONG32 Node_Search_VALUE) {


	PDynamic_NODE New_Node = Create_Node(SECTION_START_NODE_ADDRESS,(PUCHAR)NODE, NODE_SECTION_INDEX, DATA, DATA_SIZE, Node_Search_VALUE);

	New_Node->Previous_Node = (PUCHAR)NODE;

	if (SECTION_START_NODE_ADDRESS == NULL) {
		NODE->is_end_node = TRUE; // UPDATE
	}
	else {
		NODE->is_end_node = FALSE; // UPDATE
	}
	

	NODE->Next_Node = (PUCHAR)New_Node;

	return New_Node;

}

VOID print_node(){
	if (external_start_node == NULL || external_current_node == NULL) return;

	PDynamic_NODE current_node = external_start_node;

	do {
		printf("노드인덱스:[%d] /  노드 주소: %p  /  데이터사이즈: %d / 노드_서칭_값: %d  / 노드 끝인가? %d \n", current_node->NODE_SECTION_INDEX, current_node, current_node->DATA_SIZE, current_node->Node_Search_VALUE, current_node->is_end_node);
		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node!=NULL);
	printf("\n\n");
}

//

BOOLEAN Remove_Node_internal(PDynamic_NODE Specified_Node);

BOOLEAN Remove_Node(PDynamic_NODE NODE_SECTION_Start_Address) {
	WaitForSingleObject(hMutex, INFINITE);
	print_node();
	PDynamic_NODE current_node = external_start_node;
	ULONG64 NODE_SECTION_INDEX = 0xFFFFFFFF;

	ULONG64 current_Maximum_of_NODE_SECTION_INDEX = 0;// 전체 Section노드 숫자중 가장 큰 값을 구하고, 거기에서 전역 섹션 번호 업데이트

	do {

		PDynamic_NODE tmp = (PDynamic_NODE)current_node->Next_Node;// 미리 next노드 기억.

		// 시작주소부터 얻어야 index얻음
		if ((PUCHAR)NODE_SECTION_Start_Address == (PUCHAR)current_node) {
			NODE_SECTION_INDEX = NODE_SECTION_Start_Address->NODE_SECTION_INDEX;
		}



		// index얻었을 때 노드 연쇄삭제 가능.
		 if (NODE_SECTION_INDEX != 0xFFFFFFFF) {
			 if (current_node->NODE_SECTION_INDEX == NODE_SECTION_INDEX) {
				 /*
					 이중 연결리스트 서로 연결해주고 할당해제하기.
				 */
				 Remove_Node_internal(current_node);


			 }
			 else {
				 /* NODE_SECTION_INDEX 파라미터 값보다 큰 경우 - 1 씩 하기.*/
				 if (current_node->NODE_SECTION_INDEX > NODE_SECTION_INDEX) {
				 	current_node->NODE_SECTION_INDEX--;
				 }
			 }

			 // 전역변수 - 노드 섹션 인덱스 전역변수 업데이트를 위한,
			 if (current_Maximum_of_NODE_SECTION_INDEX < current_node->NODE_SECTION_INDEX) {
				 current_Maximum_of_NODE_SECTION_INDEX = current_node->NODE_SECTION_INDEX;
			 }

		}

		 
		

		current_node = tmp;
	} while (current_node != NULL);

	// 섹션 번호 검증
	if (NODE_SECTION_INDEX == 0xFFFFFFFF) return FALSE;

	// 섹션 번호 업데이트 
	external_current_NODE_SECTION_INDEX = current_Maximum_of_NODE_SECTION_INDEX;

	print_node();
	ReleaseMutex(hMutex);

	return TRUE;
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


PDynamic_NODE Get_Node_1Dim(
	PDynamic_NODE NODE_SECTION_Start_Address,
	ULONG32 node_count_for_field
) {// 노드 가져오기 ( count )

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
				return current_node; // 노드 추출 
			}
			count++;
		}

		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node != NULL);

	return NULL;
}


PDynamic_NODE Get_Node_memcmp_1Dim(
	PDynamic_NODE NODE_SECTION_Start_Address,
	PUCHAR DATA,
	ULONG32 DATA_SIZE
) {

	if (external_start_node == NULL || external_current_node == NULL) return FALSE;

	PDynamic_NODE current_node = external_start_node;
	BOOLEAN is_found_start_node = FALSE;


	ULONG64 remember_NODE_SECTION_INDEX = 0xFFFFFFFF;

	do {

		if (NODE_SECTION_Start_Address == current_node) {
			is_found_start_node = TRUE;
			remember_NODE_SECTION_INDEX = current_node->NODE_SECTION_INDEX;
		}

		if (is_found_start_node && remember_NODE_SECTION_INDEX != 0xFFFFFFFF && current_node->NODE_SECTION_INDEX == remember_NODE_SECTION_INDEX) {
			
			if (current_node->DATA_SIZE >= DATA_SIZE && memcmp(DATA, current_node->DATA, DATA_SIZE)==0   ) {
				return current_node; // 노드 추출 
			}
		}

		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node != NULL);

	return NULL;
}

//

PDynamic_NODE Get_Node_2Dim(
	PDynamic_NODE NODE_SECTION_Start_Address, // 시작점 
	PUCHAR DATA,
	ULONG32 DATA_SIZE,
	ULONG32 Node_Search_VALUE, // 노드 식별용

	PDynamic_NODE* OUTPUT_SECTION_START_ADDRESS
) {
	if (external_start_node == NULL || external_current_node == NULL) return NULL;

	PDynamic_NODE current_node = external_start_node;


	do {

		if (NODE_SECTION_Start_Address->Node_Search_VALUE == current_node->Node_Search_VALUE) {

			if ((current_node->DATA_SIZE >= DATA_SIZE) && memcmp(current_node->DATA, DATA, DATA_SIZE) == 0) {
				
				*OUTPUT_SECTION_START_ADDRESS = (PDynamic_NODE)current_node->NODE_SECTION_START_NODE_ADDRESS;
			}

		}

		current_node = (PDynamic_NODE)current_node->Next_Node;
	} while (current_node != NULL);

	return NULL;
}
