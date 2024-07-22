# Dynamic_Linked_List_for_C
비동기 처리가 가능한 동적 필드 전용 연결리스트

이 코드는 Windows.h 유저모드 윈도우즈 헤더를 이용하여 작성되었습니다.

---

# 이 연결리스트는 무엇인가? 자세한 것은 아래 블로그 링크를 참조하세요!

**[https://blog.naver.com/lastime1650/223517385494](https://blog.naver.com/lastime1650/223517385494)**

---

![initial](https://github.com/lastime1650/Dynamic_Linked_List_for_C/blob/main/images/image.png)

현재 위 사진과 같은 구성도로 구현되었습니다. 


비동기가 아닌, 순차적으로 실행했을 때는 아래와 같은 이미지로 print 됩니다. 

![initial](https://github.com/lastime1650/Dynamic_Linked_List_for_C/blob/main/images/p1.png)

---

# 2024년 07월 22일

1. Append_Node 함수를 전격 수정하여, 비동기에서 작동가능하도록 재수정함. 

2. **[NODE_RELATION_INDEX](https://github.com/lastime1650/Dynamic_Linked_List_for_C/blame/3f568e30823213097570682b56c837b16334cfcb/Source/Node_Manager_logic.c#L157) ** 라는 같은 섹션내에서 사용되는 상대적 인덱스 값을 추가함!

# 2024년 07월 19일

1. 섹션 단위로 관리하는 함수가 추가됨

2. 전체적인 함수 파라미터 추가 수정됨 

# 2024년 07월 18일 

기본 연결리스트와 다르게, 데이터와 데이터사이즈만을 가진 노드 여러개를 통하여 동적+비동기 스레드에서도 사용가능한 연결리스트 구현됨

동시에, 같은 연결리스트 자원에 전혀 다른 데이터를 노드 하나씩에 묶어서 사용가능.
