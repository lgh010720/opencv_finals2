

프로젝트 - 1 내용 생략

이진화, 모폴로지 닫기연산 전처리

__1. Countours 버튼 좌클릭시 그린 숫자의 외곽선 갯수 출력__

  외곽선 1개일시 예상숫자 1, 2, 3, 4, 5, 7

  외곽선 2개일시 예상숫자 0, 4, 6, 9

  외곽선 3개일시 숫자 8로 인식 숫자 8 출력

__2 . Center 버튼 좌클릭시 그린 숫자의 외곽선의 무게중심 출력__

  외곽선 2개, 무게중심 2개일 경우 외곽선1의 무게중심과 외곽선2의 무게중심 위치 비교

  무게중심 1의 y좌표가 무게중심 2의 y좌표값보다 클 경우(무게중심 1이 무게중심 2보다 더 아래에 있을경우) 숫자 6으로 인식 숫자 6 출력

  무게중심 1의 y좌표가 무게중심 2의 y좌표값보다 작을 경우(무게중심 1이 무게중심 2보다 더 위에 있을경우) 숫자 9로 인식 숫자 9 출력

  무게중심 1의 y좌표와 무게중심 2의 y좌표 차이가 10 이하로 매우 근접할 경우 숫자 0으로 인식 숫자 0 출력

  __3. Ratio 버튼 좌클릭시 그린 숫자의 가로 세로 비율 출력__

  숫자를 감싸는 바운딩박스를 그리고 바운딩박스의 가로 세로 길이 측정 후 세로/가로 나누기
 
  세로 / 가로로 나눴을때 비율이 3.5이상이면 1로 판별 

  __4. Line 버튼 좌클릭시 그린 숫자의 이미지의 픽셀값을 낮춘 후 addWeighted 함수로 만든 선 이미지의 픽셀값을 덧씌워 숫자부분과 선부분이 접촉하는 면적을 색출__

  2, 3, 4, 5, 7중 중앙보다 약간 위에 가로선을 그을시 2개가 접촉하는숫자는 4뿐 4출력

  1개가 접촉시 세로선을 그음

  2, 3, 5, 7중 중앙 세로선을 그을시 1개 혹은 2개가 접촉하는 숫자는 7뿐 7출력

  3개가 접촉시 오른쪽> 왼쪽 대각선을 그음

  2, 3, 5중 오른쪽 왼쪽 대각선에 접촉하는 면적이 2개인 수는 2뿐 2출력

  3개가 접촉시 좌측 상단 대각선을 그음

  3, 5중 좌측상단 대각선에 접촉하는 면적이 1개인 수 3, 3출력

  3, 5중 좌측상단 대각선에 접촉하는 면적이 2개인 수 5, 5출력
