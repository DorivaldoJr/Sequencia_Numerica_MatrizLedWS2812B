Controle Sequencial de Números na Matriz de LEDs - BitDogLab

Descrição do Projeto

Este projeto foi desenvolvido para a placa BitDogLab utilizando o microcontrolador RP2040, explorando o uso de interrupções para controle de botões e manipulação da matriz de LEDs WS2812 (5x5).

O sistema permite ao usuário incrementar e decrementar um contador exibido na matriz de LEDs ao pressionar os botões A e B. Além disso, um LED vermelho pisca 5 vezes por segundo de forma contínua. 
Para garantir a precisão na detecção dos botões, foi implementado um tratamento de debounce via software.

Componentes Utilizados
	•	Placa BitDogLab
	•	Matriz de LEDs 5x5 WS2812 (Conectada à GPIO 7)
	•	LED RGB (Pinos 11, 12, 13)
	•	Botão A (Conectado à GPIO 5)
	•	Botão B (Conectado à GPIO 6)

Modo de Uso
	•	O LED vermelho pisca 5 vezes por segundo para indicar que o sistema está rodando.
	•	A matriz de LEDs exibe um número de 0 a 9 de forma fixa.
	•	O botão A (GPIO 5) incrementa o número exibido.
	•	O botão B (GPIO 6) decrementa o número exibido.
	•	Implementação de debouncing para evitar leituras repetidas acidentais dos botões.

Detalhes da Implementação
	1.	Matriz de Números (5x5)
Cada número é representado por um array de 25 posições, invertido no eixo horizontal para a correta exibição na BitDogLab.
	2.	Interrupções para os Botões
	•	Cada botão tem sua própria interrupção via GPIO_IRQ_EDGE_FALL.
	•	O estado do botão só é registrado se passarem 200ms desde o último acionamento (debouncing).
	3.	Manipulação da Matriz WS2812
	•	Utiliza PIO (Programmable I/O) para comunicação eficiente com os LEDs.
	•	As cores são controladas pelo método matrix_rgb().

Este projeto foi desenvolvido por Dorivaldo Jesus da Silva Júnior, baseado em estudos e referências sobre sistemas embarcados e interrupções no RP2040.
