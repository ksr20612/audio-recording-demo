# audio-recording-demo
sound capturing &amp; playback with ALSA (using QT Creator)

1. 초안(20.09.03)
[녹음기 데모 프로그램 소개.pptx](https://github.com/ksr20612/DONGYEON_PROJECT/files/5182348/default.pptx)

2. 1차 수정(20.09.21)
 - record-stop 버튼으로 녹음 기간 조정 가능화
 - 녹음 시간 표시
 - 재생 파일 표시
 - 그 외 자잘한 코드 수정
 
3. 2차 수정(20.09.22~)
 - 사용자 UI 보강
 -- 모든 parameter tuning이 가능하도록 수정
 -- 사용자 UI 측에서 parameter(sampleSize, sampleRate, channel, fileName 등) 입력받아 넘길 것.
 
 - 예외처리 추가
 -- 존재하지 않는 파일명 입력시 에러메세지 출력
