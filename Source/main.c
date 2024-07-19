#include <stdio.h>
#include <Windows.h>

#include "Node_Manager.h"

DWORD WINAPI test(LPVOID lpParam);

int main() {

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("뮤텍스 생성 실패: %d\n", GetLastError());
        return 1;
    }

    ULONG32 test_parm1 = 1;

    CreateThread(NULL, 0, test, &test_parm1, 0, NULL);

    /*
    ULONG32 test_parm2 = 2;
    CreateThread(NULL, 0, test, &test_parm2, 0, NULL);
    
    ULONG32 test_parm3 = 3;
    CreateThread(NULL, 0, test, &test_parm3, 0, NULL);
    ULONG32 test_parm4 = 4;
    CreateThread(NULL, 0, test, &test_parm4, 0, NULL);
    
    test_parm =5;
    CreateThread(NULL, 0, test, &test_parm, 0, NULL);
    test_parm = 6;
    CreateThread(NULL, 0, test, &test_parm, 0, NULL);
    test_parm = 7;
    CreateThread(NULL, 0, test, &test_parm, 0, NULL);
    test_parm = 8;
    CreateThread(NULL, 0, test, &test_parm, 0, NULL);
    */

    
    while (1) {
    }
    printf("성공\n");
	return 0;
}

PDynamic_NODE make_linked_list_for_test(ULONG32 start_i, ULONG32 end_i, ULONG32 Node_Search_VALUE);

DWORD WINAPI test(LPVOID lpParam) {




    PDynamic_NODE Section_Start_Node1 = make_linked_list_for_test(0,10,'D1');


    PDynamic_NODE res1 = Get_Node_1Dim(Section_Start_Node1, 2);
    if (res1) {
        printf("[1삭제전] 데이터 추출 -> %d \n", res1->DATA_SIZE);//3번쨰 노드 값
    }

    ///

    PDynamic_NODE Section_Start_Node2 = make_linked_list_for_test(0, 10, 'D2');

    PDynamic_NODE res2 = Get_Node_1Dim(Section_Start_Node2, 2);
    if (res2) {
        printf("[2삭제전] 데이터 추출 -> %d \n", res2->DATA_SIZE);//3번쨰 노드 값
    }

    ///

    PDynamic_NODE Section_Start_Node3 = make_linked_list_for_test(0, 10, 'D1');


    PDynamic_NODE res3 = Get_Node_1Dim(Section_Start_Node3, 2);
    if (res3) {
        printf("[3삭제전] 데이터 추출 -> %d \n", res3->DATA_SIZE);//3번쨰 노드 값
    }
   
    print_node();

    //Remove_Node(Section_Start_Node);

    // Section_Start_Node 주소값은  더이상 유효하지 않음 
    //printf(" %p is_valid_address 유효결과 -> %d \n", Section_Start_Node, is_valid_address(Section_Start_Node)); // 0
    
    return 0;
}

PDynamic_NODE make_linked_list_for_test(ULONG32 start_i, ULONG32 end_i, ULONG32 Node_Search_VALUE) {
    PDynamic_NODE Section_Start_Node = NULL;

    for (start_i = 0; start_i < end_i; start_i++) {

        ULONG32 sample1 = start_i;

        if (start_i == 0) {
            Build_up_Node((PUCHAR)&sample1, sizeof(sample1), TRUE, &Section_Start_Node, Node_Search_VALUE);
            //printf("Build_up_Node 으로부터 받은 시작주소 %p \n", Section_Start_Node);
            continue;
        }

        Build_up_Node((PUCHAR)&sample1, sizeof(sample1), FALSE, &Section_Start_Node, Node_Search_VALUE);
    }

    return Section_Start_Node;
}
