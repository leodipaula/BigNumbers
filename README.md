# BigNumbers
Projeto à matéria Programação Estruturada - UFABC. O projeto visa somar, subtrair e multiplicar números cujo o tamanho limite é a memória da máquina.


**Integrantes:**

Aymê Cardoso Gonçalves - RA: 11202111547
Leonardo de Paula Fernandes - RA: 11202131805
Vinicius Lopes Romero - RA: 11202111495


Em nosso trabalho, o BigNumber foi representado através de uma string, aplicando uma lista com os dígitos, além da funcionalidade de leitura dinâmica.
A adição e subtração são realizadas manualmente, operando individualmente nos dígitos e evitando alocações desnecessárias de memória, o método de 
"papel e caneta" particiona o problema inicial em problemas menores, evitando uma possível sobrecarga na máquina, o que melhora o tempo de execução
do código, que com a incoporação de técnicas de reversão de strings o torna ainda mais eficiente.



Essas são as funções disponíveis para manipulação de números grandes (BigNumber). Aqui está um breve resumo do propósito de cada função: 


`createBigNumber` Aloca memória para uma nova instância de BigNumber e retorna um ponteiro para ela. 
`destroyBigNumber` Libera a memória associada a uma instância de BigNumber. 
`setBigNumberFromString` Define o valor de um BigNumber a partir de uma string. 
`getBigNumberAsString` Obtém uma representação de string do valor de um BigNumber. 
`addBigNumbers` Adiciona dois BigNumber e armazena o resultado em um terceiro BigNumber. 
`subBigNumbers` Subtrai dois BigNumber e armazena o resultado em um terceiro BigNumber. 
`multiplyBigNumbers` Multiplica dois BigNumber e armazena o resultado em um terceiro BigNumber.



A **divisão do trabalho** foi feita da seguinte maneira: Leonardo estruturou todos os arquivos (bignumber.h, client.c, bignumber.c e makefile), solucionando
o problema incial de receber os numeros gigantes e somá-los, enquanto que Vinicius implementou a função de subtrai-los e Aymê, iniciou a solução para 
multiplicá-los com o algoritmo de karatsuba, juntamente com os demais integrantes, posteriormente resolvida da forma tradicional, por Leonardo.



A organização e desenvolvoimento do trabalho foram feitos colaborativamente no **Git**, o repositório hospedado no **GitHub** está disponível em: [texto](github.com/leodipaula/BigNumbers)
