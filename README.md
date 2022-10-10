# Calculadora

Nome: Beatriz Favini Chicaroni  
RA 11201721608  
  
O código foi baseado no projeto tictactoe disponível em https://hbatagelo.github.io/abcgapps/src/tictactoe.zip  

O projeto é um protótipo de calculadora com as funções de somar, subtrair e multiplicar algarismos.

A lógica principal está em window.cpp, cujas classes e funções estão declaradas em window.hpp.

A função onCreate é chamada uma única vez no início da execução do programa e inicializa a janeja do openGL.  
A função clearMemory zera a variável fstNumber e coloca a calculadora no estado firstNumber, que aguarda a entrada de um novo algarismo.  
A função onPaintUI contém todas as mudanças de estado da janela de acordo com as interações realizadas pelo usuário. Ela inicia declarando o tamanho, a posição e algumas propriedades da janela (linhas 18 a 28).  
Das linhas 30 a 43 está a implementação do menu, que nesse caso possui apenas um botão clear que chama a função clearMemory.  
Das linhas 45 a 76 está implementado o "visor" da calculadora. Ele se baseia no estado da janela para exibir a operação que está sendo construíca pelo usuário.  
Das linhas 96 a 137 estão definidos os textos dos botões.  
Da linha 140 à linha 171 está a lógica responsável por alternar entre os estados da janela:

O programa inicia no estado fstNumber, quando o usuário escolhe o primeiro algarismo passa para o estado signal, que espera a seleção da operação a ser realizada. Quando a operação é selecionada o programa espera no estado sndNumber até o usuário selecionar o segundo algarismo. Quando recebe o segundo algarismo o programa realiza a operaçõ e aguarda no estado waitEqual até o usuário apertar o sinal de igual. Nesse momento o programa passa para o estado Result e apresenta o resultado da operação no "visor".
