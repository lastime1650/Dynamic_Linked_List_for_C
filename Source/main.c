#include <stdio.h>
#include <Windows.h>

#include "Node_Manager.h"

DWORD WINAPI test(LPVOID lpParam);

int main() {

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("���ؽ� ���� ����: %d\n", GetLastError());
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
    printf("����\n");
	return 0;
}

DWORD WINAPI test(LPVOID lpParam) {

    ULONG64 index = 0;
    PDynamic_NODE Section_Start_Node = NULL;

    for (ULONG32 i = 0; i < 2; i++) {

        ULONG32 sample1 = i;

        if (i == 0) {
            Build_up_Node((PUCHAR)&sample1, sizeof(sample1), TRUE, &index, &Section_Start_Node);
            printf("Build_up_Node ���κ��� ���� �����ּ� %p \n", Section_Start_Node);
            continue;
        }
        
        Build_up_Node((PUCHAR) & sample1, sizeof(sample1), FALSE,&index, NULL);
    }

    PDynamic_NODE res = Get_Node(Section_Start_Node, 2);
    if (res) {
        printf("[������] ������ ���� -> %d \n", res->DATA_SIZE);//3���� ��� ��
    }

   

    Remove_Node(Section_Start_Node);

    // Section_Start_Node �ּҰ���  ���̻� ��ȿ���� ���� 
    printf(" %p is_valid_address ��ȿ��� -> %d \n", Section_Start_Node, is_valid_address(Section_Start_Node)); // 0
    
    return 0;
}