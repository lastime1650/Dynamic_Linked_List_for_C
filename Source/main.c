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
    ULONG32 test_parm2 = 2;
    CreateThread(NULL, 0, test, &test_parm2, 0, NULL);
    
    ULONG32 test_parm3 = 3;
    CreateThread(NULL, 0, test, &test_parm3, 0, NULL);
    ULONG32 test_parm4 = 4;
    CreateThread(NULL, 0, test, &test_parm4, 0, NULL);
    /*
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

DWORD WINAPI test(LPVOID lpParam) {

    ULONG64 index = 0;
    PDynamic_NODE Section_Start_Node = NULL;

    for (ULONG32 i = 0; i < 2; i++) {

        ULONG32 sample1 = i;

        if (i == 0) {
            Build_up_Node((PUCHAR)&sample1, sizeof(sample1), TRUE, &index, &Section_Start_Node);
            printf("Build_up_Node 으로부터 받은 시작주소 %p \n", Section_Start_Node);
            continue;
        }
        
        Build_up_Node((PUCHAR) & sample1, sizeof(sample1), FALSE,&index, NULL);
    }

    PDynamic_NODE res = Get_Node(Section_Start_Node, 2);
    if (res) {
        printf("[삭제전] 데이터 추출 -> %d \n", res->DATA_SIZE);//3번쨰 노드 값
    }

   

    Remove_Node(Section_Start_Node);

    // Section_Start_Node 주소값은  더이상 유효하지 않음 
    printf(" %p is_valid_address 유효결과 -> %d \n", Section_Start_Node, is_valid_address(Section_Start_Node)); // 0
    
    return 0;
}