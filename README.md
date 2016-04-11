# Micromouse

Projekt robota rozwiązującego labirynt - Micromouse.

Repozytorium jest zbiorem kodu do robota. Całość napisana pod procesor 32-bitowy ARM marki TI.

Kod jest zbiorem funkcji obsługujących czynności niskopoziomowe, takie jak
obsługa silników, czujników, komunikacji Bluetooth (w większości zrealizowane),
ale także zbiorem funkcji wyższego poziomu implementujących algorytmy rozwiązujące
labirynt i pokonujące go w najszybszym czasie. (do zrobienia).

Na chwilę obecną zrealizowane zostało :
- napisano sterownik niskopoziomowy do obsługi silników oraz regulator prędkościowy PID
- napisano funkcje odpowiadające za przechwytywanie komunikatów przez Bluetooth
i wyzwalanie przy tym przyporządkowanych nim procedur.
- Napisano obsługę niskopoziomową czujników odległości złożonych z fototranzystora
oraz diody IR
- Napisano bardzo istotny czujnik poziomu baterii. Zastosowano akumulator LiPo, co wymaga
baczności podczas korzystania.
- Napisano funkcje odpowiadające za interrakcję z płytką wierzchną, na której znajdują się
dwa przyciski, trzy diody LED oraz buzzer.

Zastosowany procesor oferuje bardzo szeroki zakres peryferiów, dlatego duża część zadań wykonuje
się sprzętowo, w tle (np. pomiary ADC, generowanie PWM etc).

Duża część kodu wykonuje się w przerwaniach, które są możliwie krótkie i nie pochłaniają
pozostałego czasu procesora, dzięki czemu może on cały czas zajmować się zagadką labiryntu.

