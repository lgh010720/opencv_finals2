프로젝트 - 1 내용 생략

__1. 이진화, 모폴로지 닫기연산 전처리 이후 Countours 버튼 좌클릭시 그린 숫자의 외곽선 갯수 출력__

  외곽선 1개일시 예상숫자 1, 2, 3, 4, 5, 7

  외곽선 2개일시 예상숫자 0, 4, 6, 9

  외곽선 3개일시 숫자 8로 인식 숫자 8 출력

__2 .이진화 모폴로지 닫기연산 전처리 이후 Center 버튼 좌클릭시 그린 숫자의 외곽선의 무게중심 출력__

  외곽선이 2개여서 무게중심이 2개일 경우 외곽선1의 무게중심과 외곽선2의 무게중심 위치 비교

  무게중심 1의 y좌표가 무게중심 2의 y좌표값보다 클 경우(무게중심 1이 무게중심 2보다 더 아래에 있을경우) 숫자 6으로 인식 숫자 6 출력

  무게중심 1의 y좌표가 무게중심 2의 y좌표값보다 작을 경우(무게중심 1이 무게중심 2보다 더 위에 있을경우) 숫자 9로 인식 숫자 9 출력

  무게중심 1의 y좌표와 무게중심 2의 y좌표 차이가 10 이하로 매우 근접할 경우 숫자 0으로 인식 숫자 0 출력
